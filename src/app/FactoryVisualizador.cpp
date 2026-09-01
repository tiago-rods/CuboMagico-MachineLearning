#include "app/FactoryVisualizador.hpp"

std::unique_ptr<IVisualizador> criarVisualizador(TipoView tipo) {
    // TODO: instanciar VisualizadorTerminal ou VisualizadorOpenGL (este
    // ultimo so existe quando compilado com WITH_OPENGL=ON, ver COM_OPENGL).
    return nullptr;
}
