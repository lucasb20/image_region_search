/*
 * C Program to List Files in Directory
	https://www.sanfoundry.com/c-program-list-files-directory/
 */
#include <dirent.h>
#include <stdio.h>
 
int main(void)
{
    DIR *d;
    struct dirent *dir;
    d = opendir("./images");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);

						// Leitura da Imagem -PGM

						// Processar - Medir o tempo apenas nesta etapa.
						
						// Saída.
        }
        closedir(d);
    }
    return(0);
}		