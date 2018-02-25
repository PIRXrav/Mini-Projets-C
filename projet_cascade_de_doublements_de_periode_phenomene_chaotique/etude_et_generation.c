/**

https://fr.wikipedia.org/wiki/Th%C3%A9orie_du_chaos

=== Cascade de doublements de période ===

Soit : x_{n+1}  = r * x_n * (1 - x_n)

où n = 0, 1, …'' dénote le temps discret, x l'unique variable dynamique, et 0 < r < 4 un paramètre.

Lorsque le paramètre r devient supérieur à quatre, l'application sort de l'intervalle [0, 1].
La dynamique de cette application présente un comportement très différent selon la valeur du paramètre r :
* Pour 0 < r < 3 , le système possède un point fixe attractif, qui devient instable lorsque r = 3
* Pour 3 < r < 3.57..., l'application possède un attracteur qui est une orbite périodique, de période 2^n où n est un entier qui tend vers l'infini lorsque r 3.57…
* Lorsque r = 3.57..., l'application possède un attracteur chaotique fractal
* Le cas r = 4 À noter qu'on peut dans ce cas précis établir l'expression exacte de la mesure invariante

Lorsque le paramètre r augmente, on obtient donc une succession de bifurcations entre les comportements périodiques et le chaos :
https://upload.wikimedia.org/wikipedia/commons/7/7d/LogisticMap_BifurcationDiagram.png

Objectif :
    1) Etude
    2) Génération de "monde" à partir d'une clée (xini) qui sont totalement différents en cas de variation de cette clée


*/


#include <stdio.h>
#include <stdlib.h>


long double xnplus1(long double xn,long double r){///Cascade de doublements de période
    return (r * xn * (1-xn));
}

char signal_binaire(long double xn){
    /**on convertie notre suite en signal binaire
    on utilisera 5 bit de ce signal pour former une lettre*/
    return xn>0.5?1:0;
}

#if decouverte
void bifurcation_vers_le_chaos(void){//afficla la limite en converge de x(n+1)= rxn(xn-1) pour r variable
    ///Fonction inutile !
    ///phase de découverte

    ///Quand r > 3 il y à une bifurctation

    //il faudrais modéliser la courbe obtenue pour obtenir attracteur=f(r)
    //Objectif : Faire tendre notre varible x vers un nombre

    ///définition du nombre d'itération à réaliser
    #define converge 10002
    /// définition de R [inir:pasr:finr]
    #define inir 0.
    #define pasr 0.05
    #define finr 3
    float img;
    ///r = 2 -> attracteur 0.5
    float x=0.5,r=0,i;
    printf("Paramètre initiaux :");
    printf("\n nb valeur convergence : %d",converge);
    printf("\n========================= converge vers ================================= R ===");
    for(r=inir;r<=finr;r+=pasr){
       x=0.9;//position initiale
        ///calcul de la limite de la suite
        for(i=0;i<converge;i++){
            x=xnplus1(x,r);
            //printf("\n%f",x);
        }
        ///Affichage
        printf("\n");
        for(img=0;img<1-0.3;img+=0.01){
            if (x>img)putchar(219);else putchar('|');
        }
        printf("| 0.%d",(int)(r*1000));
    }

}
#endif

void calcul_signal_binaire(char *signal,int nb,float r,long double x){
    int iteration;

    /*
    for(iteration=0;iteration<100;iteration++){//on est hors zone chaotique
        x=xnplus1(x,r);
    }
    */
    for(iteration=1;iteration<nb;iteration++){
        x=xnplus1(x,r);
        //x prend une valeur entre 0 et 1
        signal[iteration-1] = signal_binaire(x);
        //signal_binaire(x) prend la valeur 1 ou 0
        //On peut donc la ramener a une valeur entre 0 et 27 (inutile)
        //printf("\n%d",(int)(x*27));
    }
}

void ecriture_fichier_char_signal_binaire(char * signal,int nb){
    FILE* fichier = NULL;
    fichier = fopen("texte.txt", "w");
    char c;
    int iteration;
    for(iteration=1;iteration<nb;iteration++){
        //Conversion en Lettres (5bit -> 1 lettre)
        if (!(iteration%5)){//toues les 5 bit recu :
            c=((signal[iteration-1]<<0)|(signal[iteration-2]<<1)|(signal[iteration-3]<<2)|(signal[iteration-4]<<3)|(signal[iteration-5]<<4))*26/32+65;
            if (c==91)c = ' ';
            printf("%c",c);
            fprintf(fichier,"%c",c);
        }
    }
    fclose(fichier);
}

int main()
{

    ///================= Condition initiale ========================
        /**une faible variation de xini engendre un
           résultat totalement different*/

        long double xini=0.200004;
        float const r = 4; // zone chaotique
        int const nb = 100000;//le nombre de valeur réalisé

    ///=============================================================


    char signal[nb];//les états du "signal binaire"en fonction des itératinons
    calcul_signal_binaire(signal,nb,r,xini);
    ecriture_fichier_char_signal_binaire(signal,nb);

    return 0;
}
