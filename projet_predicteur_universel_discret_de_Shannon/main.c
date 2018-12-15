#include <stdio.h>
#include <stdlib.h>

//Nombre généré par un humain
char tab01[]={0,0,0,0,0,1,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,0,0,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0,1,0,1,1,1,1,0,0,1,1,1,66};

//aléatoire
//char tab01[1000]={1,0,0,0,1,0,1,0,1,1,0,1,0,0,1,1,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1,1,1,0,0,1,0,0,0,0,1,0,1,1,1,0,1,0,0,1,1,0,1,0,1,0,0,0,0,1,1,1,1,0,1,1,1,1,0,1,1,0,1,0,1,0,1,1,0,1,1,0,1,0,1,1,0,1,1,1,1,0,0,0,0,1,1,0,1,1,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,0,0,1,1,0,0,1,0,1,1,1,1,1,0,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,1,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,1,0,0,0,0,1,0,0,1,1,0,1,0,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,0,1,0,0,1,1,1,1,0,1,0,0,0,1,0,0,1,0,0,1,1,1,1,1,1,1,0,0,1,0,0,0,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,0,1,1,0,1,1,1,0,0,1,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,1,1,0,1,0,1,0,1,0,1,1,0,1,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1,0,1,1,0,1,0,1,1,1,1,0,1,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,1,1,1,0,1,0,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,1,1,0,1,0,0,1,1,1,0,1,1,1,0,1,0,1,0,0,0,0,0,0,1,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,1,0,0,1,0,1,0,0,0,1,1,1,0,1,0,1,1,0,1,1,0,0,0,1,1,1,0,0,1,1,0,0,1,0,0,0,0,1,1,1,1,0,0,0,1,0,1,1,1,1,1,0,0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,0,0,1,0,1,0,1,0,1,1,1,1,0,0,0,1,0,0,1,0,1,0,0,1,1,1,1,0,1,0,1,0,1,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,0,0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,0,1,1,0,0,1,0,1,0,0,0,1,0,1,1,1,0,0,0,1,0,0,1,0,1,1,0,1,0,0,0,1,1,0,1,0,1,1,0,0,1,1,1,1,0,1,0,0,0,0,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,0,1,0,1,0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,1,0,0,1,1,0,1,1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,1,0,1,0,1,1,0,0,0,1,0,1,0,0,0,1,0,0,0,0,1,0,1,1,1,1,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1,0,1,0,1,0,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1,1,0,1,0,0,1,0,0,0,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1,0,0,65};
char tabpolyteck[10]={0,1,0,1,1,1,0,1,1,0};
int nb_nombre_entre=0;

#define SIZETAB01 sizeof(tab01)


//retourne le nombre suivant de notre suite
char give_tab01(void){
    nb_nombre_entre ++;
    static int poss = -1;
    poss++;
    if(tab01[poss]!=66)return tab01[poss];
    else{
        printf("tab01 est vide !");
        exit(66);
    }
}

//Compare deux sequences
char sequence_egales(char * tab1,char * tab2,int size){
    int i;
    for(i=0;i<size;i++){
        if(tab1[i]!=tab2[i])return 0;
    }
    return 1;
}


//Ces constantes et variables caractérisent la suite
#define K_MAX 100
#define NB_POS_MAX 100
int data[K_MAX][NB_POS_MAX];//Les position des curseurs en fonction de k(la profondeur de la série)
int nb_position[K_MAX];//Le nombre de curseur par nombre de k du tableau data
int nb_k;

void calc_all_pos_1_k(char *tab01,int sizetab01,int k){
    int i;
    nb_position[k] = 0;
    for(i=sizetab01-k-1;i>=0;i--){
        if(sequence_egales(tab01+i,tab01+sizetab01-k,k)){
            data[k][nb_position[k]]=i+k-1;//-1 ?
            nb_position[k]++;
        }
    }
}

void calc_all_pos_all_k(char *tab01,int sizetab01){
    nb_k=0;
    do{
        nb_k++;
        //printf("\ntaille a calc : %d",*taille);
        calc_all_pos_1_k(tab01,sizetab01,nb_k);
        //printf("position en %d",*position);
    }while(nb_position[nb_k]!=0);//On incrémente k tant qu'i ya des solutions.
    //printf("\ntaille finale : %d",*taille);
}

void write_data(void){
    int k,p;
    for(k=0;k<nb_k+1;k++){
        printf("\n pos(k=%d) = {",k);
        for(p=0;p<nb_position[k];p++){
            printf("%d.",data[k][p]);
        }
        for(p=nb_position[k];p<10;p++){
            printf(" .");
        }
        printf("}");
    }
}

char calc_predictionV1(char * tab){
   
    return tab[data[nb_k-1][0]+1];
}
int main()
{
    /*
    //Etude minimaliste
    int i;
    printf("\n ");
    for(i=0;i<10;i++){printf(" %d",i);}
    printf("\n");
    for(i=0;i<10;i++){printf("|%i",tabpolyteck[i]);}


    calc_all_pos_all_k(tabpolyteck,10);
    write_data();
    int predicteur = calc_predictionV1(tabpolyteck);
    printf("\nnext val : %d",predicteur);
*/


    int i;
    //for(i=0;i<20;i++){printf("\nval : %d",give_tab01());}

    char chiffre_predit=-1;
    char chiffre_reel;
    int bon=0,faux=0;

    char buffer[SIZETAB01]={1};//Les nombres auquel l'algorithme à acces
    buffer[0]=give_tab01();//On donne le premier bit à notre buffer pour ne pas fixer aléatoirement le premier bit

    for(i=1;i<SIZETAB01;i++){
        //saisis  nombre :
        chiffre_reel=give_tab01();


        // on prédit le nombre suivant : (Pour ce faire, on determine tous les curseurs pour toutes les profondeurs)
        calc_all_pos_all_k(buffer,i);
        chiffre_predit = calc_predictionV1(buffer);

        if(chiffre_reel==chiffre_predit)bon++;else faux++;


        printf(" \n predit : %d\t| reel : %d\t| gen : %d\t| win : %.3f%%",chiffre_predit,chiffre_reel,i,(float)bon*100/(i));


        buffer[i]=chiffre_reel;//On ajoute le chiffre reel dans le buffer


        //write_data();
    }
    printf("\nBon : %d | Faux : %d | reusite : %f%%",bon,faux,(float)bon*100/i);





    return 0;
}
