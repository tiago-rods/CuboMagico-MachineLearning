#include "view/opengl/Camera.hpp"
#include <cmath>
#include <algorithm>
#include <GL/freeglut.h>

namespace{
    constexpr float PI = 3.14150265f;
    constexpr float SENSIBILIDADE = 0.5f;
}

void Camera::aplicar() const {
    float radH = anguloHorizontal_ * PI / 180.0f;
    float radV = anguloVertical_ * PI / 180.0f;

    float olhoX = distancia_ * std::cos(radV) * std::sin(radH);
    float olhoY = distancia_ * std::sin(radV);
    float olhoZ = distancia_ * std::cos(radV) * std::cos(radH);

    gluLookAt(olhoX, olhoY, olhoZ, // posicao da camera
            0.0f, 0.0f, 0.0f, // olhando pro centro do cubo
            0.0f, 1.0f, 0.0f); // Pra cima e o eixo Y
}

void Camera::arrastar(int deltaX, int deltaY){
    anguloHorizontal_ += deltaX * SENSIBILIDADE;
    anguloVertical_ = std::clamp(anguloVertical_ + deltaY * SENSIBILIDADE, -89.0f, 89.0f); // se o valor for maior que 89, fica em 89, se for menor que -89, fica em -89, qualquer coisa entre, mantém o valor
}

void Camera::zoom(float delta){
    distancia_ = std::clamp(distancia_ + delta, 3.0f, 12.0f);
}
