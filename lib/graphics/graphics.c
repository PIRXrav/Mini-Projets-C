#include "graphics.h"

#define MAX_WIDTH  2048
#define MAX_HEIGHT 1200

SDL_Surface * screen;
char SDL_AFFICHE_AUTO = SDL_FALSE;//auto flip


///================================================================================
///                               INITIALISATION SDL
///================================================================================

void init_graphics(int W, int H,char showcursor,char * title)
{
    // Initialisation d'une taille raisonnable
    if ((W>10) && (W<MAX_WIDTH )) WIDTH  = W;
    else WIDTH  = 600;
    if ((H>10) && (H<MAX_HEIGHT)) HEIGHT = H;
    else HEIGHT = 400;

    // Initialisation de la SDL_surface
    SDL_Init(SDL_INIT_VIDEO);

    //SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), 0 );

    /**
    // SDL_HWSURFACE : les données seront chargées dans la mémoire vidéo.
    // SDL_SWSURFACE : les données seront chargées dans la mémoire système.
    // SDL_RESIZABLE : la fenêtre sera redimensionnable. Par défaut elle ne l'est pas.
    // SDL_NOFRAME : la fenêtre n'aura pas de barre de titre ni de bordure.
    // SDL_FULLSCREEN : mode plein écran. Full HD : 1920 * 1080
    // SDL_DOUBLEBUF : mode double buffering.
    */
    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( screen == NULL )
    {
        fprintf(stderr, "Impossible de passer en %dx%d en 32 bits: %s\n", WIDTH, HEIGHT, SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_ShowCursor(showcursor);

    // Autorise la prise en compte de répétition lors d'un appui long sur une touche
   // SDL_EnableKeyRepeat(1,0);//?? a approfondir

    SDL_WM_SetCaption(title,NULL);
}

void set_affiche_auto(char affiche_auto)
{
    SDL_AFFICHE_AUTO = affiche_auto;
}


void affiche_all(void)
{
    SDL_Flip(screen);
}

POINT wait_clic(void)
{
    int encore = 1;
    POINT P;
    SDL_Event event;
    P.x = 0;
    P.y = 0;
    while (SDL_WaitEvent(&event) && encore)
    {
        if ((event.type == SDL_MOUSEBUTTONDOWN) && (event.button.button == SDL_BUTTON_LEFT))
        {
            encore=0;
            P.x = event.button.x;
            P.y = HEIGHT-event.button.y;
        }
        if (event.type == SDL_MOUSEMOTION)
        {
            fflush(stdout);
        }
        if (event.type == SDL_QUIT) exit(0);

    }
    return P;
}

void wait_escape(void)
{
    char done = 0;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // check for messages
            switch (event.type)
            {
            // exit if the window is closed
            case SDL_QUIT:
                done = 1;
                break;
            // check for keypresses
            case SDL_KEYDOWN:
                // exit if ESCAPE is pressed
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = 1;
                break;
            } // end switch
        } // end of message processing
    } // end main loop
}

///================================================================================
///                                   COULEUR
///================================================================================

COULEUR couleur_style(STYLE style,int x,int xmax)
{
    COULEUR p;
    Uint8 r,g,b;
    switch(style)
    {
    case no:
        r=0;
        g=0;
        b=0;
        break;
    case lunary://Lunary
        r = 255-(255)* x/xmax;
        g =(255) * x/xmax;
        b = -10;
        break;
    case solary://Solary
        r = -10;
        g = 255-(255) * x/xmax;
        b = (255)* x/xmax;
        break;
    default:
        break;
    }
    p = r<<16|g<<8|b;
    return p;
}

COULEUR couleur_rgb(int r, int g, int b)
{
    return ((r%256)<<16) + ((g%256)<<8) + (b%256);
}

///================================================================================
///                                    DESSIN
///================================================================================

void fill_screen(COULEUR color)
{
    int i,j;
    for (i=0; i<WIDTH; i++)
        for (j=0; j<HEIGHT; j++) *((COULEUR *)screen->pixels + (HEIGHT-j-1) * WIDTH + i) = color;
    if (SDL_AFFICHE_AUTO) affiche_all();
}

int dans_ecran(int x, int y)
{
    if (x<0) return 0;
    if (x>=WIDTH) return 0;
    if (y<0) return 0;
    if (y>=HEIGHT) return 0;
    return 1;
}

#define add_pix(x,y,color)  if (dans_ecran((x),(y))) *((COULEUR *)screen->pixels + (HEIGHT-(y)-1) * WIDTH + (x)) = (color)

void draw_pixel(POINT p, COULEUR color)
{
    add_pix(p.x,p.y,color);
    if (SDL_AFFICHE_AUTO) affiche_all();
}

