#include "view/terminal/VisualizadorTerminal.hpp"

void VisualizadorTerminal::renderizar(const EstadoCubo& estado) {
    // TODO: desenhar planificacao em cruz (ver CLAUDE.md), cores via ANSI.
}

Comando VisualizadorTerminal::lerComando() {
    // TODO: mostrar menu, ler entrada e mapear para um Comando
    // (numero de opcao ou texto de movimento via parseMovimento).
    return Comando{};
}

void VisualizadorTerminal::mostrarMensagem(const std::string& mensagem) {
    // TODO
}
