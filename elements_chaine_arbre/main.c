#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

//Define pour ajout/suppression data (Deviendra vite inutile : on ne souhaite finalement pas ajoueter de data entre les elements) Le seul ajout possible de data se fera en fin de Liste chaine
#define FIN UINT_MAX
#define DEBUT 0
//Define pour get()
#define ALL UINT_MAX //la profondeur : le nb file a chercher est +inf = TOUTES


typedef char* STR; //Permer de caster facilement la data en (char*)=(STR)
//NOte : toute la data sera en char* ! (-> entier -> reel ->chaine)


///Les differents etats d'un element
typedef enum VALUE VALUE;
enum VALUE {V_STRING, V_NUMBER, V_OBJET, V_ARRAY, V_TRUE, V_FALSE, V_NULL};

char * printValue(VALUE x) {
    static const char * ret[8]= {"STR", "NUM", "OBJ", "ARR", "TRUE", "FALSE", "NULL"};
    return ret[x];
}

typedef void* ELEMENT_data;
typedef struct ELEMENT ELEMENT;
struct ELEMENT {
    VALUE value;//Le type d'element
    ELEMENT_data data;//Le contenue de l'element : il pointe soit vers de la data (STR) ou soit vers un autre element...
    ELEMENT *suivant;//Le suivant est lelement suivant de la liste
    size_t nbelements;//Dans le cas ou l'element est un OBJET, indique le nombre d'elements de l'objet //Sinon = 42
};
//La dimention inutile de la data data pourras faciliter par la suite l'allocation dynamique de la data (Plus de buffer : buffer = malloc(sizeof(char)*SIZEBUFF)

//Initialise un elemeent de la chaine (Toutes ses champs sont initialiser en OBJ sauf value -> modifie dans la fc insertionPos)
ELEMENT * initialisation(VALUE value) {
    ELEMENT *liste = malloc(sizeof(*liste));
    if (liste == NULL)exit(EXIT_FAILURE);
    liste->value = value;
    liste->suivant = NULL;//premier
    liste->data=NULL;
    liste->nbelements =0;
    return liste;
}

//Fonction classique d'une liste chaine
void insertionPos(ELEMENT *liste,size_t position,VALUE value,const ELEMENT_data nvdata) {
    //Check valeur pos :
    position = position>liste->nbelements?liste->nbelements:position;
    /* Création du nouvel élément */
    ELEMENT *nouveau =initialisation(value); //= malloc(sizeof(*nouveau));
    if (liste == NULL || nouveau == NULL)exit(EXIT_FAILURE);


    if(value==V_STRING || value==V_NUMBER) {
        nouveau->data = nvdata;
        nouveau->nbelements=42;//Ne sert a rien pour le moment -> permet de sauvegarde un nombre dans une cellule de data
    }



    if(position==0) {
        /* Insertion de l'élément au début de la LISTE */
        nouveau->suivant = liste->data;
        liste->data = nouveau;
    } else {
        /* Insertion de l'élément en milieu ou fin de LISTE */
        ELEMENT * actuel = liste->data;
        while (position-->1&&actuel->suivant!=NULL)actuel = actuel->suivant;
        nouveau->suivant = actuel->suivant;
        actuel->suivant = nouveau;
    }
    liste->nbelements+=1; // )= Smiley triste
}

//Fonction HS ! non mise a jour ! (Peut devenir utile ... = 0.00001 % )
/*
void suppressionPos(ELEMENT *liste,size_t position) {///T.T
    //Check valeur pos :
    position = position>liste->nbelements-1?liste->nbelements-1:position;

    if (liste == NULL)exit(EXIT_FAILURE);
    if(liste->nbelements>=1) {//Si il reste des cellules
        if(position==0) {//On veux supprimer la premiere cellule
            ELEMENT *aSupprimer = liste->suivant;
            liste->suivant = liste->suivant->suivant;
            free(aSupprimer);
            liste->nbelements-=1;
        } else {//On sveux suppriler la position'eme cellule
            ELEMENT * actuel = liste->suivant;
            while (position-->1)actuel = actuel->suivant;
            ELEMENT * save = actuel->suivant->suivant;
            free(actuel->suivant);
            actuel->suivant =save;
            liste->nbelements-=1;
        }
    }
}
void suppressionAllListe(ELEMENT *liste) {
    while (liste->suivant != NULL) {
        suppressionPos(liste,DEBUT);
    }
}
*/
//Inutile ! L'acces a la data ne sert a rien ! On dois connaitre son type !
//Fonction NON mise a jour
/*
ELEMENT_data getDataListe(ELEMENT * liste,size_t position) {
    ///Check valeur pos :
    position = position>liste->nbelements-1?liste->nbelements-1:position;
    ELEMENT * actuel = liste->suivant;
    while (position-->0)actuel = actuel->suivant;
    return actuel->data;
}
*/

