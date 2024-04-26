#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/pgm.h"
#include "lib/funcs.h"

int main(int argc,char **argv){
    if(argc != 4){
        printf("Formato: %s <num_teste alg1 (1) ou alg2 (2)> <imagem> <directory>\n",*argv);
        exit(1);
    }

    srand(time(0));

    int op = atoi(*(argv+1));
    char *imagem = *(argv+2);
    char *diretory = *(argv+3);

    int n;
    int w;
    int h;

    clock_t begin,end;

    switch(op){
        case 1:
            puts("Digite uma quantidade de subimagens.");
            scanf("%d",&n);
            puts("Digite uma largura.");
            scanf("%d",&w);
            puts("Digite uma altura.");
            scanf("%d",&h);

            alg1(imagem,diretory,n,w,h);
            break;

        case 2:
            begin = clock();
            alg2(imagem,diretory);
            end = clock();
            double tempo_total = (double)(end - begin)/CLOCKS_PER_SEC;
            printf("Tempo total: %lfs\n(%lf min)",tempo_total,tempo_total/60);
            break;
    }

    return 0;
}