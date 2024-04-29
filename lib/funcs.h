#include "pgm.h"

void alg1(char *, char *,int,int,int);
void alg2(char*,char*);
struct Image filtro(struct Image);
double correlacao_cruzada(unsigned char *, double *, int, int, int, int, int, int);
void alg_cross_corr(struct Image,struct Image,int*);