//Prend en argument un OBJET et une position
//REtourne la position de l'element ciblé
ELEMENT * getElementListe(ELEMENT * liste,size_t position) {
    ///Check valeur pos :
    position = position>liste->nbelements-1?liste->nbelements-1:position;
    ELEMENT * actuel = liste->data;
    while (position-->0)actuel = actuel->suivant;
    return actuel;
}

//Fonction inutile (+/-) un get()->data = "" fait office de set !
/*
void setDataListe(ELEMENT * liste,size_t position,ELEMENT_data data) {
    ///Check valeur pos :
    position = position>liste->nbelements-1?liste->nbelements-1:position;
    ELEMENT * actuel = liste->suivant;
    while (position-->0)actuel = actuel->suivant;
    actuel->data=data;
}
*/

//Permet l'affichage d'un element ... Finalement, il est plus utile de retourner une (STR) que de l'afficher
/*
void printElement(ELEMENT * MichaelJackson) {
    switch(MichaelJackson->value) {
    case V_STRING:
        printf("(STRING)%s",(char*)MichaelJackson->data);
        break;
    case V_NUMBER:
        printf("(NUMBER)%s",(char*)MichaelJackson->data);
        break;
    default:
        printf("(PTR)%p",MichaelJackson);
        break;
    }
}
*/

//Retourne une chaine (formate) en fonction du type de l'element
#define SIZEBUFFER 50
char * sprintElement(ELEMENT *MichaelJackson) {
    char data[SIZEBUFFER]= {0};
    switch(MichaelJackson->value) {
    case V_STRING:
        sprintf(data,"(STR)%s",(char*)MichaelJackson->data);
        break;
    case V_NUMBER:
        sprintf(data,"(NUM)%s",(char*)MichaelJackson->data);
        break;
    default:
        sprintf(data,"(PTR)%p",MichaelJackson);
        break;
    }
    static char buffer[SIZEBUFFER];
    strcpy(buffer,data);
    return buffer;
}

//Retourne l'adresse de l'ELEMENT cible !
//size_t profondeur indique a qu'elle profondeur le programme dois arrter la lecture (permet de retourne l'adresse d'un OBJ)
//sans cela, la fonction s'emballe car un va_arg(ap, size_t); retourne quand meme une valeur meme si les elements restant sont inexistant.
#include <stdarg.h>
#define INFO 0
ELEMENT * get(size_t profondeur,ELEMENT * liste,...) {
    va_list ap;
    va_start(ap, liste);


    size_t n;
#if INFO
    printf("\n========================================");
#endif // INFO
    ///printf("\nobj");

    ELEMENT * save=(ELEMENT*)liste;

    //Si son etat est bien un objet, alors : On peux poursuivre la lecture
    while(save->value==V_OBJET && profondeur-->0) {
        n = va_arg(ap, size_t);
#if INFO
        printf("\nValue ELEMENT  \t: %s",printValue(save->value));
        printf("\nNB sous ELEMENT\t: %d",save->nbelements);
        printf("\nP ELEMENTS     \t: %p",save);
        printf("\nS ELEMENTS     \t: %p",save->suivant);
        printf("\nDATA ELEMENT   \t: %p",save->data);
        printf("\nGO ELEMENT     \t: %d",n);
#endif // INFO
        if(n>save->nbelements-1) {
            n=save->nbelements-1;
            //printf("\nErreur data ->");
            //printf("\nGO ELEMENT     \t: %d",n);
        }
        ///printf("[%d]",n);
        save = getElementListe((ELEMENT*)save,n);
#if INFO
        printf("\n&obj...[%d]=%p",n,save);
        printf("\n obj...[%d]=%p=%s",n,save->data,save->data);
        printf("\nValue element suivant : %s",printValue((save)->value));
        printf("\nValue taille element suivant : %d",save->nbelements);
        printf("\n---------------------");
#endif // INFO
    }
#if INFO
    printf("\n========================================");
#endif // INFO
    va_end(ap);
    ///printf("=%s",save->data);
    return save;
}

