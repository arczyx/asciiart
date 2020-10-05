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

    // Correção de aspecto para os caracteres
    RGB pixel;
    pixel.r = 0;
    pixel.g = 0;
    pixel.b = 0;

    int aspectHeight = height/5;
    int aspectWidth = width/4;
    int aspectSize = aspectHeight*aspectWidth;

    Img aspect;
    aspect.height = aspectHeight;
    aspect.width = aspectWidth;
    RGB aspectPixels[aspectSize];
    aspect.img =  &aspectPixels;

    currentHeight = 0;
    currentWidth = 0;
    int currentAspectSize = 0;
    int sum = 0;

    //int times = 0;

    for(int charsperheight = 0; charsperheight<(height/5); charsperheight++) {
        currentHeight = charsperheight*5;
        for(int charsperwidth = 0; charsperwidth<(width/4)+1; charsperwidth++) {//(-1 na condicao)
            currentWidth = charsperwidth*4;
            for(int h = currentHeight; h<currentHeight+5; h++) {
                //printf("\n");
                for(int w = currentWidth; w<currentWidth+4; w++) {
                    //printf("[%d][%d] ", h, w);
                    //sleep(1);
                    //printf("[%d][%d]:%d \n", h, w, matriz[h][w].r);
                    sum = sum + matriz[h][w].r;
                    //printf("[%d][%d]: %d ", h, w, pixel.r);
                    //times++;
                }
                //currentWidth = currentWidth + 4;
            }
            //printf("sum: %d times: %d", sum, times);
            //exit(1);
            sum = sum/20;
            //printf("d: %d ", sum);
            //exit(1);
            pixel.r = sum;
            pixel.g = sum;
            pixel.b = sum;
            sum = 0;
            
            aspectPixels[currentAspectSize] = pixel;
            currentAspectSize++;
        }
    }

    // while(1) {
    //     for(int h = currentHeight; h<currentHeight+5; h++) {
    //         if(h<=height) {
    //             for(int w = currentWidth; w<currentWidth+4; w++) {
    //                 if(w<=width) {
    //                     printf("[%d][%d]: %d ", h, w, sum);
    //                     sum = sum + matriz[h][w].r;
    //                     //printf("%d ", matriz[h][w].r);
    //                 }
    //             }
    //         }
    //     }
    //     //printf("%d ", sum);
    //     sum = sum/9;
    //     //printf("%d ", sum);
    //     pixel.r = sum;
    //     aspectPixels[currentAspectSize] = pixel;
    //     currentAspectSize++;
    //     currentWidth = currentWidth + 4;
        
    //     if(currentWidth>aspectWidth) {
    //         currentHeight++;
    //         currentWidth=0;
    //     }
    // }
    
    // Conversão da matriz de volta em um vetor para testar na API
    // Img new;
    // RGB pixels[size];

    // new.height = height;
    // new.width = width;
    // new.img = &pixels;

    // currentHeight = 0;
    // currentWidth = 0;

    // for(int tam=0; tam<size; tam++) {
    //     pixels[tam] = matriz[currentHeight][currentWidth];
    //     currentWidth++;
    //     if(currentWidth>width) {
    //         currentHeight++;
    //         currentWidth=0;
    //     }
    // }

    // Exemplo: gravando um arquivo de saída com a imagem (não é necessário para o trabalho, apenas
    // para ver o resultado intermediário, por ex, da conversão para tons de cinza)
    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, aspect.width, aspect.height,
        3, (const unsigned char*) aspect.img);

    // Exemplo: gravando um arquivo saida.html
    FILE* arq = fopen("saida.html", "w"); // criar o arquivo: w
    if(arq == NULL) // 0, falso
    {
        printf("Erro abrindo arquivo de saída\n");
        exit(1);
    }

    fprintf(arq, "<html><head></head>\n");
    fprintf(arq,"<body style=\"background: black;\" leftmargin=0 topmargin=0>\n");
    fprintf(arq,"<style>\n");
    fprintf(arq,"   pre {\n");
    fprintf(arq,"       color: white;\n");
    fprintf(arq,"       font-family: Courier;\n");
    fprintf(arq,"       font-size: 8px;\n");
    fprintf(arq,"   }\n");
    fprintf(arq,"</style>\n");
    fprintf(arq,"<pre>\n");

    currentWidth = 0;

    for(int tam=0; tam<aspectSize; tam++) {
        if(aspect.img[tam].r < 32) {
            fprintf(arq,".");
        } else if(aspect.img[tam].r < 64) {
            fprintf(arq,":");
        } else if(aspect.img[tam].r < 96) {
            fprintf(arq,"c");
        } else if(aspect.img[tam].r < 128) {
            fprintf(arq,"o");
        } else if(aspect.img[tam].r < 160) {
            fprintf(arq,"C");
        } else if(aspect.img[tam].r < 192) {
            fprintf(arq,"O");
        } else if(aspect.img[tam].r < 224) {
            fprintf(arq,"8");
        } else {
            fprintf(arq,"@");
        }/*
        
        } else if(aspect.img[tam].r < 64) {
            fprintf(arq,":");
        } else if(aspect.img[tam].r < 96) {
            fprintf(arq,"c");
        } else if(aspect.img[tam].r < 128) {
            fprintf(arq,"o");
        } else if(aspect.img[tam].r < 160) {
            fprintf(arq,"C");
        } else if(aspect.img[tam].r < 192) {
            fprintf(arq,"O");
        } else if(aspect.img[tam].r < 224) {
        */
        currentWidth++;
        if(currentWidth>aspectWidth) {
            fprintf(arq,"\n");
            currentWidth = 0;
        }
    }

    fprintf(arq,"\n</pre>\n");
    fprintf(arq,"</body>\n");
    fprintf(arq,"</html>\n");

    fclose(arq);
    free(pic.img);
}
