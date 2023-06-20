/* 
    Aluno : Lucas da Rocha Barbosa / Aldair Ryan Fernandes Mendes
    Matricula : *** / ***
    Avaliacao 04 : Trabalho Final
    04.505.23−2023.1 − Prof.Daniel Ferreira
    Compilador : GCC versão 11.3.0
 */

#ifndef PGM_H
#define PGM_H

struct Image {
    int tipo;
    int width;
    int height;
    int maxval;
    unsigned char **Data;
};

void readPGMImage(struct Image *, char *);
void viewPGMImage(struct Image *);
void writePGMImage(struct Image *, char *);

#endif
