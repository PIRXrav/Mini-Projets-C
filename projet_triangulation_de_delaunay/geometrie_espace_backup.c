#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define pi 3.1415                   ///trigonometrie
#include <string.h>
#include "graphics\graphics.h"
#include "graphics\couleur.h"

#define wait_poll_escape() SDL_Event escape;SDL_PollEvent(&escape);if(escape.type == SDL_QUIT)break;




typedef struct{ ///vitesse ou position dans l'espace 3D
    float x,y,z;
}VECTEUR;

///==================================================================
///============================== STL ===============================
///==================================================================
#define name_file "bunny"
#define flux_in_file 0
///Structure de donné triangle pour un fichier STL
///La dirrection normale est inutile dans ce projet
typedef struct{///TRIANGLE_IN_FILE
    VECTEUR direction_normale,a,b,c;
    //unsigned char octet_de_controle;
} TRIANGLE_IN_FILE;

///Convertis un vecteur en point (int) R2
POINT pfv(VECTEUR a){///point from vecteur
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

///Useless
typedef struct{
    POINT pA,pB,pC;
}TRIANGLE_2D;

///==================================================================
///============== CALCUL D'UN CERCLE CIRCONSCRIT  ===================
///==================================================================

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
CERCLE_CIRCONSCRIT calcul_cercle_circonscrit_V1(VECTEUR A, VECTEUR B, VECTEUR C){
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
    ///cf lien
}

/**
*   OPERATIONLLE DANS R3
*   RETOURNE LA DISTANCE ENTRE 2 POINTS
*/
float distance_vecteur(VECTEUR PA,VECTEUR PB){//Pytagore 3D
    return sqrt(pow(PA.x-PB.x,2)+pow(PA.y-PB.y,2)+pow(PA.z-PB.z,2));
}

/**
*   Permet l'affichage d'un vecteur (point, vitesse ...)
*   argument : VECTEUR
*/
void print_vecteur(VECTEUR A){
    printf("\n|X = %f",A.x);
    printf("\n|Y = %f",A.y);
    printf("\n|Z = %f",A.z);
}
/**
*   Retourne le déterminant d'une matrice 3 3
*   https://fr.wikipedia.org/wiki/Calcul_du_d%C3%A9terminant_d%27une_matrice
*/
float determinant(float a,float b,float c,float d ,float e,float f,float g,float h,float i){
    return (a*e*i+d*h*c+g*b*f)-(g*e*c+a*h*f+d*b*i);
}
/**
*   OPERATIONLLE DANS R3 !
*   RETOURNE UN CELCLE CIRCONSCRIT A PARTIR DE 3 VECTEUR (points) : P1,P2,P3
*   Inspiré de http://math.15873.pagesperso-orange.fr/Cercl3p.html
*/
CERCLE_CIRCONSCRIT calcul_cercle_circonscrit_V2(VECTEUR P1, VECTEUR P2, VECTEUR P3){
    ///=================== POINT MILIEU ==================================
    ///Soit M12 le milieu du segment formé par P1 et P2
    VECTEUR M12;
    M12.x = (P1.x+P2.x)/2;
    M12.y = (P1.y+P2.y)/2;
    M12.z = (P1.z+P2.z)/2;
    ///Soit M23le milieu du segment formé par P2 et P3
    VECTEUR M23;
    M23.x = (P2.x+P3.x)/2;
    M23.y = (P2.y+P3.y)/2;
    M23.z = (P2.z+P3.z)/2;
    ///=================== VECTEUR DIRRECTEUR =============================
    ///Soit V1 le vecteur directeur de la droite P1 P2
    VECTEUR V1;
    V1.x = P1.x - P2.x;
    V1.y = P1.y - P2.y;
    V1.z = P1.z - P2.z;
    //print_vecteur(V1);
     ///Soit V2 le vecteur directeur de la droite P2 P3
    VECTEUR V2;
    V2.x = P2.x - P3.x;
    V2.y = P2.y - P3.y;
    V2.z = P2.z - P3.z;
    //print_vecteur(V2);
    ///====================================================================
    ///============ Nous allons définir 3 PLANS : =========================
    ///Le plan 1 est défini par les 3 points P1,P2,P3
    ///matrice plan 1
    float mp1[3][3] = {{-P1.x,-P1.y,-P1.z},{V1.x,V1.y,V1.z},{V2.x,V2.y,V2.z}};
    ///Equation plan 1 : ax+by+cz+d = 0 -> résolution BARBARE
    float p1a = mp1[1][1]*mp1[2][2] - mp1[2][1]*mp1[1][2];
    float p1b = mp1[2][0]*mp1[1][2] - mp1[2][2]*mp1[1][0];
    float p1c = mp1[1][0]*mp1[2][1] - mp1[2][0]*mp1[1][1];
    float p1d = mp1[0][0]*p1a+mp1[0][1]*p1b+mp1[0][2]*p1c;//Merci
    //printf("\n %.1fx +%.1fy +%.1fz + %.1f = 0",p1a,p1b,p1c,p1d);
    ///====================================================================
    ///Le plan 2 est celui qui passe par le point P12 et qui est perpendiculaire au vecteur V1
    float p2a = V1.x;
    float p2b = V1.y;
    float p2c = V1.z;
    float p2d = V1.x * -M12.x + V1.y * -M12.y + V1.z * -M12.z;
    //printf("\n %.1fx +%.1fy +%.1fz + %.1f = 0",p2a,p2b,p2c,p2d);
    ///====================================================================
    ///Le plan 3 est celui qui passe par le point P23 et qui est perpendiculaire au vecteur V2
    float p3a = V2.x;
    float p3b = V2.y;
    float p3c = V2.z;
    float p3d = V2.x * -M23.x + V2.y * -M23.y + V2.z * -M23.z;
    //printf("\n %.1fx +%.1fy +%.1fz + %.1f = 0",p3a,p3b,p3c,p3d);
    ///====================================================================
    ///Le centre va etre l'intersection de ces 3 plans
    /// |p1a x + p1b y + p1c z = - p1d
    /// |p2a x + p2b y + p2c z = - p2d
    /// |p3a x + p3b y + p3c z = - p3d
    /// Déterminons x ,y et z
    ///-> determinant de matrice 3 3
    float delta = determinant(p1a,p1b,p1c,p2a,p2b,p2c,p3a,p3b,p3c);
    //printf("\ndelta = %f",delta);
    float deltax = determinant(-p1d,p1b,p1c,-p2d,p2b,p2c,-p3d,p3b,p3c);
    //printf("\ndeltax = %f",deltax);
    float deltay = determinant(p1a,-p1d,p1c,p2a,-p2d,p2c,p3a,-p3d,p3c);
    //printf("\ndeltay = %f",deltay);
    float deltaz = determinant(p1a,p1b,-p1d,p2a,p2b,-p2d,p3a,p3b,-p3d);
    //printf("\ndeltaz = %f",deltaz);
    ///Soit le centre :
    CERCLE_CIRCONSCRIT cercle;
    cercle.centre.x = deltax/delta;
    cercle.centre.y = deltay/delta;
    cercle.centre.z = deltaz/delta;
    ///On en déduit le rayon
    cercle.rayon = distance_vecteur(cercle.centre,P1);
    ///On retoune notre cercle circonscrit au trois points
    return cercle;
}

/**
*   Permet de savoir si un point 3D (VECTEUR) est dans une sphere
*   P -> le point de test
*   cercle -> la sphere = le cercle
*   retourne 1 en cas de colision 0 sinon
*/
char collision_vecteur_dans_cercle(VECTEUR P,CERCLE_CIRCONSCRIT cercle){
   return cercle.rayon > distance_vecteur(P,cercle.centre)? 1 : 0;
}

/**
*   Permet de savoir si un point 3D (VECTEUR) est dans une sphere inscrite a trois points
*   P -> le point de test
*   A,B,C -> les trois points du cercle inscrit
*   retourne 1 en cas de colision 0 sinon
*/
char collision_vecteur_dans_cercle_circonscrit_a_3_points(VECTEUR P,VECTEUR A,VECTEUR B,VECTEUR C){
    return collision_vecteur_dans_cercle(P,calcul_cercle_circonscrit_V2(A,B,C));
}

///==================================================================
///============== GESTION INDIVIDUELLE D'UN TRIANGLE ================
///==================================================================
///Structure TRIANGLE
typedef struct{
    VECTEUR A,B,C;
    ///3 Points dans l'espace
    char etat;
    ///Etat du triangle :
    /// 1 -> Occupé
    /// 2 -> Libre
}TRIANGLE;

///REtourne un triangie à partir de 3 points (VECTEURS)
TRIANGLE new_triangle(VECTEUR A,VECTEUR B,VECTEUR C){
    TRIANGLE tr;
    tr.A = A;
    tr.B = B;
    tr.C = C;
    tr.etat = 1;
    return tr;
}

///vecteur initialisé à 0!
VECTEUR vecteur_null(){
    VECTEUR V = {0,0,0};
    return V;
}

///Libère un triangle !
TRIANGLE free_triangle(void){
    TRIANGLE tr;
    tr.A = vecteur_null();
    tr.B = vecteur_null();
    tr.C = vecteur_null();
    tr.etat = 0;
    return tr;
}

///Affiche un triangle SDL
void draw_triangle_vecteur(TRIANGLE tr){
    draw_triangle(pfv(tr.A),pfv(tr.B),pfv(tr.C),lightgreen);
}
///Affiche le cercle cercle circonscrit du triangle SDL
void draw_cercle_triangle(TRIANGLE tr){
    CERCLE_CIRCONSCRIT cercle = calcul_cercle_circonscrit_V2(tr.A,tr.B,tr.C);
    draw_circle(pfv(cercle.centre),cercle.rayon,lightsteelblue);
}
///Passage de Point à vecteur (utilise pour wait_click) SDL
VECTEUR vfp(POINT p){
    VECTEUR a;
    a.x = p.x;
    a.y = p.y;
    a.z = 0;
    return a;
}
///==================================================================
///============ GESTION DE L'ENSEMBLE DE NOS TRIANGLES ==============
///==================================================================
///Retourne le numéro d'un triangle non occupé
int numero_triangle_libre(TRIANGLE * tr,int nbtr){
    int i;
    for(i=0;i<nbtr;i++){
        if (tr[i].etat == 0)return i;
        ///Retourne le numéro d'un triangle disponible
    }
    ///Dans le cas ou il n'y à plus de triangle disponible : (error)
    exit(i);
}

///Dessine tous les triangles
void draw_all_triangle(TRIANGLE *tr,int nbtr){
    int i;
    fill_screen(black);
    for (i=0;i<nbtr;i++){
        if(tr[i].etat){///Si le tr est op
            draw_triangle_vecteur(tr[i]);
            draw_cercle_triangle(tr[i]);
        }
    }
}

///Initialialise tous les triangles comme free
void init_all_triangle(TRIANGLE *tr,int nbtr){//free_all_triangle
    int i;
    for (i=0;i<nbtr;i++){
        tr[i].etat=0;
    }
}
///==================================================================
///====== Calul de l'envelope convexe -> Marche de jarvis ===========
///==================================================================
///https://fr.wikipedia.org/wiki/Marche_de_Jarvis

///Retourne la norme d'un vecteur S = |V|
float norme_vecteur(VECTEUR V){
    return sqrt(pow(V.x,2)+pow(V.y,2)+pow(V.z,2));
}

///retourne la somme de deux vecteur S = A+B
VECTEUR somme_vecteur(VECTEUR A,VECTEUR B){
    VECTEUR S;
    S.x = A.x + B.x;
    S.y = A.y + B.y;
    S.z = A.z + B.z;
    return S;
}
///Retourne le produit scalaire S = (A.B)
float produit_scalaire_vecteur(VECTEUR A,VECTEUR B){
    return A.x * B.x + A.y * B.y + A.z * B.z;
}
///Retourne l'angle E[0;2pi] entre les vecteur A et B
float angle_vecteur(VECTEUR A,VECTEUR B){///A et B deux vecteur dirrecteur
    return acos(produit_scalaire_vecteur(A,B)/(norme_vecteur(A)*norme_vecteur(B)));
}

///Retourne le vecteur dirrecteur de la droite passant par les point P1 -> P2 :
VECTEUR vecteur_dirrecteur(VECTEUR P1,VECTEUR P2){
    ///Soit V le vecteur directeur de la droite P1 P2
    VECTEUR V;
    V.x = P2.x - P1.x;
    V.y = P2.y - P1.y;
    V.z = P2.z - P1.z;
    return V;
}
/**
*   Soit PHI l'angle entre la droite AB et la droite BC
*   -> l'angle entre le vecteur BA et le vecteur BC !
*   A---------------B
*                PHI \
*                     \
*                      \
*                       C
*    Enjoy my ascii ART :)
*/
///Retoune phi
float angle_droite(VECTEUR PA,VECTEUR PB,VECTEUR PC){
    return angle_vecteur(vecteur_dirrecteur(PB,PA),vecteur_dirrecteur(PB,PC));
}


int main (int argc, char** argv )
{

    ///============================= CREATION FENETRE GRAPHIQUE =============================
    #define xs 900
    #define ys 600
    init_graphics(xs,ys,SDL_TRUE,"STL");
    set_affiche_auto(SDL_TRUE);
    fill_screen(0);
    affiche_all();


    ///Initialisation de nos TRIANGLES (vierges)
    #define NBTR 5
    TRIANGLE tr[NBTR];
    init_all_triangle(tr,NBTR);


    int i;
    //tr[2]=new_triangle(vecteur_null(),vecteur_null(),vecteur_null());//on met un tr à 0 mais occupé ! -> plus de triangle dispo -> error(i)


    for(i=0;i<NBTR;i++){
        tr[numero_triangle_libre(tr,NBTR)]=new_triangle(vfp(wait_clic()),vfp(wait_clic()),vfp(wait_clic()));
        draw_all_triangle(tr,NBTR);
    }

    ///Danger :


    /*
    float d = 50;
    //Tr 0 init;
    VECTEUR A={d,d,0};
    VECTEUR B={d,ys-d,0};
    VECTEUR C={xs-d,d,0};
    tr[0].A = A;
    tr[0].B = B;
    tr[0].C = C;
    draw_triangle_vecteur(tr[0]);
    draw_cercle_triangle(tr[0]);

    //Tr 1 init :
    A.x = xs-d;
    A.y = ys-d;
    tr[1].A = A;
    tr[1].B = B;
    tr[1].C = C;
    draw_triangle_vecteur(tr[1]);
    draw_cercle_triangle(tr[1]);
    /
  */ /*int i;

     for(i=0;i<NBTR;i++){
            draw_triangle_vecteur(tr[i]);
            draw_cercle_triangle(tr[i]);
        }
        affiche_all();
    VECTEUR p;
    int nb_collision=0;
    int adresse_collision[100]={0};
    VECTEUR point_collision[100*3]={0};
    while(1){
        nb_collision =0;
        p = vfp(wait_clic());
        /// Dans tous les tr, on récupere le nombre et l'adresses des tr ou il y a collision
        for(i=0;i<NBTR;i++){
            if (collision_vecteur_dans_cercle(p,calcul_cercle_circonscrit_V2(tr[i].A,tr[i].B,tr[i].C))){
                adresse_collision[nb_collision]=i;
                nb_collision++;
            }
        }
        ///On extrait les point des tr précédents
        ///Correspond à l'ensembre des points de l'envelope PAS PAS PAS PAS PAS convexe
        for(i=0;i<nb_collision;i++){
            point_collision[i*3+0]=tr[adresse_collision[i]].A;
            point_collision[i*3+1]=tr[adresse_collision[i]].B;
            point_collision[i*3+2]=tr[adresse_collision[i]].C;
            //Raz
            adresse_collision[i]=0;
        }

        fill_screen(black);
        for(i=0;i<NBTR;i++){
            draw_triangle_vecteur(tr[i]);
            draw_cercle_triangle(tr[i]);

        }
        affiche_all();
    }


    CERCLE_CIRCONSCRIT cercle;

    cercle = calcul_cercle_circonscrit_V2(B,A,C);
    print_vecteur(cercle.centre);
    printf("\n rayon = %f",cercle.rayon);

    ///Vecteur pour tester collision
    VECTEUR P={5,5,8};//avec ce point x et y sont dans le cercle circonscrit, mais pas dans lasphere -> non colision !
    printf("\nCollision : %d",collision_vecteur_dans_cercle_circonscrit_a_3_points(P,A,B,C));
*/
    affiche_all();
    wait_escape();
    return 0;
}
