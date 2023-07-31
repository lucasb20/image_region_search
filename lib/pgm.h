#ifndef PGM_H
#define PGM_H

struct Image {
    int tipo;
    int width;
    int height;
    int maxval;
    unsigned char **Data;
};

void readPGMImage(struct Image *, const char *);
void viewPGMImage(struct Image *);
void writePGMImage(struct Image *, const char *);

#endif
