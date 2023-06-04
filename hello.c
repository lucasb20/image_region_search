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

    int n = 5;

    struct Image *recortes = alg1(&foto,n,2,2);

    for(int i=0;i<n;i++){
        printf("Recorte número %d: \n",i);
        imprimir(*(recortes+i));
    }

    return 0;
}

//Algoritmo 1: Fazer N recortes aleatórios de tamanho L*M (E um ponteiro para cada posição?)

struct Image *alg1(struct Image *o,int n,int width,int height){
    int k = 0;
    int i=0,j=0;
    struct Image *recortes = calloc(n,sizeof(struct Image));
    if(!recortes)exit(1);
    while(k<n){
        i = rand()%(o->width-width);
        j = rand()%(o->height-height);
        recortes[k].width = width;
        recortes[k].height = height;
        recortes[k].maxval = o->maxval;
        if(gerar_matriz(recortes+k))exit(1);
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