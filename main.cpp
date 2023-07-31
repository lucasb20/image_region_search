#include <iostream>
#include "lib/pgm.h"
#include "lib/funcs.h"
#include <chrono>

int main(int argc,char **argv){
    if(argc != 4){
        std::cout << "Formato: " << argv[0] << "<num -> alg1 (1) ou alg2 (2)> <imagem> <directory>" << std::endl;
        exit(1);
    }

    int op = std::atoi(argv[1]);

    if(op != 1 && op != 2){
        std::cout << "Operação inválida. Escolha 1 para alg1 ou 2 para alg2." << std::endl;
        exit(1);
    }

    const char *imagem = argv[2];
    const char *diretory = argv[3];

 switch (op) {
        case 1: {
            std::cout << "Digite uma quantidade de subimagens: ";
            int n;
            std::cin >> n;

            std::cout << "Digite uma largura: ";
            int w;
            std::cin >> w;

            std::cout << "Digite uma altura: ";
            int h;
            std::cin >> h;

            alg1(imagem, diretory, n, w, h);
            break;
        }

        case 2: {
            auto begin = std::chrono::high_resolution_clock::now();
            alg2(imagem, diretory);
            auto end = std::chrono::high_resolution_clock::now();
            double tempo_total = std::chrono::duration<double>(end - begin).count();
            std::cout << "Tempo total: " << tempo_total << "s\n";
            std::cout << "(" << tempo_total / 60 << " min)\n";
            break;
        }
    }

    return 0;
}