/* 
    Aluno : Lucas da Rocha Barbosa / Aldair Ryan Fernandes Mendes
    Matricula : *** / ***
    Avaliacao 04 : Trabalho Final
    04.505.23−2023.1 − Prof.Daniel Ferreira
    Compilador : GCC versão 11.3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lib/pgm.h"
#include "lib/funcs.h"

int main(int argc,char **argv){
    if(argc != 4){
        printf("Formato: %s <num_teste alg1(1)/alg2(2)> <imagem> <directory>\n",*argv);
        exit(1);
    }

    int op = atoi(*(argv+1));
    char *imagem = *(argv+2);
    char *diretory = *(argv+3);

    switch(op){
        case 1:
            int n;
            int w;
            int h;
            puts("Digite uma quantidade de subimagens.");
            scanf("%d",&n);
            puts("Digite uma largura.");
            scanf("%d",&w);
            puts("Digite uma altura.");
            scanf("%d",&h);

            alg1(imagem,diretory,n,w,h);
            break;

        case 2:
            clock_t begin = clock();
            alg2(imagem,diretory);
            clock_t end = clock();
            double tempo_total = (double)(end - begin)/CLOCKS_PER_SEC;
            printf("Tempo total: %lf\n",tempo_total);
            break;
    }

    return 0;
}