#include <winsock.h>
#include <stdio.h>

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#define SIZEMAX 100000

void init(void) {
#ifdef WIN32
    WSADATA wsa;
    int err = WSAStartup(MAKEWORD(2, 2), &wsa);
    if(err < 0) {
        puts("WSAStartup failed !");
        exit(EXIT_FAILURE);
    }
#endif
}

void end(void) {
#ifdef WIN32
    WSACleanup();
#endif
}


void Send(SOCKET sock, const char * message) {
    printf("-> : %s\n",message);
    send( sock, message, (int)strlen( message ), 0 );
}
void Receive(SOCKET sock) {
    char buf[SIZEMAX]= {0};
    recv( sock, buf, SIZEMAX, 0 );
    printf("<- : %s\n",buf);
}

void SendReceive(SOCKET sock,const char * message) {
    Send(sock,message);
    Receive(sock);
}

SOCKET connexion(const char* server_name,unsigned short port) {
    /* on récupère les informations de l'hôte auquel on veut se connecter */
    struct hostent *hostinfo = gethostbyname( server_name );
    if (hostinfo == NULL) { /* l'hôte n'existe pas */
        fprintf (stderr, "Unknown host %s.\n", server_name);
        exit(EXIT_FAILURE);
    }

    /* Configuration de la connexion */
    SOCKADDR_IN sin;
    sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
    sin.sin_port = htons( port );
    sin.sin_family = AF_INET;

    /* Création de la socket */
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET)
        exit(EXIT_FAILURE);

    /* connection */
    if(connect(sock, (SOCKADDR*)&sin, sizeof(sin)) == SOCKET_ERROR)
        exit(EXIT_FAILURE);

    return sock;
}

static const unsigned char e_64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

char * encode64(const char *str255) { //Voir projet base 64
    size_t taille = strlen(str255);
    //size_t SIZE_BUF_64 = (((taille-1)/3)+1)*4;//Dimention exacte du tableau en base 64
    //char * str64 = calloc(SIZE_BUF_64+1,sizeof(char));//+1 Pour '/0'
    //if(!str64)exit(EXIT_FAILURE);

    static char str64[100]= {0}; //En statique
    memset(str64,0,100);

    size_t curseursource=0;
    size_t curseurdest=0;
    size_t i;
    int donnesource;
    size_t octetmanquant = (3-taille%3)%3;
    while(curseursource<taille) {
        donnesource=0;
        for(i=0; i<=2; i++) {
            donnesource |= str255[curseursource]<<8*(2-i);
            curseursource++;
        }
        for(i=0; i<=3; i++) {
            str64[curseurdest]= curseursource<taille || i<=3-octetmanquant? e_64[(donnesource&(0b111111<<(6*(3-i))))>>(6*(3-i))] : '=';
            curseurdest++;
        }
    }
    return str64;
}

int SMTP(SOCKET sock,const char* from,const char* to,const char* objet,const char* message) {
    char buffer[SIZEMAX];

    Receive(sock);//message de bienvenue
    SendReceive(sock, "EHLO pirx.ml\n");

    //Connection au serveur SMTP (ID/MDP hebergeur)
    SendReceive(sock, "AUTH LOGIN\n");
    sprintf(buffer,"%s\n",encode64("identifiant"));
    SendReceive(sock, buffer);//Identifiant
    sprintf(buffer,"%s\n",encode64("mot de passe"));
    SendReceive(sock, buffer);//Mdp


    sprintf(buffer,"MAIL FROM: <%s>\n",from);
    SendReceive(sock, buffer);
    sprintf(buffer,"RCPT TO: <%s>\n",to);
    SendReceive(sock, buffer);

    //Transfert du message :
    SendReceive(sock, "DATA\n");
    sprintf(buffer, "Subject: %s\n",objet);
    Send(sock, buffer);
    Send(sock, message);
    Send(sock, ".\n");//Fin de message
    Receive(sock);

    SendReceive(sock,  "QUIT\n");
    return 0;

}

#define SERVER "smtp-pirx.exemple.com"
#define PORT 587

//Adresse emission
#define FROM "test@test.com"
//Adresse reception
#define TO "adresse@dommaines.com"

int main() {
    init();

    SOCKET sock;
    sock = connexion(SERVER,PORT);

    const char * objet   = "Hello World";
    const char * message = "Salut !\nLigne 1 \nPierre \n";

    SMTP(sock,FROM,TO,objet,message);

    shutdown( sock, 2 );
    closesocket( sock );

    end();
    return 0;
}
