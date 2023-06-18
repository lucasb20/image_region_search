#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lib/funcs.h"
#include "lib/pmg.h"

//Algoritmo 1: Fazer N subimagens de tamanho width x height com o filtro média
//Falta: Parâmetro para receber imagem PMG fornecida pelo usuário, salvar em um diretório fornecido pelo usuário.
struct Image *alg1(struct Image *o,int n,int width,int height){
    //Declaração de auxiliar para contagem de recortes e posições deles
    int k = 0;
    int i=0,j=0;
    
    //Criação da versão filtrada da imagem e alocação de *recortes
    struct Image o_filt = filtro(*o);
    struct Image *recortes = calloc(n,sizeof(struct Image));
    
    //Percorre todos os recortes
    while(k<n){
        //Gera posições aleatórias, essa condição é mais para depuração, saber que as dimensões estão corretas.
        i = (o_filt.height <= height)?0:rand()%(o_filt.height-height);
        j = (o_filt.width <= width)?0:rand()%(o_filt.width-width);
        
        //Definindo atributos
        recortes[k].tipo = o->tipo;
        recortes[k].width = width;
        recortes[k].height = height;
        recortes[k].maxval = o_filt.maxval;

        //Alocando o recorte[k]
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

        //Copiando da matriz filtrada o recorte e colando no recortes[k]
        copy_data(&o_filt,i,j,&recortes[k]);
        k++;
        //writePGMImage(recortes+k,"nome");
    }
    return recortes;
}

//Algoritmo 2: Procurar na imagem a posição de onde foi retirada o recorte e um ponteiro para ela e retorna um vetor v = [x,y].
//Falta: Parâmetro para receber diretório onde buscar os recortes e a imagem I, reportar tempo total para terminar o processo todo, salvar em um arquivo de texto as coordenadas com "nome,x,y\n"
int *alg2(struct Image src, struct Image rec) {
    
    //v aguardar a posição [x,y]
    int *p = NULL;

    //v é uma matriz para guardar o recorte normalizado
    double **v = NULL;
    //Definindo maior correlação para -infinito, já que tem que pegar a maior correlação, tem que inicializar na menor possível para ser substituida logo
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

    // Normaliza a matriz rec e armazena em v
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
        //Apenas depuração, para conseguir olhar no terminal quantas etapas faltam para acabar
        //Lembrar de remover isso
        printf("Demora: %d/%d\n",i,src.height - rec.height);
    }

    // Libera a memória alocada para a matriz v
    for (int i = 0; i < rec.height; i++) {
        free(v[i]);
    }
    free(v);

    return p;
}

//Copia os elementos de uma matriz src começando na posição [x,y] e cola em uma matriz des até completar ela.
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

//Função de retorna a Image filtrada da que foi passada por parâmetro
struct Image filtro(struct Image o){

    //img será a imagem filtrada, aux será apenas uma estrutura auxiliar para resolver problemas com bordas
    struct Image img,aux;

    //Definindo img
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

    //Definindo matriz auxiliar
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

    //Colocando o img na auxilar, deixando os espaços das bordas 0.
    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
            aux.Data[i+1][j+1] = img.Data[i][j];
        }
    }

    //Definindo a img a partir da auxiliar, de forma que as bordas serão consideradas 0
    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
            img.Data[i][j] = media(aux,i+1,j+1);
        }
    }

    return img;
}

//Calcula a media dos elementos adjacentes a um Data[i][j] em uma struct Image.
unsigned char media(struct Image o,int x, int y){
    unsigned char m;

    m = (o.Data[x][y]+o.Data[x][y+1]+o.Data[x][y-1]+o.Data[x-1][y]+o.Data[x-1][y+1]+o.Data[x-1][y-1]+o.Data[x+1][y]+o.Data[x+1][y+1]+o.Data[x+1][y-1])/9;

    return m;
}

//Método de similação
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


//Retorna a media dos elementos de uma Data de uma struct Image.
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