# Image Region Search

## Visão Geral

O Image Region Search é um projeto desenvolvido como trabalho final da disciplina de Laboratório de Programação, ministrada pelo professor Daniel Ferreira. O objetivo principal deste projeto é criar uma aplicação capaz de salvar subimagens filtradas de uma imagem em escalas de cinza e localizar a posição dessas subimagens na imagem original.

## Estrutura do Projeto

O projeto está organizado da seguinte forma:

- **Makefile:** O arquivo Makefile contém as instruções para compilar e construir o projeto.
- **lib:** Esta pasta contém os cabeçalhos necessários para compilar o projeto.
- **main.c:** O arquivo main.c é usado para testar o projeto e executar as funcionalidades implementadas.
- **funcs.c:** Este arquivo contém as funções principais responsáveis pela lógica do projeto.
- **utils.c:** Aqui estão implementadas as funções matemáticas necessárias para o funcionamento dos algoritmos.
- **pgm.c:** O arquivo pgm.c contém funções para lidar com imagens em escala de cinza.

## Algoritmos Principais

O projeto inclui dois algoritmos principais:

1. **alg1:** Este algoritmo é responsável por salvar subimagens filtradas a partir de uma imagem em escalas de cinza.
2. **alg2:** Este algoritmo realiza o processo inverso do algoritmo anterior, localizando na imagem a posição de uma subimagem filtrada.

## Como Utilizar

Para utilizar o projeto, siga estas etapas:

1. Compile o projeto utilizando o Makefile.
2. Execute o arquivo gerado para testar as funcionalidades implementadas.
3. Siga as instruções apresentadas no console para interagir com o projeto.

## Eficiência dos Algoritmos

Não há o que medir no alg1, porque, para quaisquer medidas ou quantidades, demora menos de um segundo para realizar esse simples processo.

Para os algoritmos de comparação, a quinta imagem da pasta 'outros' (img04_ORIGINAL.pgm) foi utilizada no alg1 para gerar as subimagens.

Obs: Quanto maior a área da subimagem, mais fácil é para comparar, logo melhor é a precisão. Quanto maior a quantidade de subimagens, maior é o tempo total de execução.

1. **alg2 ( MSE - Erro Médio Quadrático)**

100 subimagens 50x50

Acurácia (sem tolerância): **100%**

Precisão Média: **100%**

Tempo Total: **15 min**

2. **alg2 ( cross-corr - Correlação Cruzada )**

10 subimagens 100x100

Acurácia (sem tolerância): **70%**

Precisão Média: **99.8%**

Tempo Total: **67 min**

## Contribuindo

Contribuições são bem-vindas! Sinta-se à vontade para abrir uma issue ou enviar um pull request com melhorias ou novas funcionalidades.

## Licença

Este projeto está licenciado sob a [MIT License](LICENSE).
