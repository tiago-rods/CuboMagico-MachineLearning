#include "view/opengl/VisualizadorOpenGL.hpp"
#include "cubo/EstadoCubo.hpp"
#include "cubo/Sucessora.hpp"

// Executavel temporario pra validar janela/camera/animacao antes da
// integracao com o Controller (Fase D.6). Nao entra no cubo_magico final.
int main() {
    VisualizadorOpenGL view;
    EstadoCubo estado = estadoResolvido();

    while (true) {
        view.renderizar(estado);

        Comando comando = view.lerComando();
        if (comando.tipo == TipoComando::MOVIMENTO) {
            view.animarMovimento(comando.movimento);
            estado = aplicarMovimento(estado, comando.movimento);
        }
    }
    return 0;
}
