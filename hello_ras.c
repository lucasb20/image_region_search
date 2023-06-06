#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Image{
    int width;
    int height;
    int maxval;
    unsigned char **Data;
};

int gerar_matriz(struct Image *o);
void preencher(struct Image *o);
void imprimir(struct Image o);
void desalocar_matriz(struct Image *o);
void copy_data(struct Image *,int,int, struct Image *);
struct Image filtro(struct Image o);
unsigned char media(struct Image o,int x, int y);
struct Image *alg1(struct Image *,int,int,int);

int main(int argc,char **argv){
    if(argc != 4){
        printf("Formato: %s <width> <height> <max value>\n",*argv);
        exit(1);
    }

    srand(time(NULL));

    struct Image foto;
    foto.width = atoi(*(argv+1));
    foto.height = atoi(*(argv+2));
    foto.maxval = atoi(*(argv+3));

    if(gerar_matriz(&foto)){
        puts("Faltou memória.");
        exit(1);
    }

    preencher(&foto);

    imprimir(foto);
    
    struct Image foto_filt=filtro(foto);

    /* imprimir(foto_filt); */

    /* int n = 3;

    struct Image *recortes = alg1(&foto,n,2,2);

    for(int i=0;i<n;i++){
        printf("Recorte número %d: \n",i);
        imprimir(*(recortes+i));
    } */

    return 0;
}

//Algoritmo 1: Fazer N recortes aleatórios de tamanho L*M (E um ponteiro para cada posição?) com o filtro média.

struct Image *alg1(struct Image *o,int n,int width,int height){
    int k = 0;
    int i=0,j=0;
    struct Image *recortes = calloc(n,sizeof(struct Image));
    while(k<n){
        if(!(o->width = width))i = rand()%(o->width-width);
        if(!(o->height = height))j = rand()%(o->height-height);
        printf("i: %d j: %d\n",i,j);
        recortes[k].width = width;
        recortes[k].height = height;
        recortes[k].maxval = o->maxval;
        gerar_matriz(recortes+k);
        copy_data(o,i,j,&recortes[k]);
        k++;
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
    struct Image img;
    img.height = o.height;
    img.width = o.width;
    img.maxval = o.maxval;
    printf("img h: %d, w: %d, m: %d\n",img.height,img.width,img.maxval);
    for(int i=0;i<img.height;i++){
        for(int j=0;j<img.width;j++){
            /* img.Data[i][j]=media(o,i,j); */
            printf("Teste %d %d\n",i,j);
        }
    }
}

unsigned char media(struct Image o,int x, int y){
    unsigned char m = 0;

    printf("m: %d",m);

    if(x==0 && y==0){
        m = (o.Data[x][y]+o.Data[x][y+1]+o.Data[x+1][y]+o.Data[x+1][y+1])/9;
    }
    else if(x==0 && y==(o.width-1)){
        m = (o.Data[x][y]+o.Data[x][y-1]+o.Data[x+1][y]+o.Data[x-1][y+1])/9;
    }
    else if(y==0 && x==(o.height-1)){
        m = (o.Data[x][y]+o.Data[x-1][y]+o.Data[x-1][y+1]+o.Data[x][y+1])/9;
    }
    else if(y==(o.width-1) && x==(o.height-1)){
        m = (o.Data[x][y]+o.Data[x-1][y]+o.Data[x][y-1]+o.Data[x-1][y-1]);
    }
    else if(x==0){
        m = (o.Data[x][y]+o.Data[x][y+1]+o.Data[x+1][y]+o.Data[x+1][y+1]+o.Data[x+1][y-1]+o.Data[x][y-1])/9;
    }
    //Parei aqui
    else if(x==(o.height-1)){
        m = (o.Data[x][y]+o.Data[x-1][y]+o.Data[x][y+1]+o.Data[x][y-1]+o.Data[x-1][y+1]+o.Data[x-1][y-1])/9;
    }
    if(y==0){
        m = (o.Data[x][y]+o.Data[x-1][y]+o.Data[x+1][y]+o.Data[x-1][y+1]+o.Data[x][y+1]+o.Data[x+1][y+1])/9;
    }
    if(y==(o.width-1)){
        m = (o.Data[x][y]+o.Data[x-1][y]+o.Data[x+1][y]+o.Data[x][y-1]+o.Data[x-1][y-1]+o.Data[x+1][y-1])/9;
    }
    else{
        m = (o.Data[x][y]+o.Data[x+1][y]+o.Data[x-1][y]+o.Data[x][y+1]+o.Data[x][y-1]+o.Data[x-1][y-1]+o.Data[x+1][y+1]+o.Data[x+1][y-1]+o.Data[x-1][y+1])/9;
    }

    return m;
}

//Algoritmo 2: Procurar na imagem a posição de onde foi retirada e um ponteiro para ela.

//Esperar Documento do Daniel.

int gerar_matriz(struct Image *o){
    if(!(o->Data = (unsigned char **)malloc(o->height*sizeof(unsigned char*)))){
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
            printf("%2d ",*(*(o.Data+i)+j));
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