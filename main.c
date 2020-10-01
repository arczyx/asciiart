#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct {
    int width, height;
    RGB* img;
} Img;

// Protótipos
void load(char* name, Img* pic);
void foo(Img* pic);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (unsigned char*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

void foo(Img* pic)
{
    int ns = pic->width * pic->height / 20;
    RGB newpixels[ns];
}

int main(int argc, char** argv)
{
    Img pic;
    if(argc == 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    for(int tam=(pic.width)*(pic.height)-1; tam>=0; tam--) {
        unsigned char cinza = (0.3 * pic.img[tam].r + 0.59 * pic.img[tam].g + 0.11 * pic.img[tam].b);
        pic.img[tam].r = cinza;
        pic.img[tam].g = cinza;
        pic.img[tam].b = cinza;
        printf("[%02X %02X %02X] ", pic.img[tam].r, pic.img[tam].g, pic.img[tam].b); 
    }

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height,
        3, (const unsigned char*) pic.img);

    // Exemplo: gravando um arquivo saida.html
    FILE* arq = fopen("saida.html", "w"); // criar o arquivo: w
    if(arq == NULL) // 0, falso
    {
        printf("Erro abrindo arquivo de saída\n");
        exit(1);
    }

    fprintf(arq, "<html>\n");
    fprintf(arq,"<head>\n");
    fprintf(arq,"</head>\n");
    fprintf(arq,"<body>\n");
    fprintf(arq,"<h1>Oi!</h1>\n");
    fprintf(arq,"</body>\n");

    fclose(arq);
    free(pic.img);
}
