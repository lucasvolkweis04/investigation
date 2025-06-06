#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> 
#ifdef WIN32
#include <windows.h> 
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>   // Funções da OpenGL
#include <GL/glu.h>  // Funções da GLU
#include <GL/glut.h> // Funções da FreeGLUT
#endif

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel Pixel (24 bits)
typedef struct
{
    unsigned char r, g, b;
} Pixel;

// Uma imagem Pixel
typedef struct
{
    int width, height;
    Pixel *img;
} Img;

// Protótipos
void load(char *name, Img *pic);
void processa();

// Funções da interface gráfica e OpenGL
void init();
void draw();
void keyboard(unsigned char key, int x, int y);

// Largura e altura da janela
int width, height;

// Fator de multiplicação do ruído
int fator;

// Identificadores de textura
GLuint tex[2];

// As 2 imagens
Img pic[2];

// Imagem selecionada (0,1)
int sel;

// Carrega uma imagem para a struct Img
void load(char *name, Img *pic)
{
    int chan;
    pic->img = (Pixel *)SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if (!pic->img)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

int main(int argc, char **argv)
{
    if (argc < 1)
    {
        printf("seeing [im. entrada]\n");
        exit(1);
    }
    glutInit(&argc, argv);

    // Define do modo de operacao da GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    // pic[0] -> imagem de entrada
    // pic[1] -> imagem de saida

    // Carrega a imagem
    load(argv[1], &pic[0]);

    fator = 5;
    if (argc == 3)
        fator = atoi(argv[2]);

    width = pic[0].width;
    height = pic[0].height;

    // A largura e altura da imagem de saída são iguais às da imagem de entrada (0)
    pic[1].width = pic[0].width;
    pic[1].height = pic[0].height;
    pic[1].img = calloc(pic[1].width * pic[1].height, 3); // W x H x 3 bytes (Pixel)

    // Especifica o tamanho inicial em pixels da janela GLUT
    glutInitWindowSize(width, height);

    // Cria a janela passando como argumento o titulo da mesma
    glutCreateWindow("Analise Forense de Imagens");

    // Registra a funcao callback de redesenho da janela de visualizacao
    glutDisplayFunc(draw);

    // Registra a funcao callback para tratamento das teclas ASCII
    glutKeyboardFunc(keyboard);

    // Exibe as dimensões na tela, para conferência
    printf("Entrada  : %s %d x %d\n", argv[1], pic[0].width, pic[0].height);
    sel = 0; // entrada

    // Define a janela de visualizacao 2D
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, width, height, 0.0);
    glMatrixMode(GL_MODELVIEW);

    // Cria texturas em memória a partir dos pixels das imagens
    tex[0] = SOIL_create_OGL_texture((unsigned char *)pic[0].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
    tex[1] = SOIL_create_OGL_texture((unsigned char *)pic[1].img, width, height, SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

    // Aplica o algoritmo
    processa();

    // Entra no loop de eventos, não retorna
    glutMainLoop();
}

void processa() {
    
    Pixel *in = pic[0].img;  // 'in' aponta para a imagem de entrada
    Pixel *out = pic[1].img; // 'out' aponta para a imagem de saída

    int regsize = 3;              
    int num_pixels = regsize * regsize; // Número de pixels na janela 3x3 (9 pixels)
    int r_vals[num_pixels], g_vals[num_pixels], b_vals[num_pixels]; // Arrays para armazenar valores RGB da janela

    // Percorre a imagem para aplicar o filtro
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = 0;  // Índice para preencher os arrays de valores RGB da janela

            // Preenche a janela 3x3 com os pixels ao redor do pixel central (x, y)
            for (int j = -1; j <= 1; j++) { // Variação vertical (-1 a 1)
                for (int i = -1; i <= 1; i++) { // Variação horizontal (-1 a 1)
                    // Cálculo das coordenadas do pixel na janela
                    int ny = y + j;
                    int nx = x + i;

                    // Trata as bordas da imagem (se estiver fora dos limites, usa o valor da borda)
                    if (ny < 0) ny = 0;
                    if (ny >= height) ny = height - 1;
                    if (nx < 0) nx = 0;
                    if (nx >= width) nx = width - 1;

                    // Calcula o índice do pixel na imagem original (array unidimensional)
                    int in_idx = ny * width + nx;

                    // Armazena os valores RGB do pixel atual nos arrays correspondentes
                    r_vals[idx] = in[in_idx].r;
                    g_vals[idx] = in[in_idx].g;
                    b_vals[idx] = in[in_idx].b;
                    idx++;  // Avança para o próximo pixel da janela
                }
            }

            // Ordena os arrays de valores RGB (Bubble Sort) para encontrar a mediana
            for (int i = 0; i < num_pixels - 1; i++) {
                for (int j = 0; j < num_pixels - i - 1; j++) {
                    // Ordenação dos valores de R
                    if (r_vals[j] > r_vals[j + 1]) {
                        int temp = r_vals[j];
                        r_vals[j] = r_vals[j + 1];
                        r_vals[j + 1] = temp;
                    }
                    // Ordenação dos valores de G
                    if (g_vals[j] > g_vals[j + 1]) {
                        int temp = g_vals[j];
                        g_vals[j] = g_vals[j + 1];
                        g_vals[j + 1] = temp;
                    }
                    // Ordenação dos valores de B
                    if (b_vals[j] > b_vals[j + 1]) {
                        int temp = b_vals[j];
                        b_vals[j] = b_vals[j + 1];
                        b_vals[j + 1] = temp;
                    }
                }
            }

            // Calcula a mediana de cada canal de cor (R, G, B)
            int median_r = r_vals[num_pixels / 2]; // Mediana do canal vermelho
            int median_g = g_vals[num_pixels / 2]; // Mediana do canal verde
            int median_b = b_vals[num_pixels / 2]; // Mediana do canal azul

            // Índice do pixel central na imagem (onde aplicaremos a amplificação)
            int central_idx = y * width + x;

            // Calcula a diferença entre o valor do pixel central e a mediana, amplificando o resultado
            int amplified_r = (in[central_idx].r - median_r) * fator;
            int amplified_g = (in[central_idx].g - median_g) * fator;
            int amplified_b = (in[central_idx].b - median_b) * fator;

            // Limita os valores amplificados para que fiquem dentro do intervalo [0, 255]
            out[central_idx].r = (amplified_r < 0) ? 0 : (amplified_r > 255) ? 255 : amplified_r;
            out[central_idx].g = (amplified_g < 0) ? 0 : (amplified_g > 255) ? 255 : amplified_g;
            out[central_idx].b = (amplified_b < 0) ? 0 : (amplified_b > 255) ? 255 : amplified_b;
        }
    }

    // Carrega a nova textura na GPU (parte que não deve ser alterada)
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, out);
}


// Gerencia eventos de teclado
void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    {
        // ESC: libera memória e finaliza
        free(pic[0].img);
        free(pic[1].img);
        exit(1);
    }
    if (key >= '1' && key <= '2')
        // 1-2: seleciona a imagem correspondente (origem ou destino)
        sel = key - '1';

    if(key == '=') {
        fator += 5;
        processa();
    }
    if(key == '-') {
        fator -= 5;
        processa();
    }
    glutPostRedisplay();
}

// Callback de redesenho da tela
void draw()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Preto
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Para outras cores, veja exemplos em /etc/X11/Pixel.txt

    glColor3ub(255, 255, 255); // branco

    // Ativa a textura corresponde à imagem desejada
    glBindTexture(GL_TEXTURE_2D, tex[sel]);
    // E desenha um retângulo que ocupa toda a tela
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(0, 0);

    glTexCoord2f(1, 0);
    glVertex2f(pic[sel].width, 0);

    glTexCoord2f(1, 1);
    glVertex2f(pic[sel].width, pic[sel].height);

    glTexCoord2f(0, 1);
    glVertex2f(0, pic[sel].height);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Exibe a imagem
    glutSwapBuffers();
}
