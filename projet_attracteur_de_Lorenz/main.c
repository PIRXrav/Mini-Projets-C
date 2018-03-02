/**
L’attracteur de Lorenz est une structure fractale correspondant au comportement à long terme
de l'oscillateur de Lorenz. L'attracteur montre comment les différentes variables du système
dynamique évoluent dans le temps en une trajectoire non périodique.

    https://fr.wikipedia.org/wiki/Attracteur_de_Lorenz

Objectif :
    1) Etude de l'attracteur de lorenz et resolution de ce système d'équations différentielles
       avec la méthode d'euler. https://fr.wikipedia.org/wiki/M%C3%A9thode_d%27Euler
    2) Représentation dans une fenetre graphique
    3) Ajout de perspective : VOIR PROJET CUBE ! (Equation de transfert)
    4) Ajout de mouvement (rotation autour d'un axe central) : (Nouveau)
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics\graphics.h"

#define wait_poll_escape() SDL_Event escape;SDL_PollEvent(&escape);if(escape.type == SDL_QUIT)break;


typedef struct{ ///vitesse ou position dans l'espace 3D
    float x,y,z;
}VECTEUR;

VECTEUR equation_lorenz(VECTEUR position){
    ///Voir fonction new_position
    ///Le système d'équations différentielles de Lorenz retournent une vitesse.

    ///On pose souvent :
    float sigma = 10;
    float rho = 28;
    float beta = 8/3;

    VECTEUR vitesse;

    vitesse.x=sigma*(position.y-position.x);
    vitesse.y=rho*position.x - position.y - position.x*position.z;
    vitesse.z=position.x*position.y - beta*position.z;
    ///https://wikimedia.org/api/rest_v1/media/math/render/svg/c32b412baae29eb2c914c397d425023188c3d433

    return vitesse;
}

void calcul_position(VECTEUR *position,VECTEUR vitesse){
    ///Voir fonction new_position
    float deltat = 0.0007;
    position->x+=vitesse.x*deltat;
    position->y+=vitesse.y*deltat;
    position->z+=vitesse.z*deltat;

}

void new_position(VECTEUR *position){
    /**
    *   Résolutuion avec la méthode d'Euler d'un système d'équations différentielles
    *   https://fr.wikipedia.org/wiki/M%C3%A9thode_d%27Euler
    *
    *   Soit Xnew = Xlast + Xlast' * deltat
    *   Tel que Xlast est la vitesse de X à cet instant. on l'applique
    *   sur un temps infinitésimal dt
    */
    ///Calcul de la vitesse :
    VECTEUR vitesse = equation_lorenz(*position);
    ///On applique notre vitesse à notre position :
    calcul_position(position,vitesse);
}


float absf(float a){
    return sqrt(a*a);
}
int main ( int argc, char** argv )
{
    ///=============================== VARIABLES ===========================================

    ///Generations de l'ensemble des point verifiant le système de l'attracteur de lorenz
    int const nb_iteration=90000;       ///Nombre itération a realiser
    VECTEUR position={0,0.1,20};        ///Valeur initiale de notre système. Différente de 0,0,0.
    VECTEUR data_position[nb_iteration];///Ensemble des points
    int iteration;                      ///varable pour calculer les point de 0 à nb_iteration

    ///Passage de la 3D à la 2D :
    float const dist_camera_plan=40;    ///position de la camera. On ne tient pas compte de son depl sur x et y.
    float const dist_plan_objet=40;
    float profondeur;                   ///Profondeur du point
    float def;                          ///coefficient de deformation

    ///Variable d'affichage :
    POINT p;                            ///Le point que l'on affiche a l'ecran
    float const amplification=20;       ///Zoom sur la structure pour la rendre visible.

    ///pour mettre l'ensemble des points en rotation :
    float phi=0;                        ///angle correspond a la rotation de la structure
    float angle;                        ///angle du point avant l'application de phi
    float r;                            ///rayon de ce même point
    #define pi 3.1415                   ///trigonometrie


    ///============================= CREATION FENETRE GRAPHIQUE =============================

    init_graphics(1800,900,SDL_TRUE,"attracteur de lorenz");
    set_affiche_auto(SDL_FALSE);
    fill_screen(0);
    affiche_all();

    ///============================CALCUL DES POSITIONS REPERE 3D ===========================
    /// + Affichage sans perspective
    for (iteration=0;iteration < nb_iteration;iteration++){

        data_position[iteration]=position;
        /// la premiere position est deja initialisé

        new_position(&position);

        p.x =(position.x*15+400)-100;
        p.y=(position.z*15)+50;
        draw_pixel(p,couleur_style(lunary,position.y+25,60));
        ///On affiche en supprimant une dimention (donc pas de perspective)
    }



    ///===================== AFFICHAGE AVEC PERSPECTIVE - STATIQUE ==========================
    for(iteration=0;iteration<nb_iteration;iteration++){

        profondeur = data_position[iteration].y;
        p.x=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeur)*(0+data_position[iteration].x*amplification)+500+400;
        p.y=dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeur)*(0+data_position[iteration].z*amplification)+150;

        draw_pixel(p,couleur_style(solary,profondeur+25,60));
    }
    affiche_all();

    ///===================== AFFICHAGE AVEC PERSPECTIVE - ROTATION =========================
    for(phi=0;1;phi+=0.07){
        for(iteration=0;iteration<nb_iteration;iteration++){
            ///rayon du point (centre de rotation)
            r = sqrt(data_position[iteration].x*data_position[iteration].x+data_position[iteration].y*data_position[iteration].y);
            ///angle initial du point ...Etrange etrange ...
            angle = -atan((data_position[iteration].y+0)/(data_position[iteration].x+0));
            if(data_position[iteration].y>=0&&data_position[iteration].x<=0)      angle+=pi;  //pourquoi pi
            else if(data_position[iteration].y<=0&&data_position[iteration].x<=0) angle+=pi;//ok
            else if(data_position[iteration].y<=0&&data_position[iteration].x>=0) angle+=0;//pourquoi 0
            else                                                                  angle+=0;
            ///profondeur du point :
            profondeur = sin(phi+angle+pi)*r;
            ///coeffichient de deformation : def
            def = dist_camera_plan/(dist_camera_plan+dist_plan_objet+profondeur);
            ///cordonnée dans le repere plan :
            p.x=def*(0+cos(phi+angle)*r*amplification)+500+950;
            p.y=def*(0+data_position[iteration].z*amplification)+150;
            draw_pixel(p,couleur_style(solary,profondeur+30,75));
        }
        affiche_all();

        POINT p1={900+200,0},p2={1800,900};draw_fill_rectangle(p1,p2,noir);
        //fill_screen(black);
        wait_poll_escape();
    }
    return 0;
}
