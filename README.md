# Projeto: Busca de Regiões em Imagens

# Integrantes:
- Lucas Rocha
- Aldair Ryan

# Trabalho final da Laboratório de Programação

Projeto iniciando como trabalho final da cadeira de Laboratório de Programação, pelo professor Daniel Ferreira no curso de Ciências da Computação.

Inclui dois algoritmos principais. O alg1 salva n subimagens borradas (que chamei várias vazes de recortes), de tamanho width x height, em um diretório a partir de uma imagem. O alg2 recebe uma imagem e um diretório com subimagens, e salva em um arquivo o nome de cada subimagem ao lado da localização de onde ela foi identificada na imagem, especificamente a coordenada da imagem que equivale do que equivale pixel superior esquerdo da subimagem, calculado por correlação cruzada.

# Arquivos do projeto

### Main

//Código do main.c explicando cada passo

### PGM

//Código do pgm.c explicando cada passo

//Lembrar de corrigir

#ifdef __linux__
  fseek(fp,1, SEEK_CUR);
#elif _WIN32
  fseek(fp,0, SEEK_CUR);
#endif

### FUNÇÕES

//Código do funcs.c explicando cada passo

