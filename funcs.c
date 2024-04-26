#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
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

        #ifdef DEBUG
        printf("%s, %d, %d\n", img_name, i, j);
        #endif

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

    int p[2];

    double *v = NULL;
    char first_img = 1;

    double maior_corr = -INFINITY;

    DIR *d;
    struct dirent *dir;

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

        maior_corr = -INFINITY;

        char name[400];
        sprintf(name,"%s/%s", diretorio, dir->d_name);
        readPGMImage(&sub_image, name);

        if(first_img){
            v = calloc(sub_image.height * sub_image.width, sizeof(double));
            if (!v) {
                puts("Falta de memória.");
                exit(1);
            }
            first_img = 0;
        }

        double sub_image_mean = media_data(sub_image);
        for(int i = 0; i < sub_image.height * sub_image.width; i++){
            v[i] = sub_image.Data[i] / sub_image_mean;
        }

        for (int i = 0; i < src.height - sub_image.height + 1; i++) {
            for (int j = 0; j < src.width - sub_image.width + 1; j++) {
                double corr = correlacao_cruzada(src.Data, v, src.height, src.width, sub_image.height, sub_image.width, i, j);
                if (corr > maior_corr) {
                    maior_corr = corr;
                    p[0] = i;
                    p[1] = j;
                }
            }
        }
        
        fprintf(file_ptr, "%s, %d, %d\n", dir->d_name, p[0], p[1]);
    }
    fclose(file_ptr);
    closedir(d);
    
    free(v);
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

double correlacao_cruzada(unsigned char *src, double *sub, int src_height, int src_width, int sub_height, int sub_width, int i, int j) {
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