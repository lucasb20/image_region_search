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

int main(int argc,char **argv){
    if(argc != 4){
        printf("Formato: %s <width> <height> <max value>\n",*argv);
        exit(1);
    }

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

    desalocar_matriz(&foto);

    return 0;
}

//Algoritmo 1: Fazer N recortes aleatórios de tamanho L*M (E um ponteiro para cada posição?)

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
    printf("%d %d\n",o.height,o.width);
    printf("%d\n",o.maxval);
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