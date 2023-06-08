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
int *alg2(struct Image src,struct Image rec);
double quad(double x);
double erro_mq(unsigned char *v,int tam);
double modul(double x);

int main(int argc,char **argv){

    srand(time(NULL));

    struct Image foto;
    foto.width = 7;
    foto.height = 3;
    foto.maxval = 255;

    if(gerar_matriz(&foto)){
        puts("Faltou memória.");
        exit(1);
    }

    preencher(&foto);

    struct Image foto_f=filtro(foto);

    printf("Imagem aleatória:\n");
    imprimir(foto);
    printf("Imagem filtrada:\n");
    imprimir(foto_f);

    struct Image *recorte;

    printf("Recorte:\n");
    recorte = alg1(&foto_f,1,2,2);
    imprimir(*recorte);

    int *pos = alg2(foto,*recorte);
    printf("alg2 encontrou:\nx: %d, y: %d.\n",*pos,*(pos+1));

    return 0;
}

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

    return img;
}

unsigned char media(struct Image o,int x, int y){
    unsigned char m;

    m = (o.Data[x][y]+o.Data[x][y+1]+o.Data[x][y-1]+o.Data[x-1][y]+o.Data[x-1][y+1]+o.Data[x-1][y-1]+o.Data[x+1][y]+o.Data[x+1][y+1]+o.Data[x+1][y-1])/9;

    return m;
}

//Algoritmo 2: Procurar na imagem a posição de onde foi retirada e um ponteiro para ela.

//Tem que retorna um vetor v = [x,y].
int *alg2(struct Image src,struct Image rec){
    int *p=NULL;
    unsigned char *v;
    double emq_rec;
    double emq_aux;
    double menor_dif;
    
    // p = [x,y]
    p = calloc(2,sizeof(int));
    v = calloc(rec.width*rec.height,sizeof(unsigned char));

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

double erro_mq(unsigned char *v,int tam){
    double media = 0;
    double res=0;
    for(int i=0;i<tam;i++)media+=*(v+i);
    media/=tam;
    for(int i=0;i<tam;i++)res+=quad(*(v+i)-media);
    return res/tam;
}

double quad(double x){
    return x*x;
}

double modul(double x){
    return (x>=0)?x:-x;
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
