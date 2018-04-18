#include <stdio.h>
#include <stdlib.h>
#include <math.h>
///V1
typedef struct{ ///vitesse ou position dans l'espace 3D
    float x,y,z;
}VECTEUR;

float distance_vecteur(VECTEUR A,VECTEUR B){///Pytagore 3D
    return sqrt(pow(A.x-B.x,2)+pow(A.y-B.y,2)+pow(A.z-B.z,2));
}

/**
*   STRUCTURE CERCLE CIRCONSCRIT
*   centre -> VECTEUR centre du cerche
*   rayon -> float rayon du cercle
*/
typedef struct{
    VECTEUR centre;
    float rayon;
}CERCLE_CIRCONSCRIT;
///==========================================================================
///================================= NEW ====================================
///==========================================================================
///V2

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
     ///Soit V2 le vecteur directeur de la droite P1 P2
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

int main()
{
    ///Nous avons nos 3 points :
    //VECTEUR P1={5,-1,4},P2={8,1,-3},P3={-1,2,4};
    VECTEUR A={0,0,0},B={10,0,0},C={0,10,0};

    CERCLE_CIRCONSCRIT cercle = calcul_cercle_circonscrit_V2(B,A,C);
    print_vecteur(cercle.centre);
    printf("\n rayon = %f",cercle.rayon);

    ///Vecteur pour tester collision
    VECTEUR P={5,5,8};//avec ce point x et y sont dans le cercle circonscrit, mais pas dans lasphere -> non colision !
    printf("\nCollision : %d",collision_vecteur_dans_cercle_circonscrit_a_3_points(P,A,B,C));

    return 0;
}

