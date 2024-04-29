#include "lib/pgm.h"

double media_data(struct Image obj){
    double media = 0;
    for(int i = 0; i < obj.height * obj.width; i++){
        media += obj.Data[i];
    }
    media /= obj.height*obj.width;
    return media;
}

int kernel_mean(struct Image obj, int x, int y){
    int m = 0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            m += obj.Data[(x + i) * obj.width + (y + j)];
        }    
    }

    m /= 9;

    return m;
}

void copy_data(struct Image *src, int x, int y, struct Image *des){
    int a = x, b = y;
    for (int i = 0; i < des->height; i++) {
        for (int j = 0; j < des->width; j++) {
            des->Data[i * des->width + j] = src->Data[a * src->width + b];
            b++;
        }
        b=y;
        a++;
    }
}

unsigned uPowOf2(int x){
    return x*x;
}