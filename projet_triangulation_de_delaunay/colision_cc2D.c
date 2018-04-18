#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define pi 3.1415                   ///trigonometrie
#include <string.h>
#include "graphics\graphics.h"
#include "graphics\couleur.h"
#define wait_poll_escape() SDL_Event escape;SDL_PollEvent(&escape);if(escape.type == SDL_QUIT)break;

#define name_file "bunny"
#define flux_in_file 0


typedef struct{ ///vitesse ou position dans l'espace 3D
    float x,y,z;
}VECTEUR;

typedef struct
{
    VECTEUR direction_normale,a,b,c;
    //unsigned char octet_de_controle;
} TRIANGLE_IN_FILE;

///Convertis un vecteur en point (int) R2
POINT pfv(VECTEUR a){
    POINT p;
    p.x = a.x;
    p.y = a.y;
    return p;
}
TRIANGLE_IN_FILE * read_STL(const char* fichier,int * nb_triangle)
{
#define nbo_header 80
#define print_tr_read_stl 0
    ///Ouverture fichier read binaire
    FILE* F = fopen(fichier,"rb");
    if (!F){
        puts("ERREUR : probleme de fichier !");
        exit(1);
    }
    ///Lecture header
    unsigned char header[nbo_header];
    fread(&header,1,nbo_header,F);
    printf("%s\n",header);

    ///Lecture du nombre de triangles
    fread(nb_triangle, sizeof(int), 1, F );
    printf("nombre de triangles = %d\n",*nb_triangle);

    ///Allocation dynamique de la mémoire pour les triangles
    TRIANGLE_IN_FILE *tr = calloc(*nb_triangle, sizeof(TRIANGLE_IN_FILE));
    if (!tr){
        puts("ERREUR : probleme de memoire !");
        exit(1);
    }

    ///Lecture des triangles
    int i;
    unsigned char octetCR[2];
    for(i=0; i<*nb_triangle-1; i++)
    {
        fread(tr+i, sizeof(TRIANGLE_IN_FILE), 1, F);
        fread(&octetCR,sizeof(octetCR), 1, F);  //On lit les deux octet de controle
    }
    printf("size : %d",sizeof(*tr));

#if print_tr_read_stl
    ///affichage
    for(i=0; i<*nb_triangle; i++){
        printf("========================================================\n");
        printf("Triangle %d : \n",i+1);
        print_cord_triangle(tr[i]);
    }
#endif // print_tr_read_stl

    ///Fermeture du fichier
    fclose(F);
    ///On retourne tr
    return tr;
    /**
    Note : on ne peux pas faire passer tr en arument... etrange
        Solution -> return tr
    */
}

void print_cord_VECTEUR(VECTEUR aff)
{
    printf("\t[%f:%f:%f]\n",aff.x,aff.y,aff.z);
}
void print_cord_triangle(TRIANGLE_IN_FILE tr)
{
    printf("DN");
    print_cord_VECTEUR(tr.direction_normale);
    printf("A");
    print_cord_VECTEUR(tr.a);
    printf("B");
    print_cord_VECTEUR(tr.b);
    printf("C");
    print_cord_VECTEUR(tr.c);
}

typedef struct{
    POINT pA,pB,pC;
}TRIANGLE_2D;


/**
*   STRUCTURE CERCLE CIRCONSCRIT
*   centre -> VECTEUR centre du cerche
*   rayon -> float rayon du cercle
*/
typedef struct{
    VECTEUR centre;
    float rayon;
}CERCLE_CIRCONSCRIT;

