#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "graphics\graphics.h"

#define wait_poll_escape() SDL_Event escape;SDL_PollEvent(&escape);if(escape.type == SDL_QUIT)break;

#define pi 3.1415


    float dist_camera_plan;
    float dist_plan_objet;

typedef struct{
     float x,y,z;       ///Ses cordonnées dans le repere (x,y,z)
     float planx,plany; ///Ses cordonné dans le repere plan R(planx,plany)
}SOMMET;

typedef struct{
    SOMMET sommet[8];
    ///Les 4 premiers sommets forment le carré supérieur
    ///Les 4 dernier sommets forment le carré inférieur
    float cote;     ///la taille d'un coté
    float zplus;    ///la position du carré supérieur
    float zmoins;   ///La position du carré inférieur
}CUBE;

void init_cube(CUBE *cube,float cote,float zplus,float zmoins){
    cube->cote=cote;
    cube->zplus=zplus;
    cube->zmoins=zmoins;
}

void calcul_sommet(SOMMET *sommet,float phi,float cote,float dz){
    ///repere 3D :
    /// En projection sur (x,y):
        float R = sqrt(2* cote * cote)/2;//rayon du cercle dans le plan XY
    /// Soit une equation parametrique de cercle le repere (x,y,z):
        sommet->x=cos(phi+pi)*R;
        sommet->y=sin(phi)*R;
        sommet->z=dz;

    ///En projection dans le repere plan R(planx,plany):
    sommet->planx=-dist_camera_plan/(dist_camera_plan+dist_plan_objet+sommet->x)*sommet->y;
    sommet->plany=dist_camera_plan/(dist_camera_plan+dist_plan_objet+sommet->x)*dz;

    // A faire : Ajouter le depl sur 3 axes de la camera aux équations
}

void calcul_cube(CUBE *cube,float phi){
    int SM;
    ///On calcule la position de tous les sommets :
    for(SM=0;SM<4;SM++){
        calcul_sommet(&cube->sommet[SM],phi-SM*pi/2,cube->cote,cube->zplus);
        calcul_sommet(&cube->sommet[SM+4],phi-SM*pi/2,cube->cote,cube->zmoins);
    }
}

void draw_cube(CUBE *cube,COULEUR color){

    int amplification=100;
    int decallage=500;

    POINT p[8];
    int SM;
    for(SM=0;SM<4;SM++){

        p[SM].x=cube->sommet[SM].planx*amplification+decallage;
        p[SM].y=cube->sommet[SM].plany*amplification+decallage;
        p[SM+4].x=cube->sommet[SM+4].planx*amplification+decallage;
        p[SM+4].y=cube->sommet[SM+4].plany*amplification+decallage;

        // WTFFF
        //draw_line(p[SM],p[(SM+1)%4],white);
        //draw_line(p[SM+4],p[((SM+1)%4)+4],rouge);
        draw_line(p[SM],p[SM+4],color);
    }

    draw_line(p[0],p[1],color);
    draw_line(p[1],p[2],color);
    draw_line(p[2],p[3],color);
    draw_line(p[3],p[0],color);

    draw_line(p[4],p[5],color);
    draw_line(p[5],p[6],color);
    draw_line(p[6],p[7],color);
    draw_line(p[7],p[4],color);

    /*  Draw center + diagonales
    POINT pc={500,500};
    draw_fill_circle(pc,3,rouge);
    draw_line(p[0],p[6],color);
    draw_line(p[1],p[7],color);
    draw_line(p[2],p[4],color);
    draw_line(p[3],p[5],color);

*/
}



int main ( int argc, char** argv )
{

    init_graphics(900,900,SDL_TRUE,"3D");
    set_affiche_auto(SDL_FALSE);
    fill_screen(0);
    affiche_all();

    dist_camera_plan = 10;
    dist_plan_objet  = 10;


    CUBE cube;
    init_cube(&cube,5,2.5,-2.5);

    CUBE cube2;
    init_cube(&cube2,4,4,1);

    CUBE cube3;
    init_cube(&cube3,1,5,-7);


    float phi=0;

    for(phi=0;1;phi+=0.01){

        if(phi>2*pi)phi-=2*pi;

        if(phi<pi)init_cube(&cube,2*phi,phi,-phi);
        else init_cube(&cube,4*pi-2*phi,2*pi-phi,-2*pi+phi);
        calcul_cube(&cube,phi);
        draw_cube(&cube,vert);

        calcul_cube(&cube2,phi/2);
        draw_cube(&cube2,rouge);

        calcul_cube(&cube3,2*phi);
        draw_cube(&cube3,bleu);



        affiche_all();
        fill_screen(0);

        wait_poll_escape();
    }

       // wait_escape();

    return 0;
}
