#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL.h>
	
	//vecteur ?

	typedef struct point {int x,y;} POINT;

    typedef enum{
        no,lunary,solary
    }STYLE;
	typedef Uint32 COULEUR;


	int WIDTH;
	int HEIGHT;

	#include "couleur.h"



///================================================================================
///                               INITIALISATION SDL
///================================================================================

void init_graphics(int W, int H,char showcursor,char * title);

void set_affiche_auto(char affiche_auto);

void affiche_all(void);
POINT wait_clic(void);

void wait_escape(void);

///================================================================================
///                                   COULEUR
///================================================================================

COULEUR couleur_style(STYLE style,int x,int xmax);

///================================================================================
///                                    DESSIN
///================================================================================

void fill_screen(COULEUR color);
#define add_pix(x,y,color)  if (dans_ecran((x),(y))) *((COULEUR *)screen->pixels + (HEIGHT-(y)-1) * WIDTH + (x)) = (color)

void draw_pixel(POINT p, COULEUR color);

void draw_line(POINT p1, POINT p2, COULEUR color);

void draw_rectangle(POINT p1, POINT p2, COULEUR color);

void draw_fill_rectangle(POINT p1, POINT p2, COULEUR color);

void draw_circle(POINT centre, int rayon, COULEUR color);

void draw_fill_circle(POINT centre, int rayon, COULEUR color);

void draw_fill_ellipse(POINT F1, POINT F2, int r, COULEUR color);

void draw_triangle(POINT p1, POINT p2, POINT p3, COULEUR color);

void draw_fill_triangle(POINT p1, POINT p2, POINT p3, COULEUR color);

void chaos(void);
