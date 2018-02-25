#include "graphics.h" //lib

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