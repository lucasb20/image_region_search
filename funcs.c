#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dirent.h>
#include <string.h>
#include "lib/funcs.h"
#include "lib/pgm.h"

//Algoritmo 1: Será dado uma Imagem, e será dado um diretório para salvar N subimagens de tamanho width x height com o filtro média
void alg1(char *imagem, char *diretorio,int n,int width,int height){
    //Declaração de auxiliar para contagem de recortes e posições deles
    int k = 0;
    int i=0,j=0;
    
    //Criando uma estrutura para armazenar a imagem em que será coletado subimagens
    struct Image *o = malloc(sizeof(struct Image));
    readPGMImage(o,imagem);

    //Criação da versão filtrada da imagem e alocação de *recortes
    struct Image o_filt = filtro(*o);
    struct Image *recortes = calloc(n,sizeof(struct Image));
    
    //Percorre todos os recortes
    while(k<n){
        //Gera posições aleatórias, essa condição é mais para depuração, saber que as dimensões estão corretas.
        i = rand()%(o_filt.height-height);
        j = rand()%(o_filt.width-width);
        
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
    }

    //Salvando os recortes para a pasta.
    for(int z=0;z<n;z++){
        char nome[100];
        sprintf(nome,"%s/subimagem%d.pgm",diretorio,z);
        writePGMImage(recortes+z,nome);
    }
}

//Algoritmo 2: Recebe uma imagem, e recebe um diretório onde haverão subimagens, será verificado de onde cada uma foi retirada na imagem e imprimirá a posição superior esquerda em um arquivo.
void alg2(char *imagem, char *diretorio){

    struct Image src;
    readPGMImage(&src,imagem);

    struct Image rec;

    //p guarda a posição [x,y]
    int *p = NULL;
    //v é uma matriz para guardar o recorte normalizado
    double **v = NULL;
    //Definindo maior correlação para -infinito, já que tem que pegar a maior correlação, tem que inicializar na menor possível para ser substituida logo
    double maior_corr = -INFINITY;

    //Como todas as imagens são do mesmo tamanho, isso aqui é para garantir que o 'v' só receba as dimensões da primeira imagem
    char first_rec = 1;

    //Declarando ponteiro para a pasta.
    DIR *d;
    struct dirent *dir;

    // Aloca memória para o array de inteiros p e a matriz v
    p = calloc(2, sizeof(int));
    if (!p){
        printf("Falta de memória.\n");
        exit(1);
    }

    //Lendo cada imagem do diretório e salvando cada posição
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
            //Eu não sei o porquê, mas dentro de todo diretório existe um arquivo '.' e '..'.
            //Essa confição pula a leitura desses arquivos não .pgm.
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
                continue;
            }
            
            //Evitando de resultados anteriores influênciarem
            maior_corr = -INFINITY;

            //Nome do arquivo no diretório e passando para struct Image do recorte
            char nome[400];
            sprintf(nome,"%s/%s",diretorio,dir->d_name);
            readPGMImage(&rec,nome);

            //Como é garantido que as imagens seguem as dimensões da primeira imagem, só basta realizar alocação uma vez com essa condição
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
    
    // Libera a memória alocada para a matriz v
    for (int i = 0; i < rec.height; i++) {
        free(v[i]);
    }
    free(v);
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