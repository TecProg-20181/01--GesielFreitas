#include <stdio.h>
// não alterar as variaveis i, j do for, pois são só auxiliares do for para percorrer uma matrix
// tirar os ifs pela função que já está no codigo que não está sendo usado.
// tem alguns lugares operadores ternarios que verifica os valores maximos entre dois numeros, ex: max, e criar min
// usar o comando gitshash para juntar varios commits em um unico, ou usar o --amend
//
//
typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b   gesiel usar a struct pixel
    unsigned short int pixel[512][512][3];
    unsigned int w;
    unsigned int h;
} Image;


int max(int a, int b) {
    if(a >= b){
        return a;
    }else{
        return b;
    }
}

int min(int a, int b){
    if(a > b){
        return b;
    }else{
        return a;
    }
}

int mult(int a, int b){
    return a * b;
}

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}

float calcula_media_img_pixel(Image img, int i, int j){

    int media = 0;
    media = (img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]) / 3;
    return media;   
}


Image gray_scale(Image img) {
   
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int media = calcula_media_img_pixel(img, i, j);
            
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }

    return img;
}

int calculator_pixel_sepia(float mult1, float mult2, float mult3, Pixel pixel){

    int pixel_calculator = pixel.r * mult1 + pixel.g * mult2 + pixel.b * mult3;

    return pixel_calculator;
}

int calculator_menor_pixel(float mult1, float mult2, float mult3, Pixel pixel){
    
    int pixel_calculator = calculator_pixel_sepia(mult1, mult2, mult3, pixel);

    int pixel_menor = min(255, pixel_calculator);
}

Image sepia_filter(Image img){

    for (unsigned int x = 0; x < img.h; ++x) {
        for (unsigned int j = 0; j < img.w; ++j) {
            Pixel pixel = {0, 0, 0};
            pixel.r = img.pixel[x][j][0];
            pixel.g = img.pixel[x][j][1];
            pixel.b = img.pixel[x][j][2];

            int menor_r = calculator_menor_pixel(0.393, 0.769, 0.189, pixel);
            img.pixel[x][j][0] = menor_r;

            menor_r = calculator_menor_pixel(0.349, 0.686, 0.168, pixel);
            img.pixel[x][j][1] = menor_r;
            
            menor_r = calculator_menor_pixel(0.272, 0.534, 0.131, pixel);
            img.pixel[x][j][2] = menor_r;
        }
    }

    return img;
               
}

Pixel attach_in_media_tamanho(Pixel media, int tamanho){

    int double_tamanho = mult(tamanho, tamanho);    
    
    media.r /= double_tamanho;
    media.g /= double_tamanho;
    media.b /= double_tamanho;

    return media;
}

Image blur(Image img, int tamanho) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            Pixel media = {0, 0, 0};

            int min_h = min(img.h - 1, i + tamanho/2);
            int min_w = min(img.w - 1, j + tamanho/2);
            int max_h = max(0, i - tamanho/2);
            int max_w = max(0, j - tamanho/2);
    
            for(int x = max_h; x <= min_h; ++x) {
                for(int y = max_w; y <= min_w; ++y) {
                    
                    media.r += img.pixel[x][y][0];
                    media.g += img.pixel[x][y][1];
                    media.b += img.pixel[x][y][2];
                }
            } 
    
            media = attach_in_media_tamanho(media, tamanho);

            img.pixel[i][j][0] = media.r;
            img.pixel[i][j][1] = media.g;
            img.pixel[i][j][2] = media.b;
            
        }
    }

    return img;
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotacionada;
}

void inverter_cores(unsigned short int pixel[512][512][3],
                    unsigned int w, unsigned int h) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image cortar_imagem(Image img, int x, int y, int w, int h) {
    Image cortada;

    cortada.w = w;
    cortada.h = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cortada;
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.w, &img.h, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = gray_scale(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia_filter(img);
                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                img = blur(img, tamanho);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int w = img.w, h = img.h;

                if (horizontal == 1) w /= 2;
                else h /= 2;

                for (int i2 = 0; i2 < h; ++i2) {
                    for (int j = 0; j < w; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.w - 1 - j;
                        else x = img.h - 1 - i2;

                        Pixel aux1;
                        aux1.r = img.pixel[i2][j][0];
                        aux1.g = img.pixel[i2][j][1];
                        aux1.b = img.pixel[i2][j][2];

                        img.pixel[i2][j][0] = img.pixel[x][y][0];
                        img.pixel[i2][j][1] = img.pixel[x][y][1];
                        img.pixel[i2][j][2] = img.pixel[x][y][2];

                        img.pixel[x][y][0] = aux1.r;
                        img.pixel[x][y][1] = aux1.g;
                        img.pixel[x][y][2] = aux1.b;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(img.pixel, img.w, img.h);
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                img = cortar_imagem(img, x, y, w, h);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.w, img.h);

    // print pixels of image
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }
    return 0;
}
