#include "view/opengl/VisualizadorOpenGL.hpp"
#include "cubo/EstadoCubo.hpp"

// Executavel temporario pra validar janela/camera antes da integracao com o
// Controller (Fase D.6). Nao entra no cubo_magico final.
int main() {
    VisualizadorOpenGL view;
    EstadoCubo estado = estadoResolvido();

    // glutMainLoopEvent() so processa 1 rodada de eventos por chamada, entao
    // precisamos ficar chamando renderizar() em loop pra janela continuar viva.
    while (true) {
        view.renderizar(estado);
    }
    return 0;
}