void draw_line(POINT p1, POINT p2, COULEUR color)
{
    int xmin, xmax;
    int ymin, ymax;
    int i,j;
    float a,b,ii,jj;

    if (p1.x < p2.x)
    {
        xmin=p1.x;
        xmax=p2.x;
    }
    else
    {
        xmin=p2.x;
        xmax=p1.x;
    }
    if (p1.y < p2.y)
    {
        ymin=p1.y;
        ymax=p2.y;
    }
    else
    {
        ymin=p2.y;
        ymax=p1.y;
    }

    if (xmin==xmax) for (j=ymin; j<=ymax; j++) add_pix(xmin,j,color);
    if (ymin==ymax) for (i=xmin; i<=xmax; i++) add_pix(i,ymin,color);


    // La variation la plus grande est en x
    if ((xmax-xmin >= ymax-ymin) && (ymax-ymin>0))
    {
        a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
        b = p1.y - a*p1.x;
        for (i=xmin; i<=xmax; i++)
        {
            jj = a*i+b;
            j = jj;
            if (((jj-j) > 0.5) && (j < HEIGHT-1)) j++;
            add_pix(i,j,color);
        }
    }

    // La variation la plus grande est en y
    if ((ymax-ymin > xmax-xmin) && (xmax-xmin>0))
    {
        a = (float)(p1.y-p2.y) / ((float)(p1.x-p2.x));
        b = p1.y - a*p1.x;
        for (j=ymin; j<=ymax; j++)
        {
            ii = (j-b)/a;
            i = ii;
            if (((ii-i) > 0.5) && (i < WIDTH-1)) i++;
            add_pix(i,j,color);
        }
    }
    if (SDL_AFFICHE_AUTO) affiche_all();
}

void draw_rectangle(POINT p1, POINT p2, COULEUR color)
{
    int xmin, xmax;
    int ymin, ymax;
    int i,j;

    if (p1.x < p2.x)
    {
        xmin=p1.x;
        xmax=p2.x;
    }
    else
    {
        xmin=p2.x;
        xmax=p1.x;
    }
    if (p1.y < p2.y)
    {
        ymin=p1.y;
        ymax=p2.y;
    }
    else
    {
        ymin=p2.y;
        ymax=p1.y;
    }

    for (i=xmin; i<=xmax; i++) add_pix(i,ymin,color);
    for (i=xmin; i<=xmax; i++) add_pix(i,ymax,color);

    for (j=ymin; j<=ymax; j++) add_pix(xmin,j,color);
    for (j=ymin; j<=ymax; j++) add_pix(xmax,j,color);
    if (SDL_AFFICHE_AUTO) affiche_all();
}

void draw_fill_rectangle(POINT p1, POINT p2, COULEUR color)
{
    int xmin, xmax;
    int ymin, ymax;
    int i,j;

    if (p1.x < p2.x)
    {
        xmin=p1.x;
        xmax=p2.x;
    }
    else
    {
        xmin=p2.x;
        xmax=p1.x;
    }
    if (p1.y < p2.y)
    {
        ymin=p1.y;
        ymax=p2.y;
    }
    else
    {
        ymin=p2.y;
        ymax=p1.y;
    }

    for (i=xmin; i<=xmax; i++) for (j=ymin; j<=ymax; j++) add_pix(i,j,color);
    if (SDL_AFFICHE_AUTO) affiche_all();
}

void draw_circle(POINT centre, int rayon, COULEUR color)
{
    POINT min, max;
    int i,j;
    float dx, dy, rr;

    min.x = centre.x - rayon;
    max.x = centre.x + rayon;
    min.y = centre.y - rayon;
    max.y = centre.y + rayon;
    rr = rayon*rayon;

    for (i=min.x; i<=max.x; i++)
    {
        dx = i - centre.x;
        dy = sqrt(rr - dx*dx);
        j = centre.y + dy;
        add_pix(i,j,color);
        j = centre.y - dy;
        add_pix(i,j,color);
    }
    for (j=min.y; j<=max.y; j++)
    {
        dy = j - centre.y;
        dx = sqrt(rr - dy*dy);
        i = centre.x + dx;
        add_pix(i,j,color);
        i = centre.x - dx;
        add_pix(i,j,color);
    }
    if (SDL_AFFICHE_AUTO) affiche_all();
}

void draw_fill_circle(POINT centre, int rayon, COULEUR color)
{
    POINT min, max;
    int i,j;
    float dx, dy, rr;

    min.x = centre.x - rayon;
    max.x = centre.x + rayon;
    min.y = centre.y - rayon;
    max.y = centre.y + rayon;

    rr = rayon*rayon;

    for (i=min.x; i<=max.x; i++)
    {
        dx = i - centre.x;
        for (j=min.y; j<=max.y; j++)
        {
            dy = j - centre.y;
            if (dx*dx + dy*dy <= rr) add_pix(i,j,color);
        }
    }
    if (SDL_AFFICHE_AUTO) affiche_all();
}

