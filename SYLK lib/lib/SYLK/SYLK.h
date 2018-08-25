/* SYLK.h */

#ifndef SYLK_H_INCLUDED
#define SYLK_H_INCLUDED

#define UNDEFINED 0

/* structures ========================================================== */
//Structure opaque
typedef struct SYLK_TAB SYLK_TAB;

/* functions publiques ================================================= */
//Acces aux dimentions
size_t SYLK_GetSizeX(SYLK_TAB* I);
size_t SYLK_GetSizeY(SYLK_TAB* I);
//Allocation memoire
SYLK_TAB* SYLK_NewTab(size_t x,size_t y);
void SYLK_FreeTab(SYLK_TAB** I);

//Ecriture dans le Tableau
void SYLK_SetMessage(SYLK_TAB* I,size_t x,size_t y,const char * message);
void SYLK_SetFlottant(SYLK_TAB* I,size_t x,size_t y,double flottant);
void SYLK_SetEntier(SYLK_TAB* I,size_t x,size_t y,long int entier);
void SYLK_SetVoid(SYLK_TAB* I,size_t x,size_t y);


//Lecture dans le Tableau
char * SYLK_GetMessage(SYLK_TAB* I,size_t x,size_t y);
double SYLK_GetFlottant(SYLK_TAB* I,size_t x,size_t y);
long int SYLK_GetEntier(SYLK_TAB* I,size_t x,size_t y);


//Affichage du tableau dans la console
void SYLK_PrintTab(SYLK_TAB * I);

//lecture & ecriture fichier SYLK

char SYLK_CreateFile(SYLK_TAB* I,const char* fichier,const char * id);
SYLK_TAB* SYLK_ReadFile(const char* fichier,size_t DIMENTIONTABX, size_t DIMENTIONTABY);

#endif // SYLK_H_INCLUDED
