#include <stdio.h>
#include <stdlib.h>
#include "lib/pmg.h"

void readPGMImage(struct Image *img, char *filename) {

    FILE *fp;
    char ch;

    if (!(fp = fopen(filename, "r"))) {
        perror("Erro.");
        exit(1);
    }

    if ((ch = getc(fp)) != 'P') {
        puts("A imagem fornecida não está no formato pgm");
        exit(2);
    }

    img->tipo = getc(fp) - 48;

    fseek(fp, 1, SEEK_CUR);

    while ((ch = getc(fp)) == '#') {
        while ((ch = getc(fp)) != '\n');
    }

    fseek(fp, -1, SEEK_CUR);

    fscanf(fp, "%d %d", &img->width, &img->height);
    if (ferror(fp)) {
        perror(NULL);
        exit(3);
    }
    fscanf(fp, "%d", &img->maxval);
    fseek(fp, 1, SEEK_CUR);

    img->Data = (unsigned char **)malloc(img->height * sizeof(unsigned char *));
    for (int i = 0; i < img->height; i++) {
        img->Data[i] = (unsigned char *)malloc(img->width * sizeof(unsigned char));
    }

    switch (img->tipo) {
        case 2:
            puts("Lendo imagem PGM (dados em texto)");
            for (int i = 0; i < img->height; i++) {
                for (int j = 0; j < img->width; j++) {
                    fscanf(fp, "%hhu", &img->Data[i][j]);
                }
            }
            break;
        case 5:
            puts("Lendo imagem PGM (dados em binário)");
            for (int i = 0; i < img->height; i++) {
                fread(img->Data[i], sizeof(unsigned char), img->width, fp);
            }
            break;
        default:
            puts("Não está implementado");
    }

    fclose(fp);

}

void writePGMImage(struct Image *img, char *filename) {
    FILE *fp;
    char ch;

    if (!(fp = fopen(filename, "wb"))) {
        perror("Erro.");
        exit(1);
    }

    fprintf(fp, "%s\n", "P5");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "%d\n", 255);

    for (int i = 0; i < img->height; i++) {
        fwrite(img->Data[i], sizeof(unsigned char), img->width, fp);
    }

    fclose(fp);

}

void viewPGMImage(struct Image *img) {
    printf("Tipo: %d\n", img->tipo);
    printf("Dimensões: [%d %d]\n", img->width, img->height);
    printf("Max: %d\n", img->maxval);

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            printf("%3d ", img->Data[i][j]);
        }
        printf("\n");
    }
}
