#pragma once

#include "view/IVisualizador.hpp"

// Desenho ASCII/ANSI do cubo (planificacao em cruz) + menu no terminal.
class VisualizadorTerminal : public IVisualizador {
public:
    void renderizar(const EstadoCubo& estado) override;
    Comando lerComando() override;
    void mostrarMensagem(const std::string& mensagem) override;
};