/**
*   NON OPERATIONLLE DANS R3 ! COMPOSANTE Z non pris en compte
*   RETOURNE UN CELCLE CIRCONSCRIT A PARTIR DE 3 VECTEUR (points) : A,B,C
*   https://cral.univ-lyon1.fr/labo/fc/Ateliers_archives/ateliers_2005-06/cercle_3pts.pdf
*/
CERCLE_CIRCONSCRIT calcul_cercle_circonscrit(VECTEUR A, VECTEUR B, VECTEUR C){
    CERCLE_CIRCONSCRIT cercle;
   //centre.x = ((C.x * C.x-B.x*B.x + C.y*C.y - B.y*B.y)/(2(C.y-B.y))-(B.x*B.x-A.x*A.x + B.y*B.y - A.y*A.y)/(2(B.y-A.y)));

    float a,aa,b,bb;

    b = (pow(B.x,2) -pow(A.x,2) + pow(B.y,2) - pow(A.y,2))/(2*(B.y - A.y));
    bb = (pow(C.x,2) -pow(B.x,2) + pow(C.y,2) - pow(B.y,2))/(2*(C.y - B.y));
    a = - (B.x -A.x)/(B.y-A.y);
    aa = - (C.x -B.x)/(C.y-B.y);

    cercle.centre.x = (b-bb)/(aa-a);
    cercle.centre.y = a * cercle.centre.x + b;
    cercle.centre.z = 0;

    cercle.rayon = sqrt(pow(A.x-cercle.centre.x,2)+pow(A.y-cercle.centre.y,2));
/*
    POINT X1,X2;
    X1.x = -1000;
    X1.y = X1.x * a + b;
    X2.x = 1000;
    X2.y = X2.x * a + b;
    draw_line(X1,X2,lightskyblue);
    X1.y = X1.x * aa + bb;
    X2.y = X2.x * aa + bb;
    draw_line(X1,X2,lightskyblue);
    draw_circle(pfv(cercle.centre),cercle.rayon,blueviolet);
    draw_triangle(pfv(A),pfv(B),pfv(C),greenyellow);
    draw_fill_circle(pfv(cercle.centre),5,lightcoral);
*/
    return cercle;

}

/**
*   OPERATIONLLE DANS R3
*   RETOURNE LA DISTANCE ENTRE 2 POINTS
*/
float distance_vecteur(VECTEUR A,VECTEUR B){//Pytagore 3D
    return sqrt(pow(A.x-B.x,2)+pow(A.y-B.y,2)+pow(A.z-B.z,2));
}
int main ( int argc, char** argv )
{
    #define xs 900
    #define ys 600
    ///============================= CREATION FENETRE GRAPHIQUE =============================

    init_graphics(xs,ys,SDL_TRUE,"STL");
    set_affiche_auto(SDL_FALSE);
    fill_screen(0);
    affiche_all();

    VECTEUR A={100,100,0};
    VECTEUR B={200,200,0};
    VECTEUR C={300,150,0};
    VECTEUR ma={1,0,0},mb={-1,-1,0},mc={1,1,0};
     POINT X1,X2;
     CERCLE_CIRCONSCRIT cercle;


    VECTEUR testcolision={300,300,0};
    char colision = 0;
    #define d 100
    while (1){

        A.x += ma.x;
        A.y += ma.y;
        if(A.x < 0 + d || A.x> xs-d) ma.x = -ma.x;
        if(A.y < 0 + d || A.y> ys-d) ma.y = -ma.y;
        B.x += mb.x;
        B.y += mb.y;
        if(B.x < 0 + d || B.x> xs-d) mb.x = -mb.x;
        if(B.y < 0 + d || B.y> ys-d) mb.y = -mb.y;
        C.x += mc.x;
        C.y += mc.y;
        if(C.x < 0 + d || C.x> xs-d) mc.x = -mc.x;
        if(C.y < 0 + d || C.y> ys-d) mc.y = -mc.y;


        cercle = calcul_cercle_circonscrit(A,B,C);

        colision = cercle.rayon < distance_vecteur(testcolision,cercle.centre)? 1 : 0;

        draw_circle(pfv(cercle.centre),cercle.rayon,blueviolet);
        draw_triangle(pfv(A),pfv(B),pfv(C),greenyellow);
        draw_fill_circle(pfv(cercle.centre),5,lightcoral);

        draw_fill_circle(pfv(testcolision),10,darkgoldenrod);



        affiche_all();
        if(colision)fill_screen(black);
        else fill_screen(darkred);
        wait_poll_escape()
    }
    wait_escape();
    return 0;
}
