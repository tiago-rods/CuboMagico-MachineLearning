#include "view/opengl/RenderCubie.hpp"
#include <GL/freeglut.h>

namespace {
    //mesma ordem de cantos do README
constexpr int stickersPorCanto[8][3] = {
    {2, 8, 17},   // ULF
    {3, 9, 20},   // URF
    {4, 10, 19},  // DLF
    {5, 11, 22},  // DRF
    {0, 13, 16},  // ULB
    {1, 12, 21},  // URB
    {6, 15, 18},  // DLB
    {7, 14, 23},  // DRB
    };

constexpr float sinaisPorCanto[8][3] = {
    //sinais dos eixos (x,y,z) para cada canto , tem que bater com posicoes cantosdo visualizadorOpenGL
    {-1.0f, +1.0f, +1.0f}, // ULF
    {+1.0f, +1.0f, +1.0f}, // URF
    {-1.0f, -1.0f, +1.0f}, // DLF
    {+1.0f, -1.0f, +1.0f}, // DRF
    {-1.0f, +1.0f, -1.0f}, // ULB
    {+1.0f, +1.0f, -1.0f}, // URB
    {-1.0f, -1.0f, -1.0f}, // DLB
    {+1.0f, -1.0f, -1.0f}, // DRB
    };

void corParaRGB(Cor cor, float& r, float& g, float& b){
    switch(cor) {
        case Cor::BRANCO: r = 1.0f; g = 1.0f; b = 1.0f; break;
        case Cor::AMARELO:  r = 1.0f; g = 1.0f; b = 0.0f; break;
        case Cor::VERDE:    r = 0.0f; g = 0.8f; b = 0.0f; break;
        case Cor::AZUL:     r = 0.0f; g = 0.0f; b = 1.0f; break;
        case Cor::LARANJA:  r = 1.0f; g = 0.5f; b = 0.0f; break;
        case Cor::VERMELHO: r = 1.0f; g = 0.0f; b = 0.0f; break;
    }
}

}
void desenharCubie(int indiceCanto, const EstadoCubo& estado) {
    const float meiaAresta = 0.49f; //metade do tamanho do cubie
    const float raio = 0.4f; //metade do lado do quadrado colorido
    const float eps = 0.001f; //evita z-fighting com corpo do cubie

    // Corpo do cubie (plastico) numa cor neutra
    glColor3f(0.05f, 0.05f, 0.05f);
    glutSolidCube(meiaAresta * 2.0f);

    float sinalX = sinaisPorCanto[indiceCanto][0];
    float sinalY = sinaisPorCanto[indiceCanto][1];
    float sinalZ = sinaisPorCanto[indiceCanto][2];

    int indiceUD = stickersPorCanto[indiceCanto][0];
    int indiceFB = stickersPorCanto[indiceCanto][1];
    int indiceLR = stickersPorCanto[indiceCanto][2];

    float r, g, b;

    // Face U/D (plano y = sinalY * (meiaAresta + eps))
    corParaRGB(estado.stickers[indiceUD], r, g, b);
    glColor3f(r,g,b);
    float y = sinalY * (meiaAresta + eps);
    glBegin(GL_QUADS);
    glVertex3f(-raio, y, -raio);
    glVertex3f( raio, y, -raio);
    glVertex3f( raio, y,  raio);
    glVertex3f(-raio, y,  raio);

glEnd();

// Face F/B (plano z = sinalZ * (meiaAresta+eps))
    corParaRGB(estado.stickers[indiceUD], r, g, b);
    glColor3f(r,g,b);
    float z = sinalZ * (meiaAresta + eps);
    glBegin(GL_QUADS);
    glVertex3f(-raio, -raio, z);
    glVertex3f( raio, -raio, z);
    glVertex3f( raio,  raio, z);
    glVertex3f( raio, -raio, z);

glEnd();

    // Face L/R (plano x = sinalX * (meiaAresta + eps))
    corParaRGB(estado.stickers[indiceUD], r, g, b);
    glColor3f(r,g,b);
    float x = sinalX * (meiaAresta + eps);
    glBegin(GL_QUADS);
    glVertex3f(x, -raio, -raio);
    glVertex3f(x,  raio, -raio);
    glVertex3f(x,  raio,  raio);
    glVertex3f(x, -raio,  raio);

glEnd();

}

