# Projeto: Busca de Regiões em Imagens

# Integrantes:
- Lucas Rocha
- Aldair Ryan

# Trabalho final da Laboratório de Programação

Projeto iniciando como trabalho final da cadeira de Laboratório de Programação, pelo professor Daniel Ferreira no curso de Ciências da Computação.

Inclui dois algoritmos principais. O alg1 salva n subimagens borradas (que chamei várias vazes de recortes), de tamanho width x height, em um diretório a partir de uma imagem. O alg2 recebe uma imagem e um diretório com subimagens, e salva em um arquivo o nome de cada subimagem ao lado da localização de onde ela foi identificada na imagem, especificamente a coordenada da imagem que equivale do que equivale pixel superior esquerdo da subimagem, calculado por correlação cruzada.

# Arquivos principais do projeto

### Main

  Pega informações dadas em linha de execução e realiza uma das duas operações que o programa promete fazer, exercutar o algoritmo 1 ou o algoritmo 2.

  ```c
    char *imagem = *(argv+2);
    char *diretory = *(argv+3);
    int op = atoi(*(argv+1));

    switch(op){
        case 1:
            int n;
            int w;
            int h;
            puts("Digite uma quantidade de subimagens.");
            scanf("%d",&n);
            puts("Digite uma largura.");
            scanf("%d",&w);
            puts("Digite uma altura.");
            scanf("%d",&h);

            alg1(imagem,diretory,n,w,h);
            break;

        case 2:
            clock_t begin = clock();
            alg2(imagem,diretory);
            clock_t end = clock();
            double tempo_total = (double)(end - begin)/CLOCKS_PER_SEC;
            printf("Tempo total: %lf\n",tempo_total);
            break;
    }
  ```

### PGM

  ```c
  //D:
  ```

### FUNÇÕES

 ```c
  //D:
  ```

