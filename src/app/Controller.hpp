#pragma once

#include <memory>

#include "cubo/EstadoCubo.hpp"
#include "search/IAlgoritmoBusca.hpp"
#include "view/IVisualizador.hpp"

// Depende so de abstracoes (IVisualizador, IAlgoritmoBusca) - nunca conhece
// VisualizadorTerminal/VisualizadorOpenGL nem BFS/IDDFS/AEstrela diretamente (DIP).
class Controller {
public:
    explicit Controller(std::unique_ptr<IVisualizador> visualizador);

    void executar(); // loop principal do menu

private:
    void tratarMovimento(const Comando& comando);
    void tratarEmbaralhar(const std::string& argumento);
    void tratarResolver(std::unique_ptr<IAlgoritmoBusca> algoritmo);
    void tratarTrocarView(const std::string& argumento);
    void animarEAplicar(const Movimento& movimento);

    std::unique_ptr<IVisualizador> visualizador_;
    EstadoCubo estadoAtual_;
};
