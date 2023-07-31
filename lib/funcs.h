#include "pgm.h"

void copy_data(struct Image *,int,int, struct Image *);
struct Image filtro(struct Image);
unsigned char media(struct Image,int, int);
void alg1(const char *, const char *,int,int,int);
void alg2(const char*, const char*);
double correlacao_cruzada(unsigned char **, double **, int, int, int, int, int, int);
double media_data(struct Image);
