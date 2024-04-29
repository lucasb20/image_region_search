#include "pgm.h"

void copy_data(struct Image *,int,int, struct Image *);
int kernel_mean(struct Image,int, int);
double media_data(struct Image);
unsigned uPowOf2(int);