//Fonction recursive qui permet l'affichage formate complet de l'arbre (L'utilisateur ne dois pas l'utiliser)
void printListe(ELEMENT *liste,int profondeur) {
    size_t size;
    int i;
    ELEMENT * actuel = liste->data;
    for(size = 0; size < liste->nbelements; size++) {
        printf("\n");
        for(i=0; i<profondeur-1; i++)
            printf(" %c ",179);
        printf(" %c%c",liste->nbelements-1==size?192:195,26);

        if(actuel->value==V_STRING || actuel->value==V_NUMBER)
            printf("[%d] : (%s) %s",size,printValue(actuel->value),(STR)actuel->data);
        else {
            printf("[%d] : (%s) %p",size,printValue(actuel->value),actuel->data);
            //profondeur++;
            printListe(actuel,++profondeur);
            profondeur--;
        }
        actuel = actuel->suivant;
    }
}

//Fonction utilisateur : Affiche l'arbre (permet une bonne initialisation de la fonction recursive)
void printListeF(ELEMENT * liste) {
    if(liste){
        int profondeur = 1;
        printf("\n======================================================");
        printf("\n[0] OBJ");
        printListe(liste,profondeur);
        printf("\n======================================================");
    }else{
        printf("\nDonnees inexistantes");
    }
}

//Fonction recursive qui permet de liberer tous les element de l'arbre
//Note : il faut prevoir une gestion de la data (celle ci est statique pour le moment)
void freeListe(ELEMENT **liste) {
    size_t size;
    ELEMENT * actuel = (**liste).data;
    ELEMENT * temp   = (**liste).data;

    for(size = 0; size<(**liste).nbelements; size++) {
        if(actuel->value==V_STRING || actuel->value==V_NUMBER) {
            //free(actuel->data);A decommente lors de l'allocation dynamique de la data !
            actuel->data = NULL;//Le sort de la data. Dans ce cas la : On la met juste à null (elle n'est pas aloue dynamiquement) !
           //Note : cela supprime TOUS les Elements de type (NUMBER / STR)
            printf("\n[%d] : %s",size,(STR)actuel->data);
        } else {
            printf("\n[%d] : (OBJET)%p",size,actuel->data);
            freeListe(&actuel);
        }
        free(actuel);//On supprime l'element (tous ce qu'il contient est deja supprimer x))
        actuel = temp->suivant;
        temp = actuel;
    }
    free(*liste);
    *liste=NULL; //Note : cela supprime TOUS les Element de type OBJ !
}

int main() {

    printf("\ninitialisation");
    ELEMENT * obj = initialisation(V_OBJET);

    insertionPos(get(0,obj),0,V_NUMBER,(char*)"case[0]");
    insertionPos(get(0,obj),1,V_STRING,(char*)"case[1]");
    insertionPos(get(0,obj),2,V_OBJET,(STR)"Variable inutiliser");

    insertionPos(get(1,obj,2),0,V_NUMBER,(STR)"case[2][0]");
    insertionPos(get(1,obj,2),1,V_STRING,(STR)"case[2][1]");

    insertionPos(get(1,obj,2),2,V_OBJET,NULL);
    insertionPos(get(2,obj,2,2),0,V_NUMBER,(STR)"case[2][2][0]");
    insertionPos(get(2,obj,2,2),1,V_STRING,(STR)"case[2][2][1]");

    insertionPos(get(0,obj),3,V_STRING,(STR)"case[3]");

    insertionPos(get(1,obj,2),3,V_STRING,(STR)"case[2][3]");

    //get(ALL,obj,2,2,0)->data="SALUT";

    printf("\nAffichage : ");
    printf("\nobj[%d]=%s",0,sprintElement(get(ALL,obj,0)));
    printf("\nobj[%d]=%s",1,sprintElement(get(ALL,obj,1)));
    printf("\nobj[%d]=%s",2,sprintElement(get(1,obj,2)));

    printf("\nobj[%d][%d]=%s",2,0,sprintElement(get(ALL,obj,2,0)));
    printf("\nobj[%d][%d]=%s",2,1,sprintElement(get(ALL,obj,2,1)));

    printf("\nobj[%d][%d]=%s",2,2,sprintElement(get(2,obj,2,2)));
    printf("\nobj[%d][%d][%d]=%s",2,2,0,sprintElement(get(ALL,obj,2,2,0)));
    printf("\nobj[%d][%d][%d]=%s",2,2,1,sprintElement(get(ALL,obj,2,2,1)));

    printf("\nobj[%d]=%s",3,sprintElement(get(ALL,obj,3)));

    //printf("\n\n%s",(char*)((ELEMENT*)((ELEMENT*)((ELEMENT*)((ELEMENT*)((ELEMENT*)obj)->data)->suivant)->suivant)->suivant)->data);

    printListeF(obj);

    printf("\nFREE");
    freeListe(&obj);

    //la data est bien supprime :
    printListeF(obj);

    return 0;
}
