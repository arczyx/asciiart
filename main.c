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

    // Variáveis auxiliares
    int height = pic.height;
    int width = pic.width;
    int size = height*width;
    int currentHeight = 0;
    int currentWidth = 0;
    
    // Permite que o vetor de pixeis seja interpretado como uma matriz
    RGB(*picM)[pic.width] = (RGB(*)[pic.width]) pic.img;

    // Conversão da "matriz" em cinza
    for(int h = 0; h<height; h++){
        for(int w = 0; w<width; w++) {
            unsigned char gray = (0.3 * picM[h][w].r + 0.59 * picM[h][w].g + 0.11 * picM[h][w].b);
            picM[h][w].r = gray;
            picM[h][w].g = gray;
            picM[h][w].b = gray;
        }
    }

    // Correção de aspecto para os caracteres, gerando uma nova imagem

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

    // Permite que o vetor de pixeis seja interpretado como uma matriz
    RGB(*aspectM)[aspect.width] = (RGB(*)[aspect.width]) aspect.img;

    currentHeight = 0;
    currentWidth = 0;
    int currentAspectSize = 0;
    int sum = 0;
    // Percorre a altura
    for(int charsperheight = 0; charsperheight<(height/5); charsperheight++) {
        currentHeight = charsperheight*5;
        // Percorre a largura
        for(int charsperwidth = 0; charsperwidth<(width/4); charsperwidth++) {
            currentWidth = charsperwidth*4;
            // Percorre 5 pixeis na altura atual
            for(int h = currentHeight; h<currentHeight+5; h++) {
                //printf("\n");
                // Percorre 4 pixeis na largura atual
                for(int w = currentWidth; w<currentWidth+4; w++) {
                    //printf("[%d][%d] ", h, w);
                    //sleep(1);
                    sum = sum + picM[h][w].r;
                }
            }
            sum = sum/20;
            pixel.r = sum;
            pixel.g = sum;
            pixel.b = sum;
            sum = 0;
            
            aspectPixels[currentAspectSize] = pixel;
            currentAspectSize++;
        }
    }

    // // Redimensionar imagem

    // double factor = 0.5;

    // int resizeHeight = aspectHeight*factor;
    // int resizeWidth = aspectWidth*factor;
    // int resizeSize = round(resizeHeight*resizeWidth);

    // Img resize;
    // resize.height = resizeHeight;
    // resize.width = resizeWidth;
    // RGB resizePixels[resizeSize];
    // resize.img = &resizePixels;

    // currentHeight = 0;
    // currentWidth = 0;
    // int currentResizeSize = 0;

    // // for(int i = 0; i<resizeSize; i++) {
    // //     int index = round(i/factor);
    // //     if(index<=aspectSize) {
    // //         resizePixels[currentResizeSize] = aspect.img[index];
    // //     }
    // //     currentResizeSize++;
    // // }

    // // Permite que o vetor de pixeis seja interpretado como uma matriz
    // RGB(*resizeM)[resize.width] = (RGB(*)[resize.width]) resize.img;

    // for(int h = 0; h<resizeHeight; h++) {
    //     for(int w = 0; w<resizeWidth; w++) {
    //         int newH = round(h*factor);
    //         int newW = round(w*factor);
    //         resizeM[h][w] = aspectM[newH][newW];
    //     }
    // }

    // int x1, x2, y1, y2 = 0;
    // x1 = 100;
    // x2 = aspect.height;
    // //y1 = 50;
    // y2 = (factor * x2) / x1;

    

    // for(int i = 0; i<=y2; i++) {

    //     resizePixels[i] = 
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

    // Permite que o vetor de pixeis seja interpretado como uma matriz
    RGB(*out2)[aspect.width] = (RGB(*)[aspect.width]) aspect.img;

    // char chars[7];

    printf("Digite os caracteres desejados para a arte, um por vez seguido de ENTER do mais escuro para o mais claro\n");
    // for(int i = 0; i<=7; i++) {
    //     printf("Digite o caractere número %d: \n", i);
    //     scanf(" %c", &chars[i]);
    //     printf("Carctere: %c \n", chars[i]);
    // }

    // for(int i = 0; i<=7; i++) {
    //     printf("Carctere: %c \n", chars[i]);
    // }

    char c1, c2 ,c3, c4, c5, c6, c7, c8;

    printf("Digite o caractere número 1: \n");
    scanf(" %c", &c1);

    printf("Digite o caractere número 2: \n");
    scanf(" %c", &c2);

    printf("Digite o caractere número 3: \n");
    scanf(" %c", &c3);

    printf("Digite o caractere número 4: \n");
    scanf(" %c", &c4);

    printf("Digite o caractere número 5: \n");
    scanf(" %c", &c5);

    printf("Digite o caractere número 6: \n");
    scanf(" %c", &c6);

    printf("Digite o caractere número 7: \n");
    scanf(" %c", &c7);

    printf("Digite o caractere número 8: \n");
    scanf(" %c", &c8);

    // Imprime os caracteres de acordo com o valor de R armazenado nos pixeis
    for(int h = 0; h<aspect.height; h++) {
        for(int w = 0; w<aspect.width; w++) {
            if(out2[h][w].r < 32) {
                fprintf(arq, "%c", c1);
            } else if(out2[h][w].r < 64) {
                fprintf(arq, "%c", c2);
            } else if(out2[h][w].r < 96) {
                fprintf(arq, "%c", c3);
            } else if(out2[h][w].r < 128) {
                fprintf(arq, "%c", c4);
            } else if(out2[h][w].r < 160) {
                fprintf(arq, "%c", c5);
            } else if(out2[h][w].r < 192) {
                fprintf(arq, "%c", c6);
            } else if(out2[h][w].r < 224) {
                fprintf(arq, "%c", c7);
            } else {
                fprintf(arq, "%c", c8);
            }
        }
        fprintf(arq,"\n");
    }

    // // Imprime os caracteres de acordo com o valor de R armazenado nos pixeis
    // for(int h = 0; h<aspect.height; h++) {
    //     for(int w = 0; w<aspect.width; w++) {
    //         if(out2[h][w].r < 32) {
    //             fprintf(arq,".");
    //         } else if(out2[h][w].r < 64) {
    //             fprintf(arq,":");
    //         } else if(out2[h][w].r < 96) {
    //             fprintf(arq,"c");
    //         } else if(out2[h][w].r < 128) {
    //             fprintf(arq,"o");
    //         } else if(out2[h][w].r < 160) {
    //             fprintf(arq,"C");
    //         } else if(out2[h][w].r < 192) {
    //             fprintf(arq,"O");
    //         } else if(out2[h][w].r < 224) {
    //             fprintf(arq,"8");
    //         } else {
    //             fprintf(arq,"@");
    //         }
    //     }
    //     fprintf(arq,"\n");
    // }

    fprintf(arq,"\n</pre>\n");
    fprintf(arq,"</body>\n");
    fprintf(arq,"</html>\n");

    fclose(arq);
    free(pic.img);
}
