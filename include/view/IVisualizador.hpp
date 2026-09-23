#pragma once

#include <string>

#include "cubo/EstadoCubo.hpp"
#include "view/Comando.hpp"

// Strategy/DIP: o Controller depende so desta interface, nunca de
// VisualizadorTerminal ou VisualizadorOpenGL diretamente.
class IVisualizador {
public:
    virtual ~IVisualizador() = default;

    virtual void renderizar(const EstadoCubo& estado) = 0;
    virtual Comando lerComando() = 0;
    virtual void mostrarMensagem(const std::string& mensagem) = 0;

    // Anima a transicao de um movimento (usado tanto por movimentos manuais
    // quanto pelos passos de uma solucao). Views sem animacao (ex.: terminal)
    // podem deixar vazio - quem redesenha o estado final e' o renderizar().
    virtual void animarMovimento(const Movimento& movimento) = 0;
};
