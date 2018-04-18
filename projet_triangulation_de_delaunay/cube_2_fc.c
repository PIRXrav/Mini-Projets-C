
/**
Le format que nous choisissons d’étudier est le format STL (pour STéreoLithographie).
Dans un fichier STL, un objet en trois dimensions est découpé en triangles. Un objet
peut contenir parfois plusieurs millions de triangles. La structure du fichier
est la suivante : (exmple d'un tétrèdre)

4578 706f 7274 6564 2066 726f 6d20 426c //2*8 octet	-> header
656e 6465 722d 322e 3639 2028 7375 6220	//32
3029 0000 0000 0000 0000 0000 0000 0000 //48
0000 0000 0000 0000 0000 0000 0000 0000 //64
0000 0000 0000 0000 0000 0000 0000 0000 //80 octet <-- end header

0400 0000								//taille
[   X   ] [	  Y   ] [   Z   ]
        TR1
0000 0000 0000 0000 0000 0000 //la direction normale au triangle
0000 0000 0000 0000 0000 803f //1 [0:0:1]			A
ef5b 713f 0000 0000 aaaa aabe //2 [0.94:0:-0.33]	B
ef5b f1be ec05 513f aaaa aabe //3 [-0.47:0.81:-0.33]C
0000							// 2 octet de controle

		TR2
0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 803f A
ef5b f1be ec05 513f aaaa aabe C
ef5b f1be ec05 51bf aaaa aabe ->D
0000

		TR3
0000 0000 0000 0000 0000 0000
0000 0000 0000 0000 0000 803f A
ef5b f1be ec05 51bf aaaa aabe ->D
ef5b 713f 0000 0000 aaaa aabe B
0000

		TR4
0000 0000 0000 0000 0000 0000
ef5b 713f 0000 0000 aaaa aabe B
ef5b f1be ec05 51bf aaaa aabe ->D
ef5b f1be ec05 513f aaaa aabe C
0000

les 4 sommets sont les suivants :
A-[0.0, 0.0, 1.0]
B-[0.9428090453147888, 0.0, -0.3333333134651184]
C-[-0.4714045226573944, 0.8164966106414795, -0.3333333134651184]
D-[-0.4714045226573944, -0.8164966106414795, -0.3333333134651184]

http://www.fil.univ-lille1.fr/~L1S2API/CoursTP/fichier_stl.html
https://fr.wikipedia.org/wiki/Fichier_de_st%C3%A9r%C3%A9olithographie
*/

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

char * set_input_file(char * name_file_fc,char * extention){
    ///static force la conservation du tableau
    static char input_file[100]= {"\0"};
    strcat(input_file,name_file_fc);
    strcat(input_file,extention);
    return input_file;
}

char * set_output_file(char * name_file_fc){
    static char output_file[100]= {"\0"};
    strcat(output_file,name_file);
    strcat(output_file,"_output.txt");
    return output_file;
}
/*
int main()
{

    int i;
    for(i=0; i<nb_triangle; i++){
        printf("========================================================\n");
        printf("Triangle %d : \n",i+1);
        print_cord_triangle(tr[i]);
    }


    return 0;
}


*/


typedef struct{
    POINT pA,pB,pC;
}TRIANGLE_2D;



TRIANGLE_2D calcul_triangle_2D(TRIANGLE_IN_FILE tr,float amplification, float dist_camera_plan,float dist_plan_objet){
    TRIANGLE_2D T;//Les 3 points du triangle 2D
    VECTEUR A,B,C;//les 3 points du triangle 3D
    float profondeurA,profondeurB,profondeurC;

    A = tr.a;
    profondeurA = A.y;
    T.pA.x=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeurA)*(A.x*amplification)+400;
    T.pA.y=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeurA)*(A.z*amplification)+150;
    //draw_pixel(pA,blueviolet);

    B = tr.b;
    profondeurB = B.y;
    T.pB.x=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeurB)*(B.x*amplification)+400;
    T.pB.y=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeurB)*(B.z*amplification)+150;
    //draw_pixel(pB,greenyellow);

    C = tr.c;
    profondeurC = C.y;
    T.pC.x=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeurC)*(C.x*amplification)+400;
    T.pC.y=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeurC)*(C.z*amplification)+150;
    //draw_pixel(pC,yellow);

    return T;
}

