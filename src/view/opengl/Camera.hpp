#pragma once

// TODO (Semana 3): camera orbital controlada por arraste do mouse.
class Camera {
public:
    void aplicar() const; // chama gluLookAt/glRotatef antes de desenhar a cena
};
