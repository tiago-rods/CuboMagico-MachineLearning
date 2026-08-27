#include "app/Controller.h"

Controller::Controller(std::unique_ptr<IVisualizador> visualizador)
    : visualizador_(std::move(visualizador)) {
    // TODO: inicializar estadoAtual_ = estadoResolvido()
}

void Controller::executar() {
    // TODO: loop principal - renderizar, ler Comando, despachar para
    // tratarMovimento/tratarEmbaralhar/tratarResolver conforme TipoComando.
}

void Controller::tratarMovimento(const Comando& comando) {
    // TODO: estadoAtual_ = aplicarMovimento(estadoAtual_, comando.movimento);
}

void Controller::tratarEmbaralhar() {
    // TODO: pedir quantidade de movimentos (sugestao 6-8) e seed opcional.
}

void Controller::tratarResolver(std::unique_ptr<IAlgoritmoBusca> algoritmo) {
    // TODO: chamar algoritmo->resolver(estadoAtual_) e mostrar estados
    // visitados + sequencia de movimentos via visualizador_->mostrarMensagem.
}
