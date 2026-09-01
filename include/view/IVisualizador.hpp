#pragma once

#include <string>

#include "cubo/EstadoCubo.h"
#include "view/Comando.h"

// Strategy/DIP: o Controller depende so desta interface, nunca de
// VisualizadorTerminal ou VisualizadorOpenGL diretamente.
class IVisualizador {
public:
    virtual ~IVisualizador() = default;

    virtual void renderizar(const EstadoCubo& estado) = 0;
    virtual Comando lerComando() = 0;
    virtual void mostrarMensagem(const std::string& mensagem) = 0;
};
