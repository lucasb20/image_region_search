#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include "lib/funcs.h"
#include "lib/pgm.h"
#include "lib/utils.h"

void alg1(char *img_name, char *dir, int n, int width, int height){
    int i = 0, j = 0;
    
    struct Image *img = malloc(sizeof(struct Image));
    readPGMImage(img, img_name);

    struct Image img_filt = filtro(*img);
    struct Image *sub_images = calloc(n, sizeof(struct Image));
    
    for(int k = 0; k < n; k++){
        i = rand()%(img_filt.height - height + 1);
        j = rand()%(img_filt.width - width + 1);

        printf("subimage%d.pgm, %d, %d\n", k, i, j);

        sub_images[k].type = img_filt.type;
        sub_images[k].width = width;
        sub_images[k].height = height;
        sub_images[k].maxval = img_filt.maxval;

        if (!(sub_images[k].Data = (unsigned char *) calloc(width * height, sizeof(unsigned char))))
        {
            printf("Falta de memória.\n");
            exit(1);
        }

        copy_data(&img_filt, i, j, &sub_images[k]);
    }

    for(int k = 0; k < n; k++){
        char name[100];
        sprintf(name, "%s/subimage%d.pgm", dir, k);
        writePGMImage(sub_images+k, name);
    }

    free(img);

    for(int k = 0; k < n; k++){
        free(sub_images[k].Data);
    }

    free(sub_images);
}

void alg2(char *imagem, char *diretorio){
    struct Image src;
    readPGMImage(&src, imagem);

    struct Image sub_image;

    DIR *d;
    struct dirent *dir;

    int p[2] = {0};

    d = opendir(diretorio);

    FILE *file_ptr = fopen("alg2.txt","w");

    if(!(file_ptr)){
        puts("Erro ao abrir criar o arquivo.");
        exit(1);
    }

    if(!d){
        printf("Erro ao abrir o diretório '%s'.\n", diretorio);
        exit(1);
    }

    while ((dir = readdir(d)) != NULL){
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }

        char name[400];
        sprintf(name,"%s/%s", diretorio, dir->d_name);
        readPGMImage(&sub_image, name);

        alg_MAE(src, sub_image, p);
        
        fprintf(file_ptr, "%s, %d, %d\n", dir->d_name, p[0], p[1]);
    }
    fclose(file_ptr);
    closedir(d);
}

void alg_MAE(struct Image src, struct Image sub, int* p){
    unsigned menor_MAE = UINT_MAX;

    for (int i = 0; i < src.height - sub.height + 1; i++) {
      for (int j = 0; j < src.width - sub.width + 1; j++) {
        unsigned mae = 0;
        for (int a = 0; a < sub.height; a++) {
          for (int b = 0; b < sub.width; b++) {
            int index = (i + a) * src.width + (j + b);
            mae += abs(src.Data[index] - sub.Data[a * sub.width + b]);
          }
        }
        if (mae < menor_MAE) {
            menor_MAE = mae;
            p[0] = i;
            p[1] = j;
        }
      }
    }
}

void alg_cross_corr(struct Image src, struct Image sub, int* p){
    double maior_corr = -INFINITY;

    for (int i = 0; i < src.height - sub.height + 1; i++) {
            for (int j = 0; j < src.width - sub.width + 1; j++) {
                double corr = correlacao_cruzada(src.Data, sub.Data, src.height, src.width, sub.height, sub.width, i, j);
                if (corr > maior_corr) {
                    maior_corr = corr;
                    p[0] = i;
                    p[1] = j;
                }
                #ifdef OPTIMIZER
                else if(maior_corr > 0)
                {
                    j += (maior_corr - corr)*(src.width * 0.1)/maior_corr;
                }
                #endif
            }
    }
}

void alg_MSE(struct Image src, struct Image sub, int* p){
    unsigned menor_mse = UINT_MAX;

    #ifdef OPTIMIZER
    double ratio = (1 - sub.width/((double) src.width));
    int sub_width = sub.width * ratio;
    int sub_height = sub.height * ratio;
    #else
    int sub_width = sub.width;
    int sub_height = sub.height;
    #endif

    for (int i = 0; i < src.height - sub.height + 1; i++) {
      for (int j = 0; j < src.width - sub.width + 1; j++) {
        unsigned mse = 0;
        for (int a = 0; a < sub_height; a++) {
          for (int b = 0; b < sub_width; b++) {
            int index = (i + a) * src.width + (j + b);
            mse += uPowOf2(src.Data[index] - sub.Data[a * sub.width + b]);
          }
        }
        if (mse < menor_mse) {
            menor_mse = mse;
            p[0] = i;
            p[1] = j;
        }

        #ifdef OPTIMIZER
        else
        {
            j += sqrt(mse)/(sub.width*sub.height)*(sub.width*0.1);
        }
        #endif
      }
    }
}

struct Image filtro(struct Image obj){
    struct Image img, aux;

    img.type = obj.type;
    img.height = obj.height;
    img.width = obj.width;
    img.maxval = obj.maxval;
    if (!(img.Data = (unsigned char *) calloc(img.width * img.height, sizeof(unsigned char)))){
        printf("Falta de memória.\n");
        exit(1);
    }

    for(int i = 0; i < img.height * img.width; i++){
        img.Data[i] = obj.Data[i];
    }

    aux.height = obj.height + 2;
    aux.width = obj.width + 2;
    if (!(aux.Data = (unsigned char *) calloc(aux.width * aux.height, sizeof(unsigned char)))){
        printf("Falta de memória.\n");
        exit(1);
    }

    for(int i = 0; i < img.height; i++){
        for(int j = 0; j < img.width; j++){
            aux.Data[(i+1) * aux.width + (j+1)] = img.Data[i * img.width + j];
        }
    }

    for(int i = 0; i < img.height; i++){
        for(int j = 0; j < img.width; j++){
            img.Data[i*img.width + j] = kernel_mean(aux, i+1, j+1);
        }
    }

    free(aux.Data);

    return img;
}

double correlacao_cruzada(unsigned char *src, unsigned char *sub, int src_height, int src_width, int sub_height, int sub_width, int i, int j) {
    double sum = 0;
    double src_mean = 0;
    double sub_mean = 0;
    double src_sd = 0;
    double sub_sd = 0;

    for (int a = 0; a < sub_height; a++) {
        for (int b = 0; b < sub_width; b++) {
            src_mean += src[(i+a)*src_width + (j+b)];
            sub_mean += sub[a*sub_width + b];
        }
    }
    src_mean /= (src_height * src_width);
    sub_mean /= (sub_height * sub_width);

    for (int a = 0; a < sub_height; a++) {
        for (int b = 0; b < sub_width; b++) {
            src_sd += pow(src[(i+a)*src_width + (j+b)] - src_mean, 2);
            sub_sd += pow(sub[a*sub_width + b] - sub_mean, 2);
        }
    }
    src_sd = sqrt(src_sd / (src_height * src_width));
    sub_sd = sqrt(sub_sd / (sub_height * sub_width));

    for (int a = 0; a < sub_height; a++) {
        for (int b = 0; b < sub_width; b++) {
            sum += ((src[(i+a)*src_width + (j+b)] - src_mean) / src_sd) * ((sub[a*sub_width + b] - sub_mean) / sub_sd);
        }
    }

    return sum;
}