void draw_fill_ellipse(POINT F1, POINT F2, int r, COULEUR color)
{
    int i,j;
    int dx, fx;
    int dy, fy;
    float d, d1, d2;

    d = (F1.x-F2.x)*(F1.x-F2.x) + (F1.y-F2.y)*(F1.y-F2.y);
    d = sqrt(d);

    if (F1.x<F2.x)
    {
        dx = F1.x - d - r;
        fx = F2.x + d + r;
    }
    else
    {
        dx = F2.x - d - r;
        fx = F1.x + d + r;
    }
    if (F1.y<F2.y)
    {
        dy = F1.y - d - r;
        fy = F2.y + d + r;
    }
    else
    {
        dy = F2.y - d - r;
        fy = F1.y + d + r;
    }
    for (i=dx; i<=fx; i++)
        for (j=dy; j<=fy; j++)
            if (dans_ecran(i,j))
            {
                d1 = (i-F1.x)*(i-F1.x) + (j-F1.y)*(j-F1.y);
                d1 = sqrt(d1);
                d2 = (i-F2.x)*(i-F2.x) + (j-F2.y)*(j-F2.y);
                d2 = sqrt(d2);
                if (d1+d2 < d+r) add_pix(i,j,color);
            }
    if (SDL_AFFICHE_AUTO) affiche_all();
}

void draw_triangle(POINT p1, POINT p2, POINT p3, COULEUR color)
{
    draw_line(p1,p2,color);
    draw_line(p2,p3,color);
    draw_line(p3,p1,color);
}

int min3(int a, int b, int c)
{
    if ( (a<b) && (a<c) ) return a;
    if ( (b<a) && (b<c) ) return b;
    return c;
}

int max3(int a, int b, int c)
{
    if ( (a>b) && (a>c) ) return a;
    if ( (b>a) && (b>c) ) return b;
    return c;
}

void draw_fill_triangle(POINT p1, POINT p2, POINT p3, COULEUR color)
{
    float a12, b12, a23, b23, a31, b31;
    float s1, s2, s3;
    // La droite passant par les point pi et pj
    // a pour équation : y = aij x  +  bij
    a12 = (p1.y-p2.y)/(float)(p1.x-p2.x);
    b12 = p1.y - a12*p1.x;
    a23 = (p2.y-p3.y)/(float)(p2.x-p3.x);
    b23 = p2.y - a23*p2.x;
    a31 = (p3.y-p1.y)/(float)(p3.x-p1.x);
    b31 = p3.y - a31*p3.x;
    // Le signe de sk détermine de quel coté pk est de la droite [pi,pj]
    s3 = p3.y - (a12*p3.x + b12);
    s1 = p1.y - (a23*p1.x + b23);
    s2 = p2.y - (a31*p2.x + b31);

    int minx, maxx, miny, maxy;
    minx = min3(p1.x,p2.x,p3.x);
    maxx = max3(p1.x,p2.x,p3.x);
    miny = min3(p1.y,p2.y,p3.y);
    maxy = max3(p1.y,p2.y,p3.y);

    int i,j;
    int ok;
    for (i=minx; i<maxx; i++)
        for (j=miny; j<maxy; j++)
        {
            ok = 1;
            // On vérifie que le point (i,j) est du bon coté
            // des 3 droites du triangle
            if (s3 * (j - (a12*i + b12)) < 0) ok = 0;
            if (s1 * (j - (a23*i + b23)) < 0) ok = 0;
            if (s2 * (j - (a31*i + b31)) < 0) ok = 0;
            if (ok) add_pix(i,j,color);
        }
}




void chaos(void){
    int x=900,y=600;
    init_graphics(x,y,SDL_TRUE,"bifurcation vers le chaos");
    set_affiche_auto(SDL_FALSE);
    fill_screen(0);
    affiche_all();
    int i,j;
    float rmin=2.4;
    float rmax=4;
    float i2,r;
    POINT p;
    for(i=0; i<x; i++)
    {
        i2=0.000001;
        r=((float)i)/(x/(rmax-rmin))+rmin;
        if(r>4)r=4;
        printf("\n%d",x-i);
        //for(j=0;j<y;j++){i2 = i2*r*(1-i2);}//supression des premieres valeurs

        for(j=0; j<y; j++)
        {
            i2 = i2*r*(1-i2);
            p.x=i;
            p.y=(int)(y*(float)i2);
            draw_pixel(p,couleur_style(lunary,i,x));
        }
        affiche_all();//c bo
    }
    affiche_all();

    wait_escape();
}
