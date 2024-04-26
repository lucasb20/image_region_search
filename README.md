# Image Region Search

## Visão Geral

Projeto que foi iniciado como trabalho final da disciplina de Laboratório de Programação, lecionada pelo professor Daniel Ferreira.

O projeto inclui dois algoritmos principais: O alg1 salva subimagens filtradas a partir de uma imagem. O alg2 realiza o processo inverso do algoritmo anterior, ele localiza na imagem a posição de uma subimagem filtrada, por meio do algoritmo de correlação cruzada.

## Arquivos principais do projeto

Obs: Todos os códigos foram reduzidos para resumir melhor.

## PGM

Funções para operar com arquivos de extensão pgm.
  
- readPGMImage: Lê um arquivo .pgm e forma uma estrutura Image a partir dela.

```c
void readPGMImage(struct Image *img, char *filename) {
    img->type = getc(fp);

    while ((ch = getc(fp)) == '#') {
        while ((ch = getc(fp)) != '\n');
    }

    fscanf(fp, "%d %d", &img->width, &img->height);
    fscanf(fp, "%hhu", &img->maxval);

    img->Data = (unsigned char *)malloc(img->width * img->height * sizeof(unsigned char));
    if (img->Data == NULL) {
        perror("Erro ao alocar memória.");
        exit(4);
    }

    switch (img->type) {
        case '2':
            puts("Lendo imagem PGM (dados em texto)");
            for (int i = 0; i < img->width * img->height; i++) {
                fscanf(fp, "%hhu", &img->Data[i]);
            }
            break;
        case '5':
            puts("Lendo imagem PGM (dados em binário)");
            fread(img->Data, sizeof(unsigned char), img->width * img->height, fp);
            break;
    }
}
```

- writePGMImage: Cria um arquivo .pgm a partir de uma estrutura Image.

```c
void writePGMImage(struct Image *img, char *filename) {
    fprintf(fp, "%s\n", "P5");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "%hhu\n", img->maxval);

    fwrite(img->Data, sizeof(unsigned char), img->width * img->height, fp);
}
```

- viewPGMImage: Imprime informações de uma estrutura image, inclusive a matriz de pixels.

```c
void viewPGMImage(struct Image *img) {
    printf("Tipo: P%c\n", img->type);
    printf("Dimensões: [%d %d]\n", img->width, img->height);
    printf("Max: %hhu\n", img->maxval);

    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            printf("%3d ", img->Data[i * img->width + j]);
        }
        printf("\n");
    }
}
```

## FUNÇÕES

Funções principais.

- alg1: Recebe uma imagem e armazena subimagens filtradas em um diretório a partir de posições aleatórias da imagem.

```c
void alg1(char *img_name, char *dir, int n, int width, int height){
    struct Image *img = malloc(sizeof(struct Image));
    readPGMImage(img, img_name);

    struct Image img_filt = filtro(*img);
    struct Image *sub_images = calloc(n, sizeof(struct Image));
    
    for(int k = 0; k < n; k++){
        i = rand()%(img_filt.height - height + 1);
        j = rand()%(img_filt.width - width + 1);
        copy_data(&img_filt, i, j, &sub_images[k]);
    }
}
```

- alg2: Recebe um diretório de subimagens e identifica a localização de cada subimagem na imagem.

```c
void alg2(char *imagem, char *diretorio){
    struct Image src;
    readPGMImage(&src, imagem);

    while ((dir = readdir(d)) != NULL){
        char name[400];
        sprintf(name,"%s/%s", diretorio, dir->d_name);
        readPGMImage(&sub_image, name);

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
    }
}
```
