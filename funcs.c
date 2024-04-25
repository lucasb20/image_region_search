#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include "lib/funcs.h"
#include "lib/pgm.h"


void alg1(char *imagem, char *diretorio,int n,int width,int height){
    int k = 0;
    int i=0,j=0;
    
    struct Image *o = malloc(sizeof(struct Image));
    readPGMImage(o,imagem);

    struct Image o_filt = filtro(*o);
    struct Image *recortes = calloc(n,sizeof(struct Image));
    
    while(k<n){
        i = rand()%(o_filt.height-height);
        j = rand()%(o_filt.width-width);
        
        recortes[k].tipo = o->tipo;
        recortes[k].width = width;
        recortes[k].height = height;
        recortes[k].maxval = o_filt.maxval;

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

        copy_data(&o_filt,i,j,&recortes[k]);
        k++;
    }

    for(int z=0;z<n;z++){
        char nome[100];
        sprintf(nome,"%s/subimagem%d.pgm",diretorio,z);
        writePGMImage(recortes+z,nome);
    }
}

void alg2(char *imagem, char *diretorio){
    struct Image src;
    readPGMImage(&src,imagem);

    struct Image rec;

    int *p = NULL;
    double **v = NULL;
    double maior_corr = -INFINITY;

    char first_rec = 1;

    DIR *d;
    struct dirent *dir;

    p = calloc(2, sizeof(int));
    if (!p){
        printf("Falta de memória.\n");
        exit(1);
    }

    d = opendir(diretorio);

    FILE *file_ptr = fopen("alg2.txt","w");

    if(!(file_ptr)){
        puts("ERRO.");
        exit(1);
    }

    if(d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            
            maior_corr = -INFINITY;

            char nome[400];
            sprintf(nome,"%s/%s",diretorio,dir->d_name);
            readPGMImage(&rec,nome);

            if(first_rec){
                v = calloc(rec.height, sizeof(double *));
                if (!v){
                    printf("Falta de memória.\n");
                    exit(1);
                }
                for (int i = 0; i < rec.height; i++) {
                    v[i] = calloc(rec.width, sizeof(double));
                    if (!v[i]){
                    printf("Falta de memória.\n");
                    exit(1);
                    }
                }
                first_rec = 0;
            }

            double media_rec = media_data(rec);
            for (int a = 0; a < rec.height; a++) {
                for (int b = 0; b < rec.width; b++) {
                    v[a][b] = (double)rec.Data[a][b] / media_rec;
                }
            }

            for (int i = 0; i < src.height - rec.height + 1; i++) {
                for (int j = 0; j < src.width - rec.width + 1; j++) {
                    double corr = correlacao_cruzada(src.Data, v, src.height, src.width, rec.height, rec.width, i, j);
                    if (corr > maior_corr) {
                        maior_corr = corr;
                        p[0] = i;
                        p[1] = j;
                    }
                }
            }
            
            fprintf(file_ptr,"%s, %d, %d\n",dir->d_name,p[0],p[1]);
        }
        fclose(file_ptr);
        closedir(d);
    }
    else{
        printf("Erro ao abrir o diretório '%s'.\n",diretorio);
        exit(1);
    }
    
    for (int i = 0; i < rec.height; i++) {
        free(v[i]);
    }
    free(v);
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

    img.tipo = o.tipo;
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

    for (int a = 0; a < rec_height; a++) {
        for (int b = 0; b < rec_width; b++) {
            media_src += src[i+a][j+b];
            media_rec += rec[a][b];
        }
    }
    media_src /= (src_height * src_width);
    media_rec /= (rec_height * rec_width);

    for (int a = 0; a < rec_height; a++) {
        for (int b = 0; b < rec_width; b++) {
            desvio_padrao_src += pow(src[i+a][j+b] - media_src, 2);
            desvio_padrao_rec += pow(rec[a][b] - media_rec, 2);
        }
    }
    desvio_padrao_src = sqrt(desvio_padrao_src / (src_height * src_width));
    desvio_padrao_rec = sqrt(desvio_padrao_rec / (rec_height * rec_width));

    for (int a = 0; a < rec_height; a++) {
        for (int b = 0; b < rec_width; b++) {
            soma += ((src[i+a][j+b] - media_src) / desvio_padrao_src) * ((rec[a][b] - media_rec) / desvio_padrao_rec);
        }
    }

    return soma;
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