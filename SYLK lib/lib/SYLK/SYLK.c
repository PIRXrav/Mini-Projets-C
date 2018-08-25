/* SYLK.c */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "SYLK.h"
/* constants =========================================================== */

///La taille d'un cellulee (le nombre de char) (contient le \0)
#define SIZEMESSAGE 7

/* macro =============================================================== */

/** \brief Libere memoire avec mise a NULL du ptr
 *
 * \param _p
 * \return #define
 *
 */
#define FREE(_p) do{ \
	 free( (_p) ); \
     (_p) = NULL; \
}while(0)

/** \brief un assert() avec ajout de message personalise
 *
 * \param _expression : erreur si elle est inférieur ou égale à 0
 * \param _msg : le message a afficher
 * \param ... : ajout de variables (printf)
 * \return #define
 *
 */
#define MYASSERT(_expression,_msg, ...) \
    if(!(_expression)){\
        fprintf(stderr,"\n==============================================");\
        fprintf(stderr,"\nErreur :" _msg, ##__VA_ARGS__ );\
        fprintf(stderr,"\nExpression : %s",#_expression);\
        fprintf(stderr,"\nFichier : %s",__FILE__);\
        fprintf(stderr,"\nLigne : %d",__LINE__);\
        fprintf(stderr,"\n==============================================");\
        exit(EXIT_FAILURE);\
    }


/** \brief Teste la validite des arguments des fonctions SYLK
 *
 * \return #define
 *
 */
#define TEST_INPUT() \
    MYASSERT(I!=NULL,"Tableau Inexistant");\
    MYASSERT(x<I->x && y<I->y,"Cellule (%u;%u) Inexistante : cordonnees incorectes",x,y);



/* types =============================================================== */

typedef union CELL CELL;
union CELL {
    char message[SIZEMESSAGE];  /**< message  */
    long entier;                /**< entier */
    double flottant;            /**< flottant */
};

///Le type de cellule
typedef enum { VOID=0, MESSAGE=1, ENTIER=2, FLOTANT=3 } type;


/* structures ========================================================== */

struct SYLK_TAB {
    size_t x;               /**< Taille x SYLK_TAB */
    size_t y;               /**< Taille y SYLK_TAB */
    CELL *data;             /**< Contenue des cellules */
    type *etat;             /**< Etats des cellules */
};

/* private functions =================================================== */
static void SYLK_SetEtat(SYLK_TAB* I,size_t x,size_t y,type etat);
static type SYLK_GetEtat(SYLK_TAB* I,size_t x,size_t y);
static char atofsecu(char * str,double * result);
static char atolsecu(char * str,long int * result);
static type convertStr(char * str,CELL * data);
static void getLineC(FILE* F,size_t *x,size_t *y,char * data);

/** \brief retourne l'état d'une cellule (PRIVATE)
 *
 * \param I SYLK_TAB* Le tableau SYLA
 * \param x int
 * \param y int
 * \return type etat de la cellule
 *
 */
static type SYLK_GetEtat(SYLK_TAB* I,size_t x,size_t y) {
    TEST_INPUT()
    return I->etat[I->x*y+x];
}

/** \brief Définie l'etat d'une cellule (PRIVATE)
 *
 * \param I SYLK_TAB*
 * \param x int
 * \param y int
 * \param etat type de cellule
 * \return void
 *
 */
static void SYLK_SetEtat(SYLK_TAB* I,size_t x,size_t y,type etat) {
    TEST_INPUT()
    I->etat[I->x*y+x]=etat;
}


/** \brief Convertie une chaine en double avec une securite(PRIVATE)
 *
 * \param str char*
 * \param result double*
 * \return char
 *
 */
static char atofsecu(char * str,double * result) {
    //atof retourne 0 (un nombre) en cas d'échec.
    return (char)sscanf(str, "%lf", result);
}

/** \brief Convertie une chaine en longint avec une securite(PRIVATE)
 *
 * \param str char*
 * \param result long int*
 * \return char
 *
 */
static char atolsecu(char * str,long int * result) {
    //atof retourne 0 (un nombre) en cas d'échec.
    return (char)sscanf(str, "%ld", result);
}


/** \brief retourne le type d'une chaine de char + variable (PRIVATE)
 *
 * \param str char*
 * \param data CELL*
 * \return type
 *
 */
static type convertStr(char * str,CELL * data) {//tests concluants
    double f;
    long int d;
    char etat_f,etat_d;
    etat_f = atofsecu(str,&f);
    etat_d = atolsecu(str,&d);//Pour ne pas perdre l'avantage d'un long int !!
    if(!((double)d-f<0 || f-(double)d<0))etat_f=0;

    type r;
    if(etat_f) {
        data->flottant=f;
        r= FLOTANT;
    } else if(etat_d) {
        data->entier=d;
        r= ENTIER;
    } else {
        r= MESSAGE;
    }
    //printf("\nchaine : %s -> %d",str,r);
    return r;
}


/** \brief Lis une ligne formate C du fichier F (PRIVATE)
 *
 * \param F FILE*
 * \param x size_t* la position x lus
 * \param y size_t* la position y lue
 * \param data char* la variable a extraire
 * \return void
 *
 */
static void getLineC(FILE* F,size_t *x,size_t *y,char * data) {
    getc(F);//;
    do {
        switch(getc(F)) {
        case 'X':
            fscanf(F,"%u",x);
            break;
        case 'Y':
            fscanf(F,"%u",y);
            break;
        case 'K':
            fscanf(F,"%s",data);
            break;
        case 'E':
            fscanf(F,"%*s");//test
            break;
        }
    } while(getc(F)==';');//On lis la ligne tant quil y a des champs
    //Si il n'y a plus de champ, il mange le /n
}

/* public functions ==================================================== */

/** \brief Crée un tableau SYLK de dimention x*y
 *
 * \param x size_t
 * \param y size_t
 * \return SYLK_TAB*
 *
 */
SYLK_TAB* SYLK_NewTab(size_t x,size_t y) {
    fprintf(stderr,"\n Allocation ->");
    SYLK_TAB* I = malloc(sizeof(SYLK_TAB));
    MYASSERT(I != NULL,"malloc SYLK_TAB initialisation");
    printf("\nPosition I : \t%p",I);

    I->x = x;
    I->y = y;

    I->data = calloc(1,x*y*sizeof(CELL));//Evite de faire une boucle for [][] inutile : structure opaque
    MYASSERT(I->data != NULL,"malloc I->data initialisation");
    printf("\nPosition data :\t%p",I->data);

    //printf("\nTaille CELL : %d octets",sizeof(CELL));//?
    //printf("\nTaille data : %d octets",sizeof(*I->data));

    I->etat = calloc(x*y,sizeof(type));//Toutes les cellules sont initialisé à VOID = 0
    MYASSERT(I->etat != NULL,"malloc SYLK_TAB initialisation");
    printf("\nPosition etat :\t%p",I->etat);

    //printf("\nPosition I.x : \t%p",&I->x);
    //printf("\nPosition I.y : \t%p",&I->y);
    fprintf(stderr,"\n <- Allocation Reussit");
    return I;
}



/** \brief Libere la mémoire d'un tableau SYLK
 *
 * \param I SYLK_TAB**
 * \return void
 *
 */
void SYLK_FreeTab(SYLK_TAB** I) {//PREND &I !
    printf("\n Free -> ");
    MYASSERT(*I!=NULL,"Tableau inexistant, free impossible");
    FREE((**I).data);
    printf("\nPosition data :\t%p",(**I).data);
    FREE((**I).etat);
    printf("\nPosition etat :\t%p",(**I).etat);
    FREE(*I);
    printf("\nPosition I : \t%p",*I);
    printf("\n <- Free reussit");
}


/** \brief Retourne le message situe la case [y][x]
 *
 * \param I SYLK_TAB*
 * \param x size_t
 * \param y size_t
 * \return char*
 *
 */
char * SYLK_GetMessage(SYLK_TAB* I,size_t x,size_t y) {
    TEST_INPUT()
    MYASSERT(SYLK_GetEtat(I,x,y)==MESSAGE,"lecture cellule : ce n est pas un MESSAGE");
    //static char error[] = "NODATA";
    //if(SYLK_GetEtat(I,x,y))return I->data[I->x*y+x].message;
    //else return error;
    //les assertions empêche toute lecture impossible
    return I->data[I->x*y+x].message;
}

/** \brief Initialise le message situe la case [y][x]
 *
 * \param I SYLK_TAB*
 * \param x size_t
 * \param y size_t
 * \param message const char*
 * \return void
 *
 */
void SYLK_SetMessage(SYLK_TAB* I,size_t x,size_t y,const char * message) {
    TEST_INPUT()
    //Copie secu :
    strncpy(I->data[I->x*y+x].message,message,SIZEMESSAGE-1);
    I->data[I->x*y+x].message[SIZEMESSAGE-1]='\0';//A mettre si la data est definie en malloc ! le dernier char n'est jamsi modif (=0)
    //Definition de l'etat de la cellule
    SYLK_SetEtat(I,x,y,MESSAGE);
}


/** \brief Retourne le reel (double) situe la case [y][x]
 *
 * \param I SYLK_TAB*
 * \param x size_t
 * \param y size_t
 * \return double
 *
 */
double SYLK_GetFlottant(SYLK_TAB* I,size_t x,size_t y) {
    TEST_INPUT()
    MYASSERT(SYLK_GetEtat(I,x,y)==FLOTANT,"lecture cellule : ce n est pas un FLOTTANT");
    return I->data[I->x*y+x].flottant;
}

/** \brief Initialise le flottant situe la case [y][x]
 *
 * \param I SYLK_TAB*
 * \param x size_t
 * \param y size_t
 * \param flottant double
 * \return void
 *
 */
void SYLK_SetFlottant(SYLK_TAB* I,size_t x,size_t y,double flottant) {
    TEST_INPUT()
    I->data[I->x*y+x].flottant=flottant;
    SYLK_SetEtat(I,x,y,FLOTANT);
}

/** \brief Retourne l'entier (long int) situe la case [y][x]
 *
 * \param I SYLK_TAB*
 * \param x size_t
 * \param y size_t
 * \return long int
 *
 */
long int SYLK_GetEntier(SYLK_TAB* I,size_t x,size_t y) {
    TEST_INPUT()
    MYASSERT(SYLK_GetEtat(I,x,y)==ENTIER,"lecture cellule : ce n est pas un ENTIER");
    return I->data[I->x*y+x].entier;
}

/** \brief Initialise l'entier situe la case [y][x]
 *
 * \param I SYLK_TAB*
 * \param x size_t
 * \param y size_t
 * \param entier long int
 * \return void
 *
 */
void SYLK_SetEntier(SYLK_TAB* I,size_t x,size_t y,long int entier) {
    TEST_INPUT()
    I->data[I->x*y+x].entier=entier;
    SYLK_SetEtat(I,x,y,ENTIER);
}

/** \brief Initialise vide la case [y][x]
 *
 * \param I SYLK_TAB*
 * \param x size_t
 * \param y size_t
 * \return void
 *
 */
void SYLK_SetVoid(SYLK_TAB* I,size_t x,size_t y) {
    SYLK_SetEtat(I,x,y,VOID);
}

/** \brief Affiche un tableau SYLK
 *
 * \param I SYLK_TAB*
 * \return void
 *
 */
void SYLK_PrintTab(SYLK_TAB * I) {
    size_t i,j;
    printf("\n\n%s","TAB");
    printf("\t");
    for(i=0; i<I->x; i++)printf("|x%d\t",i);
    printf("\n--------");
    for(i=0; i<I->x; i++)printf("|-------");
    for(j=0; j<I->y; j+=1) {
        printf("\n y%d\t",j);
        for(i=0; i<I->x; i++) {
            switch(SYLK_GetEtat(I,i,j)) {
            case ENTIER:
                printf("|%ld\t",SYLK_GetEntier(I,i,j));
                break;
            case MESSAGE:
                printf("|%s\t",SYLK_GetMessage(I,i,j));
                break;
            case FLOTANT:
                printf("|%.0e\t",SYLK_GetFlottant(I,i,j));
                break;
            case VOID:
                printf("|VOID\t");
                break;
            }
        }
        printf("\n--------");
        for(i=0; i<I->x; i++)printf("|-------");
    }
}

/** \brief Crée un fichier slk (SYLK) a partir du tableau SYLK_TAB
 *
 * \param I SYLK_TAB*
 * \param fichier const char* : le nom du fichier (dois comporter l'extension)
 * \param id const char* : ID
 * \return char 0 en cas d'erreur / assertion en cas d'erreur
 *
 */
char SYLK_CreateFile(SYLK_TAB* I,const char* fichier,const char * id) {
    printf("\n============================================================");
    printf("\n Ecriture fichier SYLK ->");
    printf("\nFile : %s \nID : %s\nSize : X = %d et Y = %d",fichier,id,I->x,I->y);
    FILE* F = fopen(fichier,"w+");
    MYASSERT(F,"Impossible d'ouvrir le fichier %s",fichier);
    fprintf(F,"ID;%s",id);
    size_t i,j;
    for(j=0; j<I->y; j++) {
        for(i=0; i<I->x; i++) {
            switch(SYLK_GetEtat(I,i,j)) {
            case ENTIER:
                fprintf(F,"\nC;Y%d;X%d;K%ld",j+1,i+1,SYLK_GetEntier(I,i,j));
                break;
            case MESSAGE:
                fprintf(F,"\nC;Y%d;X%d;K\"%s\"",j+1,i+1,SYLK_GetMessage(I,i,j));
                //On rajoute des " ... "
                break;
            case FLOTANT:
                fprintf(F,"\nC;Y%d;X%d;K%.10E",j+1,i+1,SYLK_GetFlottant(I,i,j));
                break;
            case VOID:
                //Do nothing
                break;
            }
            //y en premier
            //La ligne/colonne 0/0 n'exisye pas dans un SYLK_TAB -> +1
        }
    }
    fprintf(F,"\nE");
    fclose(F);
    printf("\n <- Ecriture fichier SYLK reussit");
    printf("\n============================================================");

    return 0;
}

/** \brief Crée un tableau SYLK_TAB à partir d'un fichier slk (SYLK)
 *
 * \param fichier const char* : Le nom du fichier à lire (dois comporter l'extension)
 * \param DIMENTIONTABX size_t : dimention x du tableau à cree. Peux etre initialise a UNDEFINED
 * \param DIMENTIONTABY size_t : m
 * \return SYLK_TAB*
 *
 */
SYLK_TAB* SYLK_ReadFile(const char* fichier,size_t DIMENTIONTABX, size_t DIMENTIONTABY) {
                                            //PErmet de forcer une dimentions au tableau si on veux plus de case ! (
    printf("\n============================================================");
    printf("\nLecture fichier SYLK ->");
    printf("\nFile : %s",fichier);
    FILE* F = fopen(fichier,"r");
    MYASSERT(F,"Impossible d'ouvrir le fichier %s",fichier);
    size_t x,y;
    char str[SIZEMESSAGE];
    int identificateur=0;

    //Lecture Header
    fscanf(F,"ID;%s\n",str);
    printf("\nID : %s",str);
    /// Determination Dimention tableau
    do {
        identificateur = getc(F);//Chaque ligne commence par un indicateur
        if(identificateur=='C') {
            getLineC(F,&x,&y,str);
            //printf("\n INDICATEUR : %C \t Y=%d \t X=%d \t STR=%s \t ",identificateur,x,y,str);

            //On actualise les dimentions
            DIMENTIONTABX = x>DIMENTIONTABX ? x : DIMENTIONTABX;
            DIMENTIONTABY = y>DIMENTIONTABY ? y : DIMENTIONTABY;
            //printf("\nDim : X = %d Y = %d",DIMENTIONTABX,DIME)
        } else if(identificateur=='?') {
            //On ne fais rien
        }else if(identificateur=='E'){//Si l'identificaeur == E on se bloque dans le else
            break;
        }else {
            //On va à la ligne suivante si l'indicateur est inconnu
            do {}while(getc(F)!='\n');
        }
    } while(identificateur !='E');//On lis toutes les lignes

    printf("\nFin de ficheir : %c",identificateur);
    printf("\nSize : X = %d / Y = %d",DIMENTIONTABX,DIMENTIONTABY);

    ///On crée un SYLK_tab de bonne dimention :
    SYLK_TAB* I=SYLK_NewTab(DIMENTIONTABX,DIMENTIONTABY);
    //L'allocation est reussi ou assert

    /// On le remplis
    rewind(F);//On se repositionne au début du fichier
    fscanf(F,"ID;%s\n",str);  //Lecture Header
    //lecture fichier

    ///On enregistre toutes les valeurs
    CELL data;
    do {
        identificateur = getc(F);//Chaque ligne commence par un indicateur
        if(identificateur=='C') {
            getLineC(F,&x,&y,str);
            //printf("\n INDICATEUR : %C \t Y=%d \t X=%d \t STR=%s \t ",identificateur,x,y,str);
            x--;
            y--;//A1 = [0][0]
            switch(convertStr(str,&data)) {
            case ENTIER:
                SYLK_SetEntier(I,x,y,data.entier);
                break;
            case MESSAGE:
                str[strlen(str)-1]='\0';//Cette opération supprime la deuxieme "
                SYLK_SetMessage(I,x,y,str+1);//Le +1 supprime la premiere "
                break;
            case FLOTANT:
                SYLK_SetFlottant(I,x,y,data.flottant);
                break;
            case VOID:
                //Do nothing
                break;
            }

        } else if(identificateur=='?') {
            //On ne fais rien
        }else if(identificateur=='E'){//Si l'identificaeur == E on se bloque dans le else
            break;
        } else {
            //On va à la ligne suivante si l'indicateur est inconnu
            do {} while(getc(F)!='\n');
        }
    } while(identificateur !='E');//On lis toutes les lignes
    fclose(F);
    //if(identificateur=='E')printf("\n-> Success"); else printf("\n-> Error");
    printf("\n <- Lecture fichier SYLK reussit");
    printf("\n============================================================");

    return I;

}

/** \brief retourne taille x SYLK_TAB
 *
 * \param I SYLK_TAB*
 * \return size_t x
 *
 */
size_t SYLK_GetSizeX(SYLK_TAB* I){
    return I->x;
}

/** \brief retourne taille y SYLK_TAB
 *
 * \param I SYLK_TAB*
 * \return size_t y
 *
 */
size_t SYLK_GetSizeY(SYLK_TAB* I){
    return I->y;
}
