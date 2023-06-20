/* 
    Aluno : Lucas da Rocha Barbosa / Aldair Ryan Fernandes Mendes
    Matricula : *** / ***
    Avaliacao 04 : Trabalho Final
    04.505.23−2023.1 − Prof.Daniel Ferreira
    Compilador : GCC versão 11.3.0
 */

#include "pgm.h"

void copy_data(struct Image *,int,int, struct Image *);
struct Image filtro(struct Image);
unsigned char media(struct Image,int, int);
void alg1(char *, char *,int,int,int);
void alg2(char*,char*);
double correlacao_cruzada(unsigned char **, double **, int, int, int, int, int, int);
double media_data(struct Image);
