#pragma once

class Camera {
public:
    void aplicar() const; // chama gluLookAt/glRotatef antes de desenhar a cena
    void arrastar(int deltaX, int deltaY); //chamado pelo callback de motion do mouse
    void zoom(float delta);
private:
    float anguloHorizontal_ = 45.0f;
    float anguloVertical_ = 30.0f;
    float distancia_ = 8.0f;
};
