#include "lib/pmg.h"
#include "lib/funcs.h"
#include "lib/LerDire.h"

//Remover isso aqui depois.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc,char **argv){
    //Apenas depuração.

    srand(time(NULL));

    struct Image foto;
    foto.tipo = 5;
    foto.width = 20;
    foto.height = 20;
    foto.maxval = 255;
    foto.Data = (unsigned char **)malloc(foto.height*sizeof(unsigned char *));
    for(int i=0;i<foto.height;i++)foto.Data[i]=(unsigned char *)calloc(foto.width,sizeof(unsigned char));

    for(int i=0;i<foto.height;i++)for(int j=0;j<foto.width;j++)foto.Data[i][j]=rand()%100+1;

    printf("Foto:\n");
    viewPGMImage(foto);

    int n=3;

    struct Image *recorte = calloc(n,sizeof(struct Image));
    recorte = alg1(&foto,n,5,5);

    for(int i=0;i<n;i++){
        printf("Recorte n°%d\n",i);
        viewPGMImage(*(recorte + i));
    }


    int *p = (int *)calloc(2,sizeof(int));

    p = alg2(foto,*recorte);

    printf("Algoritmo encontrou: [%d,%d]\n",*p,*(p+1));

    return 0;
}