POINT calcul_point_2D_phi(VECTEUR POINT_TRIANGLE,float phi,float amplification, float dist_camera_plan,float dist_plan_objet){

    ///=============PROJET CUBE + ATTRACTEUR DE LORENZ ====================

    float r;                ///rayon du point (centre de rotation)
    float angle;            ///angle initial du point
    float profondeur;       ///profondeur du point
    float def;              ///coeffichient de deformation
    POINT p;                ///cordonnée dans le repere plan
    #define draw 1

    ///rayon du point (centre de rotation)
    r = sqrt(POINT_TRIANGLE.x*POINT_TRIANGLE.x+POINT_TRIANGLE.y*POINT_TRIANGLE.y);
    ///angle initial du point ...Etrange etrange ...
    angle = -atan((POINT_TRIANGLE.y+0)/(POINT_TRIANGLE.x+0));
    if(POINT_TRIANGLE.y>=0&&POINT_TRIANGLE.x<=0)      angle+=pi;  //pourquoi pi
    else if(POINT_TRIANGLE.y<=0&&POINT_TRIANGLE.x<=0) angle+=pi;//ok
    else if(POINT_TRIANGLE.y<=0&&POINT_TRIANGLE.x>=0) angle+=0;//pourquoi 0
    else                                                                  angle+=0;
    ///profondeur du point :
    profondeur = sin(phi+angle+pi)*r;
    ///coeffichient de deformation : def
    def = dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeur);
    ///cordonnée dans le repere plan :
    p.x=def*(cos(phi+angle)*r*amplification)+400+950;
    p.y=def*(POINT_TRIANGLE.z*amplification)+150;

    #if draw
    draw_pixel(p,couleur_style(solary,profondeur+30,75));
    #endif // draw
    return p;
}
int main ( int argc, char** argv )
{
    printf("INIT FILE STL\n");

    ///=============================== INIT FILE STL =======================================
    char * input_file = set_input_file(name_file,".stl");
    printf("Input file : %s\n",input_file);

    ///=================================READ STL ===========================================

    int nb_triangle;
    TRIANGLE_IN_FILE *tr;
    tr = read_STL(input_file,&nb_triangle);

    ///=============================== VARIABLES ===========================================

    ///varable itératives
    int const nb_iteration=nb_triangle;       ///Nombre itération a realiser
    int iteration;                      ///varable pour calculer les point de 0 à nb_iteration

    ///Passage de la 3D à la 2D :
    float const amplification=20;       ///Zoom sur la structure pour la rendre visible.
    float const dist_camera_plan=70;    ///position de la camera. On ne tient pas compte de son depl sur x et y.
    float const dist_plan_objet=70;
    float def;                          ///coefficient de deformation

    ///Variable d'affichage :

    ///pour mettre l'ensemble des points en rotation :
    float phi=0;                        ///angle correspond a la rotation de la structure
    float angle;                        ///angle du point avant l'application de phi
    float r;                            ///rayon de ce même point



    ///============================= CREATION FENETRE GRAPHIQUE =============================

    init_graphics(1800,900,SDL_TRUE,"STL");
    set_affiche_auto(SDL_FALSE);
    fill_screen(0);
    affiche_all();


    ///===================== AFFICHAGE AVEC PERSPECTIVE - STATIQUE ==========================
    TRIANGLE_2D T;
    for(iteration=0;iteration<nb_triangle;iteration++){
        T = calcul_triangle_2D(tr[iteration],amplification,dist_camera_plan,dist_plan_objet);
        draw_fill_triangle(T.pA,T.pB,T.pC,greenyellow);
    }
    affiche_all();
   // wait_escape();

    ///===================== AFFICHAGE AVEC PERSPECTIVE - ROTATION =========================
    POINT p;
    TRIANGLE_2D T2;
    for(phi=0;1;phi+=0.07){
        for(iteration=0;iteration<nb_iteration;iteration++){
            T2.pA = calcul_point_2D_phi(tr[iteration].a,phi,amplification,dist_camera_plan,dist_plan_objet);
            //T2.pB = calcul_point_2D_phi(tr[iteration].b,phi,amplification,dist_camera_plan,dist_plan_objet);
            //T2.pC = calcul_point_2D_phi(tr[iteration].c,phi,amplification,dist_camera_plan,dist_plan_objet);
            //draw_triangle(T2.pA,T2.pB,T2.pC,greenyellow);
        }

        affiche_all();

        POINT p1={800+200,0},p2={1800,900};draw_fill_rectangle(p1,p2,noir);
        //fill_screen(black);
        wait_poll_escape();
    }


    free(tr);//ca soulage :)
    return 0;
}
