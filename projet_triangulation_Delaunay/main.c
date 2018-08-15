#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define pi 3.1415                   ///trigonometrie
#include <string.h>
#include "graphics\graphics.h"
#include "graphics\couleur.h"

#define wait_poll_escape() SDL_Event escape;SDL_PollEvent(&escape);if(escape.type == SDL_QUIT)break;


///Fenetre
#define xs 900
//XScreen
#define ys 600
//Yscreen
///==================================================================
///========================== ALEATOIRE =============================
///==================================================================
///Retourne un flotant aléatoire entre a et b
double frand(double a,double b){
    return ((rand()/(double)RAND_MAX)*(b-a)+a);
}

///Retourne un entier entre a -50 et 50 (point dans un écran)
int randd(int a){
    //return rand() % (a-100)+50;
    return rand() % (a-100)+50;
}


///==================================================================
///=========================== VECTEURS =============================
///==================================================================

typedef struct{ ///vitesse ou position dans l'espace 3D
    float x,y,z;
}VECTEUR;

VECTEUR init_vecteur(float x,float y,float z){
    VECTEUR V;
    V.x = x;
    V.y = y;
    V.z = z;
    return V;
}


char vecteur_egal(VECTEUR A,VECTEUR B){
    if (A.x==B.x && A.y==B.y && A.z==B.z)return 1;
    return 0;
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

///vecteur initialisé à 0!
VECTEUR vecteur_null(){
    VECTEUR V = {0,0,0};
    return V;
}

///Passage de Point à vecteur (utilise pour wait_click... draw_...) SDL
VECTEUR vfp(POINT p){
    VECTEUR a;
    a.x = p.x;
    a.y = p.y;
    a.z = 0;
    return a;
}

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

VECTEUR produit_vectorielle_vecteur(VECTEUR A,VECTEUR B){
    VECTEUR V;
    V.x = A.y*B.z-A.z*B.y;
    V.y = A.z*B.x-A.x*B.z;
    V.z = A.x*B.y-A.y*B.x;
    return V;
}

///Retourne l'angle E[0;pi] entre les vecteur A et B
float angle_vecteur(VECTEUR A,VECTEUR B){///A et B deux vecteur dirrecteur
    ///Suppression div 0
    if(norme_vecteur(A)*norme_vecteur(B)==0)return 0;
    ///Bien ?? -> (=0) ?
    if(A.x==B.x && A.y==B.y && A.z==B.z)return 0;
    return (float)acos((float)(produit_scalaire_vecteur(A,B)/(norme_vecteur(A)*norme_vecteur(B))));
}
///Retourne l'angle E[0;2!!!!pi] entre les vecteur A et B
///USELESS
float angle_vecteur_V2(VECTEUR A,VECTEUR B){///A et B deux vecteur dirrecteur
    ///Suppression div 0
    if(norme_vecteur(A)*norme_vecteur(B)==0)return 0;
    ///Bien ?? -> (=0) ?
    if(A.x==B.x && A.y==B.y && A.z==B.z)return 0;
    float angle = acos((produit_scalaire_vecteur(A,B)/(norme_vecteur(A)*norme_vecteur(B))));
    float signe = asin(norme_vecteur(produit_vectorielle_vecteur(B,A))/(norme_vecteur(A)*norme_vecteur(B)));
    //if (signe>0)return signe*angle;
    //else exit(55);
    float det = norme_vecteur(produit_vectorielle_vecteur(A,B));

    return angle+pi;
    if (det<0)return angle;
    else return -angle+2*pi;
    if(signe<0)return 99;
    else return pi+angle;
}

///Retourne le vecteur dirrecteur de la droite passant par les point P1 -> P2 :
VECTEUR vecteur_dirrecteur(VECTEUR P1,VECTEUR P2){//
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
*   A<--------------B
*                PHI \
*                     \
*                      \>
*                       C
*    Enjoy my ascii ART :)
*/
///Retoune phi
float angle_droite(VECTEUR PA,VECTEUR PB,VECTEUR PC){
    return (angle_vecteur(vecteur_dirrecteur(PB,PA),vecteur_dirrecteur(PB,PC)));///Correction inversion sens
}

//USELESS
float angle_droite_V2(VECTEUR PA,VECTEUR PB,VECTEUR PC){
    return (angle_vecteur_V2(vecteur_dirrecteur(PB,PA),vecteur_dirrecteur(PB,PC)));
}

VECTEUR init_vecteur_mode(char mode){
    VECTEUR p;

    #define dec 100
    //le décallage apr raport a la fenetre
    ///Note : les tr 1 2 sont decallé de 50
    int ls = xs-2*dec;
    int hs = ys-2*dec;
    float alea;

    switch(mode){
        case 0:
            p = vfp(wait_clic());
        break;
        case 1:
            alea = (float)frand(0,100);
            p.x = sqrt(ls*ls-(((alea-50)/50*ls)*((alea-50)/50*ls)))/2;
            if(alea>50)p.x = ls-p.x;
            p.x +=dec;
            alea = frand(0,100);
            p.y = sqrt(hs*hs-(((alea-50)/50*hs)*((alea-50)/50*hs)))/2;
            if(alea>50)p.y = hs-p.y;
            p.y +=dec;
        break;
        case 2:
            p.x = (log((float)frand(1,100))/log(100))*(xs-2*dec)+dec;
            p.y = rand()%(ys-2*dec)+dec;
        break;
        default:
            p.x = rand()%(xs-2*dec)+dec;
            p.y = rand()%(ys-2*dec)+dec;
        break;
    }
    return p;
}

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
///===================  CERCLE CIRCONSCRIT  =========================
///==================================================================

/**
*   STRUCTURE CERCLE CIRCONSCRIT
*   centre -> VECTEUR centre du cerche
*   rayon -> float rayon du cercle
*/
typedef struct{///CERCLE_CIRCONSCRIT
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
typedef struct{///TRIANGLE
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
    tr.etat = 1;///On occupe le triangle
    return tr;
}


///Libère un triangle !
TRIANGLE free_triangle(void){
    TRIANGLE tr;
    tr.A = vecteur_null();///WTF !!!! vecteuur null = vecteur !
    tr.B = vecteur_null();
    tr.C = vecteur_null();
    tr.etat = 0;///On libere le triangle !
    return tr;
}

///Affiche un triangle SDL
void draw_triangle_vecteur(TRIANGLE tr,COULEUR color){
    if(tr.etat)draw_triangle(pfv(tr.A),pfv(tr.B),pfv(tr.C),color);
}
/// !!!!! NE TIENT PAS COMPTE DE Z !!!

///Affiche le cercle cercle circonscrit du triangle SDL

CERCLE_CIRCONSCRIT calcul_cercle_circonscrit_tr(TRIANGLE tr){
     return calcul_cercle_circonscrit_V2(tr.A,tr.B,tr.C);
}

void draw_centre_triangle(TRIANGLE tr,int size,COULEUR c){
    CERCLE_CIRCONSCRIT cercle = calcul_cercle_circonscrit_tr(tr);
    draw_fill_circle(pfv(cercle.centre),size,c);
}

void draw_cercle_triangle(TRIANGLE tr,COULEUR c){
    CERCLE_CIRCONSCRIT cercle = calcul_cercle_circonscrit_tr(tr);
    draw_circle(pfv(cercle.centre),cercle.rayon,c);
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
void draw_all_triangle(TRIANGLE *tr,int nbtr,char affiche_cercle,COULEUR color){
    int i;
    //fill_screen(black);
    for (i=0;i<nbtr;i++){
        if(tr[i].etat){///Si le tr est op
            draw_triangle_vecteur(tr[i],color);
            if(affiche_cercle)draw_cercle_triangle(tr[i],lightsteelblue);
        }
    }
}

///Initialialise tous les triangles comme free
void free_all_triangle(TRIANGLE *tr,int nbtr){//free_all_triangle
    int i;
    for (i=0;i<nbtr;i++){
        tr[i]=free_triangle();
    }
}

///Initialise les deux premeir triangle
void init_tr_0_1(TRIANGLE *tr){
     float d = 50;
    //Tr 0 init;
    VECTEUR A={d,d,0};
    VECTEUR B={d,ys-d,0};
    VECTEUR C={xs-d,d,0};
    tr[0]=new_triangle(A,B,C);

    //Tr 1 init :
    A.x = xs-d;
    A.y = ys-d;
    tr[1]=new_triangle(A,B,C);
}

///Condition d'initialisation d'un TR
char vecteur_existant_dans_tr(VECTEUR v,TRIANGLE * tr,int nbtr){
    int i;
    for(i=0;i<nbtr;i++){
        if (vecteur_egal(v,tr[i].A))return 1;
        if (vecteur_egal(v,tr[i].B))return 1;
        if (vecteur_egal(v,tr[i].C))return 1;
        ///Retourne 1 si le point est déja existant tr par tr
    }
    ///Dans le cas ou le nouveau point est libre, on retourne 0
    return 0;
}


///==================================================================
///=================== DETECTION DE l'ENVELOPPE =====================
///==================================================================
///enveloppe FINALEMENT NON connexe ALGO OPERATINEL

typedef struct{
    VECTEUR P1,P2;
}SEGMENT;

SEGMENT init_segment(VECTEUR P1,VECTEUR P2){
    SEGMENT s;
    s.P1 = P1;
    s.P2 = P2;
    return s;
}

char segment_egal(SEGMENT s,SEGMENT t){
    //Les points nont pas d'ordre
    if((vecteur_egal(s.P1,t.P1) && vecteur_egal(s.P2,t.P2))||(vecteur_egal(s.P1,t.P2) && vecteur_egal(s.P2,t.P1)))return 1;return 0;
}

void extract_segment_of_tr(TRIANGLE *tr_in_colision,int nb_tr_in_colision,SEGMENT *tab_seg,int *nb_tab_seg){
    int i;
    int nb_seg=0; //=nb_seg
    for(i=0;i<nb_tr_in_colision;i++){
        tab_seg[nb_seg+0]=init_segment(tr_in_colision[i].A,tr_in_colision[i].B);
        tab_seg[nb_seg+1]=init_segment(tr_in_colision[i].B,tr_in_colision[i].C);
        tab_seg[nb_seg+2]=init_segment(tr_in_colision[i].C,tr_in_colision[i].A);
        nb_seg+=3;
    }
    if(nb_seg!=nb_tr_in_colision*3)exit(0x3ABCDEF);
    *nb_tab_seg=nb_seg;
}

void supprimer_segment_tab(SEGMENT *tab,int *size,int pos){
    int i;
    for(i=pos;i<=*size-1;i++){
        tab[i]=tab[i+1];
    }
    ///On met à une valeur d'initialisation la case vide crée;
    tab[*size-1]=init_segment(vecteur_null(),vecteur_null());
    *size-=1;
}

void supprimer_tous_segment_en_double(SEGMENT *tab_seg,int *nb_tab_seg){
    int i,j;
    for(j=*nb_tab_seg-1;j>=0;j--){
        for(i=j-1;i>=0;i--){
            if(segment_egal(tab_seg[j],tab_seg[i])){
                    supprimer_segment_tab(tab_seg,nb_tab_seg,j);
                    supprimer_segment_tab(tab_seg,nb_tab_seg,i);
                //j>i DANS tous les CAS !
                //*nb_tab_seg-=2;//Louche ->//Correction louche : le *size-- NE MARCHE PAS !!
            }
        }
    }
}

void draw_all_line(SEGMENT *tab_seg,int nb_tab_seg){
    int i;
    for(i=0;i<nb_tab_seg;i++){
        draw_line(pfv(tab_seg[i%nb_tab_seg].P1),pfv(tab_seg[i%nb_tab_seg].P2),/*couleur_style(lunary,i,nb_tab_seg)*/0xff0000);
        //draw_line(pfv(init_vecteur(tab_seg[i%nb_tab_seg].P1.x+1,tab_seg[i%nb_tab_seg].P1.y+1,tab_seg[i%nb_tab_seg].P1.z)),pfv(init_vecteur(tab_seg[i%nb_tab_seg].P2.x+1,tab_seg[i%nb_tab_seg].P2.y+1,tab_seg[i%nb_tab_seg].P2.z)),couleur_style(lunary,i,nb_tab_seg)/*lightcyan*/);
    }
}


///==================================================================
///=================== Diagramme_de_Voronoï =====================
///==================================================================

void draw_all_centre_tr(TRIANGLE *tr,int nbtr,int size,COULEUR color){
    int i;
    //fill_screen(black);
    for (i=0;i<nbtr;i++){
        if(tr[i].etat){///Si le tr est op
           draw_centre_triangle(tr[i],size,color);
        }
    }
}

void extract_segment_voronoi_of_delaunay(TRIANGLE *tr,int NBTR,SEGMENT *tab_seg,int *nb_tab_seg){

    SEGMENT AB,BC,CA,XY,YZ,ZX;
    CERCLE_CIRCONSCRIT c_voro1,c_voro2;
    int i,j;
    int numero_tr_libre = numero_triangle_libre(tr,NBTR);
    *nb_tab_seg=0;
    for(i=0;i<numero_tr_libre;i++){
        for(j=i;j<numero_tr_libre;j++){
            AB = init_segment(tr[i].A,tr[i].B);
            BC = init_segment(tr[i].B,tr[i].C);
            CA = init_segment(tr[i].C,tr[i].A);
            XY = init_segment(tr[j].A,tr[j].B);
            YZ = init_segment(tr[j].B,tr[j].C);
            ZX = init_segment(tr[j].C,tr[j].A);
            if(segment_egal(AB,XY)||segment_egal(AB,YZ)||segment_egal(AB,ZX)||segment_egal(BC,XY)||segment_egal(BC,YZ)||segment_egal(BC,ZX)||segment_egal(CA,XY)||segment_egal(CA,YZ)||segment_egal(CA,ZX)){
                c_voro1 = calcul_cercle_circonscrit_tr(tr[i]);
                c_voro2 = calcul_cercle_circonscrit_tr(tr[j]);
                draw_line(pfv(c_voro1.centre),pfv(c_voro2.centre),0xFF0000);
                tab_seg[*nb_tab_seg]=init_segment(c_voro1.centre,c_voro2.centre);
                *nb_tab_seg+=1;
            }
        }
    }
}

int main (int argc, char** argv )
{
    ///============================= CREATION FENETRE GRAPHIQUE =============================
    init_graphics(xs,ys,SDL_TRUE,"STL");
    set_affiche_auto(0);
    fill_screen(0);
    affiche_all();

    ///============================= INITIALISATION TRIANGLES ================================
    ///Initialisation de nos TRIANGLES (vierges)
    #define NBTR 5000
    TRIANGLE tr[NBTR];
    free_all_triangle(tr,NBTR);
    //On créé deux triangle représentatif de l'ecran
    init_tr_0_1(tr);
    draw_all_triangle(tr,NBTR,1,lightgreen);
    affiche_all();
    //Variable donnant le numero d'un tr libre
    int numero_tr_libre;


    int i;

    ///============================== ENVELOPPE : SEGMENTS ====================================
    #define nb_seg_max 100*3
    SEGMENT tab_seg[nb_seg_max];
    int nb_tab_seg; ///Nombre de SEGMENT de tab_seg

    TRIANGLE tr_in_colision[nb_seg_max/3];
    int nb_tr_in_colision = 0;//Nombre de tr en colision == Nb tr de tr_in_colision

    ///============================== voronoi ====================================
    #define nb_seg_voro_max 5000
    SEGMENT tab_seg_voro[nb_seg_voro_max];
    int nb_seg_voro;



    ///DELAUNAy -->

    int t=10000;       // Le nombre d'itérations
    VECTEUR p;         //Le POINT que l'on ajoute a chaque itération
    #define PAS 0
    //Le quadrillage de nos points (la différence de distance entre chauque points sur 1D mettre à 0 pour ne pas en tenir compte
    #define MODE 0
    /**Le mode de génération des points :
    *   Manuel X/Y              0
    *   Etoilé circulaire X/Y   1
    *   Logarithmique X         2
    *   Random                  Default
    */


    #define evolution 0
    #define draw_cercle 0

    ///Couleurs :
    #define BACKGROUND 0x000000
    #define C_TR_GOOD lightgreen
    #define C_TR_DELETE 0x22BABA
    #define C_CC_DELETE lightsalmon

    while(t-->0){


        ///Initlaisation d'un point convenable (respectant ses contraintes et loies)
        do{
            p=init_vecteur_mode(MODE);
            #if PAS
            p=init_vecteur((int)p.x/PAS*PAS,(int)p.y/PAS*PAS,0);
            #endif // PAS
        }while(vecteur_existant_dans_tr(p,tr,NBTR));

        //draw_fill_circle(pfv(p),5,0x22BABA);

        ///Dans tous les tr, on récupere le nombre et l'adresses des tr ou il y a collision a leur cercle circonscrit respectif
        nb_tr_in_colision = 0;
        for(i=0;i<NBTR;i++){
            if(tr[i].etat){///On reste dans le monde des vivants, c' est deja bien :p
                if (collision_vecteur_dans_cercle_circonscrit_a_3_points(p,tr[i].A,tr[i].B,tr[i].C)){
                    tr_in_colision[nb_tr_in_colision]=tr[i];//on enregistre le triangle
                    nb_tr_in_colision++;//On implémentre le nombre de tr en colision
                    #if evolution //Affichage des tr qui depop :
                            draw_triangle_vecteur(tr[i],C_TR_DELETE);
                            if(1)draw_cercle_triangle(tr[i],C_CC_DELETE);
                    #endif // evolution
                    ///Puis on le libere
                    tr[i]=free_triangle();//On met juste son etet a 0.
                }
            }
        }
        #if evolution//Affichage tr del
            affiche_all();
            wait_clic();
            fill_screen(black);
            draw_all_triangle(tr,NBTR,draw_cercle,C_TR_GOOD);
            affiche_all();
            wait_clic();
        #endif // evolution

        ///Calcul enveloppe des segmens
        extract_segment_of_tr(tr_in_colision,nb_tr_in_colision,tab_seg,&nb_tab_seg);
        supprimer_tous_segment_en_double(tab_seg,&nb_tab_seg);
        #if evolution
            draw_all_line(tab_seg,nb_tab_seg);
            affiche_all();
            wait_clic();
        #endif // evolution

        ///On cree de nouveau tr :
        for(i=0;i<nb_tab_seg;i++){

            numero_tr_libre = numero_triangle_libre(tr,NBTR);
            tr[numero_tr_libre]=new_triangle(tab_seg[i].P1,tab_seg[i].P2,p);
            #if evolution
            draw_triangle_vecteur(tr[numero_tr_libre],couleur_style(lunary,i,nb_tab_seg));
            if(1)draw_cercle_triangle(tr[numero_tr_libre],lightsteelblue);
            #endif // evolution

        }
        #if evolution //On affiche les tr Crées
        affiche_all();
        wait_clic();
        #endif // evolution


         ///Voro
        extract_segment_voronoi_of_delaunay(tr,NBTR,tab_seg_voro,&nb_seg_voro);



        ///On affiche le resultat final
        fill_screen(BACKGROUND);
        draw_all_triangle(tr,NBTR,draw_cercle,C_TR_GOOD);
        draw_all_centre_tr(tr,NBTR,2,0xFF0000);
        draw_all_line(tab_seg_voro,nb_seg_voro);



         //+barre chargement;
       // draw_fill_rectangle(pfv(init_vecteur(10,ys-10,0)),pfv(init_vecteur((xs-20)*(1-(float)t/10000)+10,ys-40,0)),C_TR_GOOD);

        affiche_all();
        wait_poll_escape()

    }

    ///Fin delaunay
    fill_screen(black);
    draw_all_triangle(tr,NBTR,draw_cercle,C_TR_GOOD);
    affiche_all();
    wait_escape();
    return 0;
}
