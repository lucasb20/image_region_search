#include "lib/pmg.h"
#include "lib/funcs.h"

//Remover isso aqui depois.
/* #include <stdio.h>
#include <stdlib.h>
#include <time.h>
 */
int main(int argc,char **argv){
    //Apenas depuração.
/* 
    srand(time(NULL));

    struct Image foto,foto_f;
    foto.tipo = 5;
    foto.width = 1000;
    foto.height = 1000;
    foto.maxval = 255;
    foto.Data = (unsigned char **)malloc(foto.height*sizeof(unsigned char *));
    for(int i=0;i<foto.height;i++)foto.Data[i]=(unsigned char *)calloc(foto.width,sizeof(unsigned char));

    for(int i=0;i<foto.height;i++)for(int j=0;j<foto.width;j++)foto.Data[i][j]=rand()%100+1;

    foto_f = filtro(foto);

    struct Image *recorte = calloc(1,sizeof(struct Image));

    recorte = alg1(&foto_f,1,200,200);

    printf("Recorte:\n");
    viewPGMImage(*recorte);

    int *p = (int *)calloc(2,sizeof(int));

    p = alg2(foto,*recorte);

    printf("Algoritmo encontrou: [%d,%d]\n",*p,*(p+1));
 */
    return 0;
}