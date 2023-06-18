/* 
    Aluno : Lucas da Rocha Barbosa / Aldair Ryan Fernandes Mendes
    Matricula : *** / ***
    Avaliacao 04 : Trabalho Final
    04.505.23−2023.1 − Prof.Daniel Ferreira
    Compilador : GCC versão 11.3.0
 */

#include "lib/pmg.h"
#include "lib/funcs.h"
#include "lib/LerDire.h"

//Remover isso aqui depois.
/* #include <stdio.h>
#include <stdlib.h>
#include <time.h> */

int main(int argc,char **argv){
    //Apenas depuração.
/* 
    srand(time(NULL));

    struct Image foto;
    foto.tipo = 5;
    foto.width = 100;
    foto.height = 100;
    foto.maxval = 255;
    foto.Data = (unsigned char **)malloc(foto.height*sizeof(unsigned char *));
    for(int i=0;i<foto.height;i++)foto.Data[i]=(unsigned char *)calloc(foto.width,sizeof(unsigned char));
    for(int i=0;i<foto.height;i++)for(int j=0;j<foto.width;j++)foto.Data[i][j]=rand()%100+1;

    printf("Foto:\n");
    viewPGMImage(foto);

    //alg1 sendo usado, n é o número de recortes
    int n=1;
    struct Image *recorte = calloc(n,sizeof(struct Image));
    recorte = alg1(&foto,n,7,3);


    printf("Recorte:\n");
    viewPGMImage(*recorte);
    
    //alg2 sendo usado, p é a posição [x,y] do recorte.
    int *p = malloc(2*sizeof(int));
    p = alg2(foto,*recorte);
    printf("Algoritmo encontrou: [%d,%d]\n",*p,*(p+1));
 */   

 return 0;
}