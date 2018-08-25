/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/SYLK/SYLK.h"

int main(void) {

    //On cree un nouveau tableau I de taille 5*5
    SYLK_TAB * I = SYLK_NewTab(5,5);
    //initialisation :
    SYLK_SetMessage(I,1,2,"test");
    size_t i,j;
    for(j=0; j<SYLK_GetSizeY(I); j++) {
        for(i=0; i<SYLK_GetSizeX(I); i++) {
            SYLK_SetEntier(I,i,j,i+j);
        }
    }
    SYLK_SetMessage(I,0,0,"MESSAGE");
    SYLK_SetMessage(I,1,1,"test");
    SYLK_SetVoid(I,2,2);
    SYLK_SetFlottant(I,3,3,(double)1/3*1E50);
    SYLK_SetFlottant(I,4,4,(double)6.6*1E50);
    //Affichage
    SYLK_PrintTab(I);
    //Creation du fichier SYLK
    SYLK_CreateFile(I,"namefile.slk","PIERRE");
    //On supprime le tableau
    SYLK_FreeTab(&I);

    //On cree un nouveau tableu READ_I de taille UNDEFINED * 6
    //La taille UNDEFINED s'adaptera automatiquement au fichier
    //De meme si 6 est trop petit
    //Le fichier est alors lus
    SYLK_TAB * READ_I =SYLK_ReadFile("namefile.slk",UNDEFINED,6);
    //On affiche le nouveau tableau READ_I
    SYLK_PrintTab(READ_I);
    //On supprime le tableau
    SYLK_FreeTab(&READ_I);

    return 0;
}

/* Output :

 Allocation ->
Position I :    00740CB0
Position data : 00740CC8
Position etat : 00740D98
 <- Allocation Reussit

TAB     |x0     |x1     |x2     |x3     |x4
--------|-------|-------|-------|-------|-------
 y0     |MESSAG |1      |2      |3      |4
--------|-------|-------|-------|-------|-------
 y1     |1      |test   |3      |4      |5
--------|-------|-------|-------|-------|-------
 y2     |2      |3      |VOID   |5      |6
--------|-------|-------|-------|-------|-------
 y3     |3      |4      |5      |3e+049 |7
--------|-------|-------|-------|-------|-------
 y4     |4      |5      |6      |7      |7e+050
--------|-------|-------|-------|-------|-------
============================================================
 Ecriture fichier SYLK ->
File : namefile.slk
ID : PIERRE
Size : X = 5 et Y = 5
 <- Ecriture fichier SYLK reussit
============================================================
 Free ->
Position data : 00000000
Position etat : 00000000
Position I :    00000000
 <- Free reussit
============================================================
Lecture fichier SYLK ->
File : namefile.slk
ID : PIERRE
Fin de ficheir : E
Size : X = 5 / Y = 6
 Allocation ->
Position I :    00740CB0
Position data : 00740CC8
Position etat : 00740E48
 <- Allocation Reussit
 <- Lecture fichier SYLK reussit
============================================================

TAB     |x0     |x1     |x2     |x3     |x4
--------|-------|-------|-------|-------|-------
 y0     |MESSAG |1      |2      |3      |4
--------|-------|-------|-------|-------|-------
 y1     |1      |test   |3      |4      |5
--------|-------|-------|-------|-------|-------
 y2     |2      |3      |VOID   |5      |6
--------|-------|-------|-------|-------|-------
 y3     |3      |4      |5      |3e+049 |7
--------|-------|-------|-------|-------|-------
 y4     |4      |5      |6      |7      |7e+050
--------|-------|-------|-------|-------|-------
 y5     |VOID   |VOID   |VOID   |VOID   |VOID
--------|-------|-------|-------|-------|-------
 Free ->
Position data : 00000000
Position etat : 00000000
Position I :    00000000
 <- Free reussit
Process returned 0 (0x0)   execution time : 0.043 s
Press any key to continue.
*/
