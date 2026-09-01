#pragma once

#include "view/IVisualizador.hpp"

// TODO (Semana 3): renderizar os 8 cubies em 3D com freeglut (pipeline fixo).
// So compilado quando WITH_OPENGL=ON no CMake.
class VisualizadorOpenGL : public IVisualizador {
public:
    void renderizar(const EstadoCubo& estado) override;
    Comando lerComando() override;
    void mostrarMensagem(const std::string& mensagem) override;
};
