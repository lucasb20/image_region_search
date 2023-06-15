#include "pmg.h"

void copy_data(struct Image *,int,int, struct Image *);
struct Image filtro(struct Image o);
unsigned char media(struct Image o,int x, int y);
struct Image *alg1(struct Image *,int,int,int);
int *alg2(struct Image src,struct Image rec);
double correlacao_cruzada(unsigned char **src, double **rec, int src_height, int src_width, int rec_height, int rec_width, int i, int j);
double media_data(struct Image o);
