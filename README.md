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
    img->tipo = getc(fp) - 48;

    while ((ch = getc(fp)) == '#') {
        while ((ch = getc(fp)) != '\n');
    }

    fseek(fp, -1, SEEK_CUR);

    fscanf(fp, "%d %d", &img->width, &img->height);
    fscanf(fp, "%d", &img->maxval);
    fseek(fp, 1, SEEK_CUR);

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
}
```

- writePGMImage: Cria um arquivo .pgm a partir de uma estrutura Image.

```c
void writePGMImage(struct Image *img, char *filename) {
    fprintf(fp, "%s\n", "P5");
    fprintf(fp, "%d %d\n", img->width, img->height);
    fprintf(fp, "%d\n", 255);

    for (int i = 0; i < img->height; i++) {
        fwrite(img->Data[i], sizeof(unsigned char), img->width, fp);
    }
}
```

- viewPGMImage: Imprime informações de uma estrutura image, inclusive a matriz de pixels.

```c
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
```

## FUNÇÕES

Funções principais.

- alg1: Recebe uma imagem e armazena subimagens filtradas em um diretório a partir de posições aleatórias da imagem.

```c
void alg1(char *imagem, char *diretorio,int n,int width,int height){  
    struct Image *o = malloc(sizeof(struct Image));
    readPGMImage(o,imagem);

    struct Image o_filt = filtro(*o);
    struct Image *recortes = calloc(n,sizeof(struct Image));
    
    while(k<n){
        i = rand()%(o_filt.height-height);
        j = rand()%(o_filt.width-width);
        copy_data(&o_filt,i,j,&recortes[k]);
        k++;
    }
  }
```

- alg2: Recebe um diretório de subimagens e identifica a localização de cada subimagem na imagem.

```c
void alg2(char *imagem, char *diretorio){
    struct Image src;
    readPGMImage(&src,imagem);

    FILE *file_ptr = fopen("alg2.txt","w");

    if(d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            char nome[400];
            sprintf(nome,"%s/%s",diretorio,dir->d_name);
            readPGMImage(&rec,nome);

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
    }
}
  ```