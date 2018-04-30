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
#define ys 800
//Yscreen

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

///vecteur initialisé à 0!
VECTEUR vecteur_null(){
    VECTEUR V = {0,0,0};
    return V;
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
void draw_all_triangle(TRIANGLE *tr,int nbtr,char affiche_cercle,COULEUR color){
    int i;
    //fill_screen(black);
    for (i=0;i<nbtr;i++){
        if(tr[i].etat){///Si le tr est op
            draw_triangle_vecteur(tr[i],color);
            if(affiche_cercle)draw_cercle_triangle(tr[i]);
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

///==================================================================
///====== Calul de l'envelope convexe -> Marche de jarvis ===========
///==================================================================
///https://fr.wikipedia.org/wiki/Marche_de_Jarvis
///Operationelle dans R2
///Opérationelle dans R3 ? -> non ? AYA YA AYAYAY
///ON NA PAS BESOIN D'ENVELOPPE VONVEXE !!!!!!! enveloppe connexe ha ha ?
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
///USELESS
float angle_droite_V2(VECTEUR PA,VECTEUR PB,VECTEUR PC){
    return (angle_vecteur_V2(vecteur_dirrecteur(PB,PA),vecteur_dirrecteur(PB,PC)));
}

///Déterminse le premier points
int indice_vecteur_xmin(VECTEUR * tb_vec,int nbpoints){
    int i;
    int meilleur_indice = 0;
    for(i=0;i<nbpoints;i++){
        if(tb_vec[i].x<tb_vec[meilleur_indice].x)meilleur_indice = i;
    }
    return meilleur_indice;
}

///Fonction inutile et non testé ^^
///FONCTION DEVENUE GENIALE !!!
char is_in_tab(int * tab,int size,int val){
    //if (val = 0) return 0;
    char to_be_or_not_to_be = 0;
    int i;
    for(i=0;i<size;i++){
        if(tab[i]==val)to_be_or_not_to_be = 1;
    }
    return to_be_or_not_to_be;
}

void calcul_jarvis(VECTEUR * ec,int nb_vect_enveloppe,int evolution,int numero_ec_trie[],int *nb_vect_sur_enveloppee){
    ///           |---------------------INPUT----------------------|--------------------OUTPUT----------------------|
    /**
    *   INPUT
    *      -> ec[nb_vect_enveloppe_max] -> le tableau contenant tous les point à étudié + de l'epace
    *      -> nb_vect_enveloppe -> la taille du tableau précédent à considérer
    *      -> evolution -> 1 = on afficle l'évolution (tuto) / 0 = opérationel !
    *
    *   OUTPUT :
    *      -> numero_ec_trie[] -> ordre des points de ec[] formant l'enveloppe convexe
    *      -> nb_vect_sur_enveloppe -> la taille du tableau précédente à considérer
    *
    *   Cette fonction (massive) retourne l'indice et l'ordre des points de l'enveloppe convexe.
    *   Cette fonction prend e nargument l'ensemble des points de E
    */
    ///nombre de point finis sur l'enveloppe convexe //Cette valeur s'incrémente
    ///à partide 0 à chaque nouveau point detecter.
    int nb_vect_sur_enveloppe = 0;
    int i;
    ///Affichage de tous les cercles dans leur ordre natal = mauvais
    if(evolution){
        for(i=0;i<nb_vect_enveloppe;i++){
                draw_fill_circle(pfv(ec[i%nb_vect_enveloppe]),5,couleur_style(solary,i,nb_vect_enveloppe));
                draw_line(pfv(ec[i]),pfv(ec[(i+1)%nb_vect_enveloppe]),darkorange);
        }
        wait_clic();
    }

    ///=====================================================================
    ///CALCUL DU PREMIR POINT -> POINT 0
    numero_ec_trie[0]=indice_vecteur_xmin(ec,nb_vect_enveloppe);
    if(evolution){
        draw_fill_circle(pfv(ec[numero_ec_trie[0]]),10,lightsteelblue);
    }
    nb_vect_sur_enveloppe++;
    ///=====================================================================

    //wait_clic();

    ///=====================================================================
    ///Determination d'un point plus petit sur y pour pour cree un vecteur
    //point inferieru ay premier pour vecteur !
    VECTEUR p_inf;
    p_inf.x = ec[numero_ec_trie[0]].x;
    p_inf.y = ec[numero_ec_trie[0]].y-10;///-50 a modifié : -> 1 on veux juste l'orientation du vecteur !
    p_inf.z = ec[numero_ec_trie[0]].z;
    if(evolution) {
        draw_fill_circle(pfv(p_inf),10,darkcyan);
    }
    //wait_clic();
    ///Variable pour algo itération
    float angle_max;    ///l'angle max obtenue !
    float angle_i;      ///angle mesure à chaque itération
    int meilleur;       ///Le numéro du meilleur angle !

    ///CALCUL DU DEUXIME POINT -> POINT 1 !
    //1 pour initialise à un angle existant
    meilleur = 99;//cas impossible ! Mathématiquement ~~impossible ! (sous rserve de nbtr)
    angle_max = 0;//Whaou, que c' est grand !
    for(i=0;i<nb_vect_enveloppe;i++){
        /*
        draw_line(pfv(p_inf),pfv(ec[numero_ec_trie[0]]),lightgreen);
        draw_line(pfv(ec[i]),pfv(ec[numero_ec_trie[0]]),lightgreen);
        wait_clic();
        */
        angle_i = angle_droite(p_inf,ec[numero_ec_trie[0]],ec[i]);
        if(angle_i>angle_max){
            angle_max = angle_i;
            meilleur = i;
        }
    }
    numero_ec_trie[1]=meilleur;
    nb_vect_sur_enveloppe++;
    if(evolution){
        draw_fill_circle(pfv(ec[numero_ec_trie[1]]),10,lightsteelblue);
        draw_line(pfv(ec[numero_ec_trie[0]]),pfv(ec[numero_ec_trie[1]]),lightsteelblue);
    }
    //wait_clic();

    ///=====================================================================

    ///CALCUL DU POINT N-> POINT i_point = N-1!
    ///i_point correspond au point i que l'on détermine !
    int i_point;

    ///On calcule les points un à un tant que nous navons pas finis = (tous les tr calc ou cycle recommence)
    for(i_point=2;(i_point<nb_vect_enveloppe)&&!(numero_ec_trie[i_point-1]==numero_ec_trie[0]);i_point++){
        //1 pour initialise à un angle existant
        meilleur = 99;
        angle_max = 0;
        for(i=0;i<nb_vect_enveloppe;i++){
            /*
            draw_line(pfv(ec[numero_ec_trie[i_point-2]]),pfv(ec[numero_ec_trie[i_point-1]]),lightgreen);
            draw_line(pfv(ec[i]),pfv(ec[numero_ec_trie[i_point-1]]),lightgreen);
            wait_clic();
            */
            ///l'angle entre les deux dernier points et un point variable
            angle_i = angle_droite(ec[numero_ec_trie[i_point-2]],ec[numero_ec_trie[i_point-1]],ec[i]);
            if(angle_i>angle_max){
                angle_max = angle_i;
                meilleur = i;
            }
        }
        numero_ec_trie[i_point]=meilleur;
       nb_vect_sur_enveloppe++;

        /////afficchage bordure envelope conver + point (ITERATION)
        if(evolution){
            draw_fill_circle(pfv(ec[numero_ec_trie[i_point]]),10,lightsteelblue);
            draw_line(pfv(ec[numero_ec_trie[i_point-1]]),pfv(ec[numero_ec_trie[i_point]]),lightsteelblue);
        }
    }
    ///On ne compte qu'une fois le point 0 ->
    ///on pouras lire un tableau a tab[nb_vect_sur...] car la valeur y est toujour enregistré sinon tab[(i+1)%nb_vect_sur...]
    nb_vect_sur_enveloppe--;
    if(evolution){
        draw_fill_circle(pfv(ec[numero_ec_trie[nb_vect_sur_enveloppe]]),15,lightsteelblue);
        draw_fill_circle(pfv(ec[numero_ec_trie[0]]),10,lightgreen);
        wait_clic();
        ///AFFICHAGE //test

        fill_screen(black);
        for(i=0;i<nb_vect_sur_enveloppe;i++){
            draw_fill_circle(pfv(ec[numero_ec_trie[i]]),15,couleur_style(solary,i,nb_vect_sur_enveloppe));
            draw_line(pfv(ec[numero_ec_trie[i]]),pfv(ec[numero_ec_trie[(i+1)%nb_vect_enveloppe]]),lightgreen);
        }
    }
    *nb_vect_sur_enveloppee=nb_vect_sur_enveloppe;
    ///ne peux pas etre uttilisr partout avec *, wtf !
    ///-> solution variable transitoire
    //return nb_vect_sur_enveloppe;

}


///==================================================================
///====== Calul de l'envelope CONNEXE ?-> Marche de PIERRE ==========
///==================================================================
void calcul_marche_pierre(VECTEUR * ec,int nb_vect_enveloppe,VECTEUR P,int evolution,int numero_ec_trie2[]){
    ///                   |---------------------INPUT----------------------|--------OUTPUT-------|
    /**
    *   INPUT
    *      -> ec[nb_vect_enveloppe_max] -> le tableau contenant tous les point à étudié + de l'epace
    *      -> nb_vect_enveloppe -> la taille du tableau précédent à considérer
    *      -> P un point : c' est a partie de lui que l'on fait la lecture des angles
    *      -> evolution -> 1 = on afficle l'évolution (tuto) / 0 = opérationel !
    *
    *   OUTPUT :
    *      -> numero_ec_trie2[] -> ordre des points de ec[] formant l'enveloppe convexe
    *           la taille de ce tableau est nb_vect_enveloppe. Il ya autant de points
    *           indice 2 car le premier est transitoire !
    *
    *   Cette fonction (massive) retourne l'indice et l'ordre des points de l'enveloppe CONNEXE.
    *   Cette fonction prend comme nargument l'ensemble des points de E et un point P (vecteur)
    */
        int i;

    P = vecteur_null();
    //on crée un centre de gravité !
    for(i=0;i<nb_vect_enveloppe;i++){
        P.x += ec[i].x;
        P.y += ec[i].y;
        P.z += ec[i].z;
    }
        P.x /= nb_vect_enveloppe;
        P.y /= nb_vect_enveloppe;
        P.z /= nb_vect_enveloppe;
    if(evolution)draw_circle(pfv(P),15,darkorchid);
    ///MAIN ! ->
    #define nb_vect_enveloppe_max 1000
    ///Tableau indice (tableau temporel)
    int numero_ec_trie[nb_vect_enveloppe_max]={0};
    for(i=0;i<nb_vect_enveloppe_max;i++){numero_ec_trie[i]=-1;}

    ///DETERMINATION DU POINT INITIAL
    numero_ec_trie[0]=0;//indice_vecteur_xmin(ec,nb_vect_enveloppe);///Le premier point est le point de ec;
    ///ON DOIS DEMARRER A 0 SINON ON NE PEUX PAS LIRE CE POINT
    ///SOLUTION EMPLOYE DANS LA LECTURE INVERSE INIT A -1 ! Hé Hé !!

    if(evolution)draw_fill_circle(pfv(ec[numero_ec_trie[0]]),15,lightsteelblue);

    ///Variable pour algo itération
    float angle_min;    ///l'angle max obtenue !
    float angle_i;      ///angle mesure à chaque itération
    int meilleur;       ///Le numéro du meilleur angle !
    int i_point;
    ///CALCUL DU POINT N
    //1 pour initialise à un angle existant
    for(i_point=1;i_point<nb_vect_enveloppe;i_point++){

        meilleur = numero_ec_trie[0];//cas impossible ! Mathématiquement ~~impossible ! (sous rserve de nbtr)
        angle_min = 999;//Whaou, que c' est PETIT !
        for(i=0;i<nb_vect_enveloppe;i++){
            angle_i = angle_droite(ec[numero_ec_trie[i_point-1]],P,ec[i]);

            if(angle_i<angle_min && !is_in_tab(numero_ec_trie,nb_vect_enveloppe,i)){
                angle_min = angle_i;
                meilleur = i;
            }
        }
        numero_ec_trie[i_point]=meilleur;
        if(evolution){
            draw_fill_circle(pfv(ec[numero_ec_trie[i_point]]),5,couleur_style(solary,i_point,nb_vect_enveloppe));
            draw_line(pfv(ec[numero_ec_trie[i_point]]),pfv(ec[numero_ec_trie[i_point-1]]),couleur_style(solary,i_point,nb_vect_enveloppe));
        }
        //wait_clic();
    }
    if(evolution)wait_clic();
   /*
    fill_screen(black);
    for(i=0;i<nb_vect_enveloppe;i++){
        draw_fill_circle(pfv(ec[i]),5,couleur_style(solary,i,nb_vect_enveloppe));
        //draw_line(pfv(ec[i]),pfv(ec[(i+1)%nb_vect_enveloppe]),lightgrey);
    }*/
    //wait_clic();
    //int numero_ec_trie2[nb_vect_enveloppe_max];
    for(i=0;i<nb_vect_enveloppe_max;i++){numero_ec_trie2[i]=-1;}
    numero_ec_trie2[0]=numero_ec_trie[nb_vect_enveloppe-1];//le -0  dernier point !
    numero_ec_trie2[1]=numero_ec_trie[nb_vect_enveloppe-2];//le -1 dernier point !
    numero_ec_trie2[2]=numero_ec_trie[nb_vect_enveloppe-3];//le -2 dernier point !

    if(evolution){
        draw_fill_circle(pfv(ec[numero_ec_trie2[0]]),15,lightyellow);
        draw_fill_circle(pfv(ec[numero_ec_trie2[1]]),15,lightyellow);
        draw_line(pfv(ec[numero_ec_trie2[1]]),pfv(ec[numero_ec_trie2[0]]),lightyellow);
        draw_fill_circle(pfv(ec[numero_ec_trie2[2]]),15,lightyellow);
        draw_line(pfv(ec[numero_ec_trie2[2]]),pfv(ec[numero_ec_trie2[1]]),lightyellow);
    }

    for(i_point=3;i_point<nb_vect_enveloppe;i_point++){
        meilleur = numero_ec_trie2[0];
        angle_min = 999;//Whaou, que c' est PETIT !
        for(i=0;i<nb_vect_enveloppe;i++){
            angle_i = angle_droite(ec[numero_ec_trie2[i_point-1]],P,ec[i]);
            if(angle_i<angle_min && !is_in_tab(numero_ec_trie2,nb_vect_enveloppe,i)){
                angle_min = angle_i;
                meilleur = i;
            }
        }
        numero_ec_trie2[i_point]=meilleur;
        if(evolution){
            draw_fill_circle(pfv(ec[numero_ec_trie2[i_point]]),5,couleur_style(solary,i_point,nb_vect_enveloppe));
            draw_line(pfv(ec[numero_ec_trie2[i_point]]),pfv(ec[numero_ec_trie2[i_point-1]]),couleur_style(solary,i_point,nb_vect_enveloppe));
            //wait_clic();
        }
             //if(evolution)wait_clic();

    }
     if(evolution)wait_clic();

}


///RANDOM

///CALCUL ENVELOPPE CONNEXE V2 AJOUT POINTS

void init_tab(VECTEUR *tab, int size){
    int i;
    for(i=0;i<size;i++){
        //tab[i].content=rand()%5;
    }
}

char vecteur_egal(VECTEUR A,VECTEUR B){
    if (A.x==B.x && A.y==B.y && A.z==B.z)return 1;
    return 0;
}


///Supprime l'intervalle [pos1,pos2]
void cut_tab(VECTEUR *tab,int * size,int pos1,int pos2){

    ///erreur programmeur
    if(pos1>pos2)exit(1122);

    int i;
    ///le nombre de case à del
    int d = pos2-pos1+1;
    ///Calcul nouvelle taille
    *size-=d;
    ///Ou decale
    for(i=pos1;i<=*size;i++){
        tab[i]=tab[i+d];
    }
    ///On met à une valeur d'initialisation les cases crée
    for(i=*size;i<*size+d;i++){
        tab[i]=init_vecteur(0,0,0);
    }
}

void nettoyer_occurence_tab(VECTEUR *tab,int *size){
    int i,j;
    ///On cherche tous les occurences. En cas de rencontre, on supprime la case.
    for(j=*size-1;j>=0;j--){
        for(i=j-1;i>=0;i--){
            if(vecteur_egal(tab[j],tab[i])){
                cut_tab(tab,size,i,i);
            }
        }
    }
}

double frand_a_b(double a, double b){
    return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}
///SUIVI
void init_tab_suiveur(VECTEUR *tab, int size,VECTEUR min,VECTEUR max){
    const int d = 10;
    int i;
    for(i=0;i<size;i++){
        ///On cree des points uniquement dans le curbe forme par les points min et max
        tab[i]=init_vecteur(frand_a_b(min.x-d,max.x+d),frand_a_b(min.y-d,max.y+d),frand_a_b(min.z-d,max.z+d));
        ///Mettre une fc tour
    }
}

void fusionner_tab(VECTEUR *tab,int * taille_tab,VECTEUR *tab_add,int taille_tab_add){
    int i;
    for (i=0;i<taille_tab_add;i++){
        tab[i+*taille_tab]=tab_add[i];
    }
    *taille_tab+=taille_tab_add;
}

char is_vecteur_in_tab(VECTEUR *tab,int size,VECTEUR test){
    int i;
    for(i=0;i<size;i++){
        if(vecteur_egal(test,tab[i]))return 1;
    }
    return 0;
}

void supprimer_tab_suiveur(VECTEUR *tab,int * taille_tab,VECTEUR *tab_add,int taille_tab_add){
    int i;
    for (i=*taille_tab;i>=0;i--){
        if(is_vecteur_in_tab(tab_add,taille_tab_add,tab[i]))cut_tab(tab,taille_tab,i,i);
    }
    //*taille_tab-=taille_tab_add;
}

void calcul_marche_pierre_V2(VECTEUR * ec,int nb_vect_enveloppe,VECTEUR P,int evolution,int numero_ec_trie2[]){
    ///                   |---------------------INPUT----------------------|--------OUTPUT-------|
    /**
    *   INPUT
    *      -> ec[nb_vect_enveloppe_max] -> le tableau contenant tous les point à étudié + de l'epace
    *      -> nb_vect_enveloppe -> la taille du tableau précédent à considérer
    *      -> P un point : c' est a partie de lui que l'on fait la lecture des angles
    *      -> evolution -> 1 = on afficle l'évolution (tuto) / 0 = opérationel !
    *
    *   OUTPUT :
    *      -> numero_ec_trie2[] -> ordre des points de ec[] formant l'enveloppe convexe
    *           la taille de ce tableau est nb_vect_enveloppe. Il ya autant de points
    *           indice 2 car le premier est transitoire !
    *
    *   Cette fonction (massive) retourne l'indice et l'ordre des points de l'enveloppe CONNEXE.
    *   Cette fonction prend comme nargument l'ensemble des points de E et un point P (vecteur)
    */
        int i;///Tjr a ce ballader lui !!
    ///AJOUT TAB VALEUR
    //--> ENjoy the silence !
    nettoyer_occurence_tab(ec,&nb_vect_enveloppe);
    ///Pika Bho !

    VECTEUR max=ec[0],min=ec[0];

    /**
    *   max et min correspondent au deux points extreme le cube forme par ces deux points englobe tous les points
    *   Sert a generrer des points aleatoire dans cet interval
    */

    for(i=0;i<nb_vect_enveloppe;i++){
        if(ec[i].x>max.x)max.x = ec[i].x;
        if(ec[i].y>max.y)max.y = ec[i].y;
        if(ec[i].z>max.z)max.z = ec[i].z;

        if(ec[i].x<min.x)min.x = ec[i].x;
        if(ec[i].y<min.y)min.y = ec[i].y;
        if(ec[i].z<min.z)min.z = ec[i].z;
    }

    int sauvegarde_nb_vect_ebveloppe = nb_vect_enveloppe;/// On se la joue safe ^^

    /*
    P = vecteur_null();
    //on crée un centre de gravité !
    for(i=0;i<nb_vect_enveloppe;i++){
        P.x += ec[i].x;
        P.y += ec[i].y;
        P.z += ec[i].z;
    }
        P.x /= nb_vect_enveloppe;
        P.y /= nb_vect_enveloppe;
        P.z /= nb_vect_enveloppe;
    */
    if(evolution)draw_circle(pfv(P),15,darkorchid);
    ///MAIN ! ->
    #define nb_vect_enveloppe_max 200
    ///Tableau indice (tableau temporel)
    int numero_ec_trie[nb_vect_enveloppe_max]={0};
    for(i=0;i<nb_vect_enveloppe_max;i++){numero_ec_trie[i]=-1;}

    ///Tableau de point trandom
    #define nb_point_suivis 50
    if(nb_point_suivis+nb_vect_enveloppe>nb_vect_enveloppe_max)exit(nb_vect_enveloppe_max);//securité
    VECTEUR tab_point_suiveur[nb_point_suivis];
    init_tab_suiveur(tab_point_suiveur,nb_point_suivis,min,max);
    //printf("\nTableau suiveur :");
    //printf_tab(tab_point_suiveur,nb_point_suivis);
    ///On le fusionne a notre tableau de vecteur
    fusionner_tab(ec,&nb_vect_enveloppe,tab_point_suiveur,nb_point_suivis);
    //printf("\nTableau avec ajout tableau suiveur :");
    //printf_tab(tab,taille_utile);

    if(evolution){
        for(i=0;i<nb_vect_enveloppe;i++){
            draw_fill_circle(pfv(ec[i]),5,lightslategray);
        }
        wait_clic();
    }
    ///DETERMINATION DU POINT INITIAL
    numero_ec_trie[0]=0;//indice_vecteur_xmin(ec,nb_vect_enveloppe);///Le premier point est le point de ec;
    ///ON DOIS DEMARRER A 0 SINON ON NE PEUX PAS LIRE CE POINT
    ///SOLUTION EMPLOYE DANS LA LECTURE INVERSE INIT A -1 ! Hé Hé !!

    if(evolution)draw_fill_circle(pfv(ec[numero_ec_trie[0]]),15,lightsteelblue);

    ///Variable pour algo itération
    float angle_min;    ///l'angle max obtenue !
    float angle_i;      ///angle mesure à chaque itération
    int meilleur;       ///Le numéro du meilleur angle !
    int i_point;
    ///CALCUL DU POINT N
    //1 pour initialise à un angle existant
    for(i_point=1;i_point<nb_vect_enveloppe;i_point++){

        meilleur = numero_ec_trie[0];//cas impossible ! Mathématiquement ~~impossible ! (sous rserve de nbtr)
        angle_min = 999;//Whaou, que c' est PETIT !
        for(i=0;i<nb_vect_enveloppe;i++){
            angle_i = angle_droite(ec[numero_ec_trie[i_point-1]],P,ec[i]);

            if(angle_i<angle_min && !is_in_tab(numero_ec_trie,nb_vect_enveloppe,i)){
                angle_min = angle_i;
                meilleur = i;
            }
        }
        numero_ec_trie[i_point]=meilleur;
        if(evolution){
            draw_fill_circle(pfv(ec[numero_ec_trie[i_point]]),5,couleur_style(solary,i_point,nb_vect_enveloppe));
            draw_line(pfv(ec[numero_ec_trie[i_point]]),pfv(ec[numero_ec_trie[i_point-1]]),couleur_style(solary,i_point,nb_vect_enveloppe));
        }
        //wait_clic();
    }
    if(evolution){
        wait_clic();
        for(i=0;i<nb_vect_enveloppe;i++){
            draw_fill_circle(pfv(ec[numero_ec_trie[i]]),15,darkmagenta);
            draw_line(pfv(ec[numero_ec_trie[i]]),pfv(ec[numero_ec_trie[(i+1)%nb_vect_enveloppe]]),lightgrey);
        }
        wait_clic();
    }
    ///GOOD

    ///On supprime les poits de suivi
    ///IL faut désormait supprimer les points précédents qui sont dans le désordre
    //supprimer_tab_suiveur(tab,&taille_utile,tab_point_suiveur,nb_point_suivis);

    int j;
    for (i=nb_vect_enveloppe;i>=0;i--){
        if(is_vecteur_in_tab(tab_point_suiveur,nb_point_suivis,ec[numero_ec_trie[i]])){
            //if(evolution)draw_fill_circle(pfv(ec[numero_ec_trie[i]]),20,darkmagenta);
            //cut_tab(numero_ec_trie,&nb_vect_enveloppe,i,i);
            if(evolution)draw_fill_circle(pfv(ec[numero_ec_trie[i]]),15,darkgray);
            ///NEw cut tab // DSL MA FC
            for(j=i;j<=nb_vect_enveloppe;j++){
                numero_ec_trie[j]=numero_ec_trie[j+1];
            }
            ///
            ///REMPLACEZ J PAR I WHAOU ! ca marche ! recursivité (sacré erreur)
            ///
            nb_vect_enveloppe--;
            if(evolution)draw_fill_circle(pfv(ec[numero_ec_trie[i]]),10,lightskyblue);
        }
    }
    if(evolution)wait_clic();
    ///Bon trie !
    ///On vérifie que la supression du tableau est bonne :
    if(nb_vect_enveloppe!=sauvegarde_nb_vect_ebveloppe)exit(66);


    if(evolution){
        for(i=0;i<nb_vect_enveloppe;i++){
            draw_fill_circle(pfv(ec[numero_ec_trie[i]]),15,lightgreen);
            draw_line(pfv(ec[numero_ec_trie[i]]),pfv(ec[numero_ec_trie[(i+1)%nb_vect_enveloppe]]),lightgreen);
        }
        wait_clic();

    }
    ///GOOD
    if(evolution)fill_screen(0);

    ///On initialise nos 3 premiers proint avec les 3 dernieres valeurs !
    for(i=0;i<nb_vect_enveloppe;i++){numero_ec_trie2[i]=-1;}
    numero_ec_trie2[0]=numero_ec_trie[nb_vect_enveloppe-1];//le -0  dernier point !
    numero_ec_trie2[1]=numero_ec_trie[nb_vect_enveloppe-2];//le -1 dernier point !
    numero_ec_trie2[2]=numero_ec_trie[nb_vect_enveloppe-3];//le -2 dernier point !

    if(evolution){
        draw_fill_circle(pfv(ec[numero_ec_trie2[0]]),15,lightyellow);
        draw_fill_circle(pfv(ec[numero_ec_trie2[1]]),15,lightyellow);
        draw_line(pfv(ec[numero_ec_trie2[1]]),pfv(ec[numero_ec_trie2[0]]),lightyellow);
        draw_fill_circle(pfv(ec[numero_ec_trie2[2]]),15,lightyellow);
        draw_line(pfv(ec[numero_ec_trie2[2]]),pfv(ec[numero_ec_trie2[1]]),lightyellow);
    }
    for(i_point=3;i_point<nb_vect_enveloppe;i_point++){
        meilleur = numero_ec_trie2[0];
        angle_min = 999;//Whaou, que c' est PETIT !
        for(i=0;i<nb_vect_enveloppe;i++){
            angle_i = angle_droite(ec[numero_ec_trie2[i_point-1]],P,ec[i]);
            if(angle_i<angle_min && !is_in_tab(numero_ec_trie2,nb_vect_enveloppe,i)){
                angle_min = angle_i;
                meilleur = i;
            }
        }
        numero_ec_trie2[i_point]=meilleur;
        if(evolution){
            draw_fill_circle(pfv(ec[numero_ec_trie2[i_point]]),20,couleur_style(solary,i_point,nb_vect_enveloppe));
            draw_line(pfv(ec[numero_ec_trie2[i_point]]),pfv(ec[numero_ec_trie2[i_point-1]]),couleur_style(solary,i_point,nb_vect_enveloppe));
            //wait_clic();
        }
             //if(evolution)wait_clic();

    }
    ///On calcul l'enveloppe connexe.
    ///<--

    if(evolution)wait_clic();
}


int randd(int a){
    //return rand() % (a-100)+50;
    return rand() % (a-100)+50;
}



int main (int argc, char** argv )
{
    ///============================= CREATION FENETRE GRAPHIQUE =============================
    init_graphics(xs,ys,SDL_TRUE,"STL");
    set_affiche_auto(1);
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

    ///--> NON ROBUSTE = DEL !!
    ///===========INITIALISATION MARCHE JARVIS + MARCHE PIERRE V1 + MARCHE PIERRE V2 ====================
    #define nb_vect_enveloppe_max 1000
    ///LE TABLEAU CONTENANT TOUS LES POINTS
    VECTEUR ec[nb_vect_enveloppe_max]={0,0,0};///Marche ? -> si oui securité
    ///nombre de point finis appartenant à l'enveloppe PAS convexe  E:
    int nb_vect_enveloppe = 5;///Varie à nouvelle itération delaunay
    //LE TABLEAU DE TRIS
    int numero_ec_trie[nb_vect_enveloppe_max]={0};// ??? Wtf jarvis ! -> correction ///Ok jarvis !
    int nb_vect_sur_enveloppe = 0;
    nb_vect_enveloppe=0;
    ///mes vecteur sont dans ec
    ///<-- FIN


    ///
    VECTEUR p;

    #define evolution 1
    #define draw_cercle 0
    while(1){
        nb_vect_enveloppe=0;
        nb_vect_sur_enveloppe=0;

        p = vfp(wait_clic());
        //p.x = rand()%(xs-200)+100;
        //p.y = rand()%(ys-200)+100;
        /// Dans tous les tr, on récupere le nombre et l'adresses des tr ou il y a collision ===== 333
        ///Correspond à l'ensembre des points de l'envelope PAS PAS PAS PAS PAS convexe ???

        for(i=0;i<NBTR;i++){
            if(tr[i].etat){///On reste dans le monde des vivants, c' est deja bien :p
                if (collision_vecteur_dans_cercle_circonscrit_a_3_points(p,tr[i].A,tr[i].B,tr[i].C)){
                    ec[nb_vect_enveloppe]=tr[i].A;
                    ec[nb_vect_enveloppe+1]=tr[i].B;
                    ec[nb_vect_enveloppe+2]=tr[i].C;
                    ///Affichage des tr qui depop :
                    if(evolution)draw_triangle_vecteur(tr[i],darkblue);
                    tr[i]=free_triangle();
                    nb_vect_enveloppe+=3;
                }
            }
        }

        if(evolution){///Affichage tr del
            affiche_all();
            wait_clic();
            fill_screen(black);
            draw_all_triangle(tr,NBTR,draw_cercle,lightgreen);
            affiche_all();
            wait_clic();
        }

        ///Calcul enveloppe connexe
        calcul_marche_pierre_V2(ec,nb_vect_enveloppe,p,1,numero_ec_trie);

        ///On cree de nouveau tr :
        for(i=0;i<nb_vect_enveloppe;i++){
            //draw_fill_circle(pfv(ec[numero_ec_trie[i]]),30,couleur_style(solary,i,nb_vect_enveloppe));
            //draw_line(pfv(ec[numero_ec_trie[i]]),pfv(ec[numero_ec_trie[(i+1)%nb_vect_enveloppe]]),lightgreen);
            ///Pour ne pas cree de triangle plat ! MARCHE EN CARTON ! Bug -> Bug -> bug -> bug ...
            if(!vecteur_egal(ec[numero_ec_trie[i]],p)&&!vecteur_egal(ec[numero_ec_trie[(i+1)%nb_vect_enveloppe]],p)&&!vecteur_egal(ec[numero_ec_trie[i]],ec[numero_ec_trie[(i+1)%nb_vect_enveloppe]])){
                numero_tr_libre = numero_triangle_libre(tr,NBTR);
                tr[numero_tr_libre]=new_triangle(ec[numero_ec_trie[i]],ec[numero_ec_trie[(i+1)%nb_vect_enveloppe]],p);
                draw_triangle_vecteur(tr[numero_tr_libre],couleur_style(solary,i,nb_vect_enveloppe));
                if(draw_cercle)draw_cercle_triangle(tr[numero_tr_libre]);
                affiche_all();
                //wait_clic();
            }
        }
        //wait_clic();
        ///On affiche le resultat final
        if(evolution)affiche_all();
        fill_screen(black);
        draw_all_triangle(tr,NBTR,draw_cercle,lightgreen);
        if(!evolution)affiche_all();
        wait_poll_escape()
    }

    ///Fin delaunay


    affiche_all();
    wait_escape();
    return 0;
}
