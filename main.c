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
    int ns = (pic->width * pic->height) * 0.5;
    RGB newpixels[ns];
    //pic->img = newpixels;
}

int main(int argc, char** argv)
{
    Img pic;
    if(argc == 1) {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    // Conversão da imagem armazenada em vetor para uma matriz
    int height = pic.height;
    int width = pic.width;
    int size = height*width;

    RGB matriz[height][width];

    int currentHeight = 0;
    int currentWidth = 0;
    
    for(int tam=0; tam<size; tam++) {
        matriz[currentHeight][currentWidth] = pic.img[tam];
        currentWidth++;
        if(currentWidth>width) {
            currentHeight++;
            currentWidth=0;
        }
    }

    // Conversão da matriz em cinza
    for(int h = 0; h<height; h++){
        for(int w = 0; w<width; w++) {
            unsigned char gray = (0.3 * matriz[h][w].r + 0.59 * matriz[h][w].g + 0.11 * matriz[h][w].b);
            matriz[h][w].r = gray;
            matriz[h][w].g = gray;
            matriz[h][w].b = gray;
        }
    }

    //Conversão da matriz de volta em um vetor para testar na API
    Img new;
    RGB pixels[size];

    new.height = height;
    new.width = width;
    new.img = &pixels;

    currentHeight = 0;
    currentWidth = 0;

    for(int tam=0; tam<size; tam++) {
        pixels[tam] = matriz[currentHeight][currentWidth];
        currentWidth++;
        if(currentWidth>width) {
            currentHeight++;
            currentWidth=0;
        }
    }

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, new.width, new.height,
        3, (const unsigned char*) new.img);

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
