#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define name_file "bunny"
#define flux_in_file 1

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

typedef struct
{
    float x,y,z;
} POINT;

typedef struct
{
    POINT direction_normale,a,b,c;
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

void print_cord_point(POINT aff)
{
    printf("\t[%f:%f:%f]\n",aff.x,aff.y,aff.z);
}
void print_cord_triangle(TRIANGLE_IN_FILE tr)
{
    printf("DN");
    print_cord_point(tr.direction_normale);
    printf("A");
    print_cord_point(tr.a);
    printf("B");
    print_cord_point(tr.b);
    printf("C");
    print_cord_point(tr.c);
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

int main()
{
    char * input_file = set_input_file(name_file,".stl");
    printf("Input file : %s\n",input_file);
#if flux_in_file
    char * output_file = set_output_file(name_file);
    printf("Output file : %s\n",output_file);
    fclose(stdout);
    fopen(output_file,"w");
#endif // flux_in_file


    //Main
    int nb_triangle;
    TRIANGLE_IN_FILE *tr;
    tr = read_STL(input_file,&nb_triangle);


    int i;
    for(i=0; i<nb_triangle; i++){
        printf("========================================================\n");
        printf("Triangle %d : \n",i+1);
        print_cord_triangle(tr[i]);
    }

    free(tr);//ca soulage :)

    return 0;
}
