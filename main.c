/* 
    Aluno : Lucas da Rocha Barbosa / Aldair Ryan Fernandes Mendes
    Matricula : *** / ***
    Avaliacao 04 : Trabalho Final
    04.505.23−2023.1 − Prof.Daniel Ferreira
    Compilador : GCC versão 11.3.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "lib/pgm.h"
#include "lib/funcs.h"

int main(int argc,char **argv){
    if(argc != 3){
        printf("Formato: %s <imagem> <directory>\n",*argv);
        exit(1);
    }

    char *imagem = *(argv+1);
    char *diretory = *(argv+2);

/*     int n;

    puts("Digite uma quantidade de recortes.");
    scanf("%d",&n);

    alg1(imagem,diretory,n,10,10);
 */
    alg2(imagem,diretory);

    return 0;
}