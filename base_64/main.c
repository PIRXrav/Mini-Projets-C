#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const unsigned char e_64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

static char reverseTab(char b64) {
    unsigned int i;
    for(i=0; i<strlen(e_64); i++) {
        if(b64==e_64[i])
            return (char)i;
        else if(b64=='=')
            return 0;
    }
    return 0;
}

#define TAILLE_MAX 100 //'\0' compris
char * setSource(FILE * stream) {
    char * source = malloc(sizeof(char)*TAILLE_MAX);
    if(!source)exit(EXIT_FAILURE);

    int i;
    for(i=0; i<TAILLE_MAX && source[i-1]!='\n'; i++) {
        source[i]=(char)fgetc(stream);
    }
    source[i-1]='\0';
    return source;
}

char * encode64(const char *str255) {
    size_t taille = strlen(str255);
    size_t SIZE_BUF_64 = (((taille-1)/3)+1)*4;//Dimention exacte du tableau en base 64
    char * str64 = calloc(SIZE_BUF_64+1,sizeof(char));//+1 Pour '/0'
    if(!str64)exit(EXIT_FAILURE);

    size_t curseursource=0;
    size_t curseurdest=0;
    size_t i;
    int donnesource;
    size_t octetmanquant = (3-taille%3)%3;
    while(curseursource<taille) {
        donnesource=0;
        for(i=0; i<=2; i++) {
            donnesource |= str255[curseursource]<<8*(2-i);
            curseursource++;
        }
        for(i=0; i<=3; i++) {
            str64[curseurdest]= curseursource<taille || i<=3-octetmanquant? e_64[(donnesource&(0b111111<<(6*(3-i))))>>(6*(3-i))] : '=';
            curseurdest++;
        }
    }
    return str64;
}

char * decode64(const char *str64) {
    size_t taille = strlen(str64);
    size_t SIZE_BUF_255 = strlen(str64)*3/4;//Dimention +/-exacte du tableau en base 256 //C'est forcement un multiple de 4
    char * str255 = calloc(SIZE_BUF_255+1,sizeof(char));//+1 Pour '/0'
    if(!str255)exit(EXIT_FAILURE);

    size_t curseursource=0;
    size_t curseurdest=0;
    size_t i;
    int donnesource;
    while(curseursource<taille) {
        donnesource = 0;
        for(i=0; i<=3; i++) {
            donnesource |= reverseTab(str64[curseursource])<<6*(3-i);
            curseursource++;
        }
        for(i=0; i<=2; i++) {
            str255[curseurdest]= (char)((donnesource&(0b11111111<<(8*(2-i))))>>(8*(2-i)));
            curseurdest++;
        }
    }
    return str255;
}


int main(void) {
    const char * strhuman = /*setSource(stdin);*/ "Hello world!";
    printf("chaine ini \t: %s\n",strhuman);
    char * str64 = encode64(strhuman);
    printf("chaine 64 \t: %s\n",str64);
    char * str255 = decode64(str64);
    printf("chaine 255 \t: %s\n",str255);

    free(str64);
    free(str255);

    return 0;
}
