#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/funcs.h"
#include "lib/pmg.h"

//Algoritmo 1: Fazer N recortes aleatórios de tamanho L*M com o filtro média.
//Falta salvar em um arquivo.
struct Image *alg1(struct Image *o,int n,int width,int height){
    int k = 0;
    int i=0,j=0;
    struct Image *recortes = calloc(n,sizeof(struct Image));
    while(k<n){
        i = (o->height == height)?0:rand()%(o->height-height);
        j = (o->width == width)?0:rand()%(o->width-width);
        recortes[k].width = width;
        recortes[k].height = height;
        recortes[k].maxval = o->maxval;
        if(!(recortes[k].Data = (unsigned char **)calloc(recortes[k].height,sizeof(unsigned char*)))){
            printf("Faltou memória.");
            exit(1);
        }
        for(int z = 0; z<recortes[k].height; z++){
            if(!(recortes[k].Data[z] = (unsigned char *)calloc(recortes[k].width,sizeof(unsigned char)))){
                printf("Faltou memória.");
                exit(1);      
            }
        }
        copy_data(o,i,j,&recortes[k]);
        k++;
        //Debugg
        printf("pos [%d,%d].\n",i,j);
    }
    return recortes;
}

void copy_data(struct Image *src,int x,int y, struct Image *des){
    int a=x,b=y;
    for(int i=0;i<des->height;i++){
        for(int j=0;j<des->width;j++){
            des->Data[i][j]=src->Data[a][b];
            b++;
        }
        b=y;
        a++;
    }
}

struct Image filtro(struct Image o){
    struct Image img,aux;
    img.height = o.height;
    img.width = o.width;
    img.maxval = o.maxval;
    if(!(img.Data = (unsigned char **)calloc(img.height,sizeof(unsigned char*)))){
            printf("Faltou memória.");
            exit(1);
    }
    for(int i = 0; i<img.height; i++){
        if(!(img.Data[i] = (unsigned char *)calloc(img.width,sizeof(unsigned char)))){
            printf("Faltou memória.");
            exit(1);
        }
    }
    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
            img.Data[i][j] = o.Data[i][j];
        }
    }

    aux.height = o.height + 2;
    aux.width = o.height + 2;
    aux.maxval = o.maxval;
    
    if(!(aux.Data = (unsigned char **)calloc(aux.height,sizeof(unsigned char*)))){
            printf("Faltou memória.");
            exit(1);
    }
    for(int i = 0; i<aux.height; i++){
        if(!(aux.Data[i] = (unsigned char *)calloc(aux.width,sizeof(unsigned char)))){
            printf("Faltou memória.");
            exit(1);
        }
    }

    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
            aux.Data[i+1][j+1] = img.Data[i][j];
        }
    }

    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
            img.Data[i][j] = media(aux,i+1,j+1);
        }
    }

    return img;
}

unsigned char media(struct Image o,int x, int y){
    unsigned char m;

    m = (o.Data[x][y]+o.Data[x][y+1]+o.Data[x][y-1]+o.Data[x-1][y]+o.Data[x-1][y+1]+o.Data[x-1][y-1]+o.Data[x+1][y]+o.Data[x+1][y+1]+o.Data[x+1][y-1])/9;

    return m;
}

double correlacao_cruzada(unsigned char **src, double **rec, int src_height, int src_width, int rec_height, int rec_width, int i, int j) {
    double soma = 0.0;
    double media_src = 0.0;
    double media_rec = 0.0;
    double desvio_padrao_src = 0.0;
    double desvio_padrao_rec = 0.0;

    // Calcula a média das matrizes src e rec
    for (int a = 0; a < rec_height; a++) {
        for (int b = 0; b < rec_width; b++) {
            media_src += src[i+a][j+b];
            media_rec += rec[a][b];
        }
    }
    media_src /= (src_height * src_width);
    media_rec /= (rec_height * rec_width);

    // Calcula o desvio padrão das matrizes src e rec
    for (int a = 0; a < rec_height; a++) {
        for (int b = 0; b < rec_width; b++) {
            desvio_padrao_src += pow(src[i+a][j+b] - media_src, 2);
            desvio_padrao_rec += pow(rec[a][b] - media_rec, 2);
        }
    }
    desvio_padrao_src = sqrt(desvio_padrao_src / (src_height * src_width));
    desvio_padrao_rec = sqrt(desvio_padrao_rec / (rec_height * rec_width));

    // Calcula a correlação cruzada entre as matrizes src e rec
    for (int a = 0; a < rec_height; a++) {
        for (int b = 0; b < rec_width; b++) {
            soma += ((src[i+a][j+b] - media_src) / desvio_padrao_src) * ((rec[a][b] - media_rec) / desvio_padrao_rec);
        }
    }

    return soma;
}

//Algoritmo 2: Procurar na imagem a posição de onde foi retirada e um ponteiro para ela.
//Tem que retorna um vetor v = [x,y].
int *alg2(struct Image src, struct Image rec) {
    int *p = NULL;
    double **v = NULL;
    double maior_corr = -INFINITY;

    // Aloca memória para o array de inteiros p e a matriz v
    p = calloc(2, sizeof(int));
    if (!p) exit(1);
    v = calloc(rec.height, sizeof(double *));
    if (!v) exit(1);
    for (int i = 0; i < rec.height; i++) {
        v[i] = calloc(rec.width, sizeof(double));
        if (!v[i]) exit(1);
    }

    // Normaliza a matriz rec
    double media_rec = media_data(rec);
    for (int a = 0; a < rec.height; a++) {
        for (int b = 0; b < rec.width; b++) {
            v[a][b] = (double)rec.Data[a][b] / media_rec;
        }
    }

    // Percorre todas as possíveis posições na matriz src onde a matriz rec pode ser colocada
    for (int i = 0; i < src.height - rec.height + 1; i++) {
        for (int j = 0; j < src.width - rec.width + 1; j++) {
            // Calcula a correlação cruzada entre as matrizes src e rec na posição (i, j)
            double corr = correlacao_cruzada(src.Data, v, src.height, src.width, rec.height, rec.width, i, j);
            if (corr > maior_corr) {
                maior_corr = corr;
                p[0] = i;
                p[1] = j;
            }
        }
        printf("Demora: %d/%d\n",i,src.height - rec.height);
    }

    // Libera a memória alocada para a matriz v
    for (int i = 0; i < rec.height; i++) {
        free(v[i]);
    }
    free(v);

    return p;
}

double media_data(struct Image o){
    double media=0;
    for(int i=0;i<o.height;i++){
        for(int j=0;j<o.width;j++){
            media+=o.Data[i][j];
        }
    }
    media /= o.height*o.width;
    return media;
}