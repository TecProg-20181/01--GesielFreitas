#include <stdio.h>

/* Não alterar as variaveis i, j do for, pois são só auxiliares do for para percorrer uma matrix
* Tirar os ifs pela função que já está no codigo que não está sendo usado.
* Tem alguns lugares operadores ternarios que verifica os valores maximos entre dois numeros, ex: max, e criar min
* Usar o comando gitshash para juntar varios commits em um unico, ou usar o --amend
*/

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    
    /* [width][height][rgb]
     * 0 -> r
     * 1 -> g
     * 2 -> b 
    */

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

Image rotate_image90_right(Image img, int quantas_vezes) {
    Image rotacionada;
    quantas_vezes %= 4;

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (int k = 0; k < quantas_vezes; ++k) {
        for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
            for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
                rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
                rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
                rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
            }
        }
    }

    return rotacionada;
}
Image vertical_mirroring(Image img, int horizontal){
    
    int width = img.w, height = img.h;

    if(horizontal == 1){
        width /= 2;
    }else{
        height /= 2;
    }

    for (int i2 = 0; i2 < height; ++i2) {
        for (int j = 0; j < width; ++j) {
            int x = i2, y = j;

            if (horizontal == 1){
                y = img.w - 1 - j;
            }else{
                x = img.h - 1 - i2;
            }

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


   return img;
}
Image invert_colors(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            img.pixel[i][j][0] = 255 - img.pixel[i][j][0];
            img.pixel[i][j][1] = 255 - img.pixel[i][j][1];
            img.pixel[i][j][2] = 255 - img.pixel[i][j][2];
        }
    }
    return img;
}


Image crop_image(Image img, int x, int y, int w, int h) {
    Image cropped_image;

    cropped_image.w = w;
    cropped_image.h = h;

    for(int i = 0; i < h; ++i) {
        for(int j = 0; j < w; ++j) {
            cropped_image.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cropped_image.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cropped_image.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }

    return cropped_image;
}

Image init_image(Image img){

    // Read type of image
    
    char p3[4];
    scanf("%s", p3);

    // Read width height and color of image
    
    int max_color;
    scanf("%u %u %d", &img.w, &img.h, &max_color);

    // Read all pixels of image
    
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j][0],
                                 &img.pixel[i][j][1],
                                 &img.pixel[i][j][2]);

        }
    }

    return img;
}

Image options_img(Image img){

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {

            // Option of gray scale

            case 1: { 
                img = gray_scale(img);
                return img;
                break;
            }
            case 2: { 

                // Option for sepia filter

                img = sepia_filter(img);
                return img;
                break;
            }
            case 3: { 

                // Option aplicated Blur

                int tamanho = 0;
                scanf("%d", &tamanho);
                img = blur(img, tamanho);
                return img;
                break;
            }
            case 4: { 

                // Option for rotate 90 right

                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                img = rotate_image90_right(img, quantas_vezes);
                return img;
                break;
            }
            case 5: { 

                // Option for vertical mirroring

                int horizontal = 0;
                scanf("%d", &horizontal);
                img = vertical_mirroring(img, horizontal);
                return img;
                break;
            }
            case 6: { 

                // Option for invert colors
                
                img = invert_colors(img);
                return img;
                break;
            }
            case 7: { 

                // Option crop
                
                int x, y;
                scanf("%d %d", &x, &y);
                int w, h;
                scanf("%d %d", &w, &h);

                img = crop_image(img, x, y, w, h);
                return img;
                break;
            }
        }

    }
}


void print_new_image(Image img){

    // Print type of image
    
    printf("P3\n");
    
    // Print width height and color of image
    
    printf("%u %u\n255\n", img.w, img.h);

    // Print pixels of image
    
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j][0],
                                   img.pixel[i][j][1],
                                   img.pixel[i][j][2]);

        }
        printf("\n");
    }
}

int main() {
    
    Image img;

    img = init_image(img);

    img = options_img(img);

    print_new_image(img);
    
    return 0;
}
