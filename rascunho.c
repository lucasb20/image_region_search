#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <lib/pmg.h>

//Algoritmo 1: Fazer N recortes aleatórios de tamanho L*M (E um ponteiro para cada posição?) com o filtro média.
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
        gerar_matriz(recortes+k);
        copy_data(o,i,j,&recortes[k]);
        k++;
        printf("Recorte: i: %d, j: %d\n",i,j);
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
    if(gerar_matriz(&img))exit(1);
    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
            img.Data[i][j] = o.Data[i][j];
        }
    }

    aux.height = o.height + 2;
    aux.width = o.height + 2;
    aux.maxval = o.maxval;
    if(gerar_matriz(&aux))exit(1);

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

    desalocar_matriz(&aux);

    return img;
}

unsigned char media(struct Image o,int x, int y){
    unsigned char m;

    m = (o.Data[x][y]+o.Data[x][y+1]+o.Data[x][y-1]+o.Data[x-1][y]+o.Data[x-1][y+1]+o.Data[x-1][y-1]+o.Data[x+1][y]+o.Data[x+1][y+1]+o.Data[x+1][y-1])/9;

    return m;
}

//Algoritmo 2: Procurar na imagem a posição de onde foi retirada e um ponteiro para ela.

//Tem que retorna um vetor v = [x,y].

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

//Esperar Documento do Daniel.

int gerar_matriz(struct Image *o){
    if(!(o->Data = (unsigned char **)calloc(o->height,sizeof(unsigned char*)))){
        return 1;
    }
    for(int i=0;i<o->height;i++){
            if(!(o->Data[i]=(unsigned char *)calloc(o->width,sizeof(unsigned char)))){
                return 1;
            }
    }

    return 0;
}

void preencher(struct Image *o){
    for(int i=0;i<o->height;i++){
        for(int j=0;j<o->width;j++){    
            *(*(o->Data+i)+j)=rand()%(o->maxval+1);
        }
    }
}

void imprimir(struct Image o){
    for(int i=0;i<o.height;i++){
        for(int j=0;j<o.width;j++){
            printf("%3d ",*(*(o.Data+i)+j));
        }
        printf("\n");
    }
    printf("\n");
}

void desalocar_matriz(struct Image *o){
    for(int i=0;i<o->height;i++){
        free(o->Data[i]);
    }
    free(o->Data);
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

struct Image resize_image(struct Image img, float scale) {
    struct Image resized;
    resized.tipo = img.tipo;
    resized.width = (int)(img.width * scale);
    resized.height = (int)(img.height * scale);
    resized.maxval = img.maxval;

    // Aloca memória para a matriz de dados redimensionada
    resized.Data = calloc(resized.height, sizeof(unsigned char *));
    for (int i = 0; i < resized.height; i++) {
        resized.Data[i] = calloc(resized.width, sizeof(unsigned char));
    }

    // Redimensiona a imagem usando interpolação bilinear
    for (int i = 0; i < resized.height; i++) {
        for (int j = 0; j < resized.width; j++) {
            float x = j / scale;
            float y = i / scale;
            int x1 = (int)x;
            int y1 = (int)y;
            int x2 = x1 + 1;
            int y2 = y1 + 1;

            // Garante que os índices estejam dentro dos limites da imagem original
            x2 = x2 < img.width ? x2 : img.width - 1;
            y2 = y2 < img.height ? y2 : img.height - 1;

            // Interpolação bilinear
            float a = img.Data[y1][x1] * (x2 - x) * (y2 - y);
            float b = img.Data[y1][x2] * (x - x1) * (y2 - y);
            float c = img.Data[y2][x1] * (x2 - x) * (y - y1);
            float d = img.Data[y2][x2] * (x - x1) * (y - y1);

            resized.Data[i][j] = (unsigned char)(a + b + c + d);
        }
    }

    return resized;
}

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

    // Crie a pirâmide de imagens
    float scale = 0.5; // Fator de escala para a pirâmide de imagens
    struct Image src_resized = resize_image(src, scale);
    struct Image rec_resized = resize_image(rec, scale);

    // Verifique se a matriz src_resized é maior ou igual à matriz rec_resized
    if (src_resized.height < rec_resized.height || src_resized.width < rec_resized.width) {
        printf("Erro: a matriz src_resized deve ser maior ou igual à matriz rec_resized.\n");
        return NULL;
    }

    // Percorra todas as possíveis posições na matriz src_resized onde a matriz rec_resized pode ser colocada
    for (int i = 0; i < src_resized.height - rec_resized.height + 1; i++) {
        for (int j = 0; j < src_resized.width - rec_resized.width + 1; j++) {
            // Calcula a correlação cruzada entre as matrizes src_resized e rec_resized na posição (i, j)
            double corr = correlacao_cruzada(src_resized.Data, v, src_resized.height, src_resized.width, rec_resized.height, rec_resized.width, i, j);
            if (corr > maior_corr) {
                maior_corr = corr;
                p[0] = i;
                p[1] = j;
            }
        }
    }

    // Refine a posição do recorte na imagem original
    int search_range = 10; // Ajuste este valor de acordo com a precisão desejada
     for (int i = max(0, p[0] * 2 - search_range); i < min(src.height - rec.height, p[0] * 2 + search_range); i++) {
        for (int j = max(0, p[1] * 2 - search_range); j < min(src.width - rec.width, p[1] * 2 + search_range); j++) {
            // Calcula a correlação cruzada entre as matrizes src e rec na posição (i, j)
            double corr = correlacao_cruzada(src.Data, v, src.height, src.width, rec.height, rec.width, i, j);
            if (corr > maior_corr) {
                maior_corr = corr;
                p[0] = i;
                p[1] = j;
            }
        }
    }    

    // Libera a memória alocada para a matriz v
    for (int i = 0; i < rec.height; i++) {
        free(v[i]);
    }
    free(v);

    return p;
}


//Depois eu ajeito isso, criar as funções.
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))


/* int *alg2(struct Image src,struct Image rec){
    int *p=NULL;

    double *v;
    double emq_rec;
    double emq_aux;
    double menor_dif;

    // p = [x,y]
    p = calloc(2,sizeof(int));
    if(!p)exit(1);
    v = calloc(rec.width*rec.height,sizeof(unsigned char));
    if(!v)exit(1);

    for(int a=0;a<rec.height;a++){
        for(int b=0;b<rec.width;b++){
            *(v+b+a*rec.height)=rec.Data[a][b];
        }
    }

    emq_rec = erro_mq(v,rec.width*rec.height);

    for(int a=0;a<rec.height;a++){
        for(int b=0;b<rec.width;b++){
            *(v+b+a*rec.height)=src.Data[a][b];
        }
    }

    emq_aux = erro_mq(v,rec.width*rec.height);

    menor_dif = modul(emq_aux - emq_rec);

    for(int i=0;i<src.height-rec.height+1;i++){
        for(int j=0;j<src.width-rec.width+1;j++){
            for(int a=0;a<rec.height;a++){
                for(int b=0;b<rec.width;b++){
                    *(v+b+a*rec.height)=src.Data[i+a][j+b];
                }
            }
            emq_aux = erro_mq(v,rec.width*rec.height);
            if(modul(emq_aux-emq_rec)<menor_dif){
                menor_dif = modul(emq_aux-emq_rec);
                *p = i;
                *(p+1) = j;
            }
        }
    }

    return p;
}
 */