#include "app/Controller.hpp"

#include <random>
#include <sstream>

#include "app/FactoryAlgoritmo.hpp"
#include "app/FactoryVisualizador.hpp"
#include "cubo/Avaliadora.hpp"
#include "cubo/Embaralhar.hpp"
#include "cubo/Sucessora.hpp"

Controller::Controller(std::unique_ptr<IVisualizador> visualizador)
    : visualizador_(std::move(visualizador)) {
    estadoAtual_ = estadoResolvido();
}

void Controller::executar() {
    visualizador_->mostrarMensagem(
        "Cubo Magico 2x2x2 - comandos:\n"
        "  Movimento: U, U', U2, D, D', D2, L, L', L2, R, R', R2, F, F', F2, B, B', B2\n"
        "  embaralhar N [seed]  - embaralha com N movimentos (seed opcional)\n"
        "  bfs | iddfs | astar  - pede pra IA resolver o cubo atual\n"
        "  estado                - redesenha o cubo\n"
        "  trocar terminal|opengl - troca a interface\n"
        "  sair                  - encerra o programa");

    bool continuar = true;
    while (continuar) {
        visualizador_->renderizar(estadoAtual_);
        Comando comando = visualizador_->lerComando();

        switch (comando.tipo) {
            case TipoComando::MOVIMENTO:
                tratarMovimento(comando);
                break;
            case TipoComando::EMBARALHAR:
                tratarEmbaralhar(comando.argumento);
                break;
            case TipoComando::RESOLVER_BFS:
                tratarResolver(criarAlgoritmo(TipoBusca::BFS));
                break;
            case TipoComando::RESOLVER_IDDFS:
                tratarResolver(criarAlgoritmo(TipoBusca::IDDFS));
                break;
            case TipoComando::RESOLVER_ASTAR:
                tratarResolver(criarAlgoritmo(TipoBusca::ASTAR));
                break;
            case TipoComando::MOSTRAR_ESTADO:
                break; // o loop ja redesenha a cada iteracao
            case TipoComando::TROCAR_VIEW:
                tratarTrocarView(comando.argumento);
                break;
            case TipoComando::SAIR:
                continuar = false;
                break;
            case TipoComando::INVALIDO:
                visualizador_->mostrarMensagem("Comando invalido: \"" + comando.argumento + "\"");
                break;
        }
    }
}

void Controller::tratarMovimento(const Comando& comando) {
    animarEAplicar(comando.movimento);

    if (ehEstadoObjetivo(estadoAtual_)) {
        visualizador_->mostrarMensagem("Parabens! O cubo foi resolvido!");
    }
}

void Controller::animarEAplicar(const Movimento& movimento) {
    visualizador_->animarMovimento(movimento);
    estadoAtual_ = aplicarMovimento(estadoAtual_, movimento);
}

void Controller::tratarEmbaralhar(const std::string& argumento) {
    std::istringstream entrada(argumento);
    int nMovimentos = 0;
    unsigned seed = 0;

    entrada >> nMovimentos;
    if (nMovimentos <= 0) {
        visualizador_->mostrarMensagem(
            "Uso: embaralhar N [seed]  (sugestao: N entre 6 e 8)");
        return;
    }

    if (!(entrada >> seed)) {
        seed = std::random_device{}();
    }

    estadoAtual_ = embaralhar(nMovimentos, seed);
    visualizador_->mostrarMensagem(
        "Cubo embaralhado com " + std::to_string(nMovimentos) +
        " movimentos (seed=" + std::to_string(seed) +
        "). Use \"embaralhar " + std::to_string(nMovimentos) + " " + std::to_string(seed) +
        "\" para repetir este mesmo embaralhamento.");
}

void Controller::tratarResolver(std::unique_ptr<IAlgoritmoBusca> algoritmo) {
    ResultadoBusca resultado = algoritmo->resolver(estadoAtual_);

    if (!resultado.encontrou) {
        visualizador_->mostrarMensagem(
            std::string(algoritmo->nome()) + ": sem solucao (" +
            std::to_string(resultado.estadosVisitados) + " estados visitados).");
        return;
    }

    std::string passos;
    for (const Movimento& movimento : resultado.caminho) {
        if (!passos.empty()) passos += " ";
        passos += movimento.toString();
    }
    if (passos.empty()) passos = "(cubo ja estava resolvido)";

    visualizador_->mostrarMensagem(
        std::string(algoritmo->nome()) + " encontrou solucao em " +
        std::to_string(resultado.caminho.size()) + " movimentos, visitando " +
        std::to_string(resultado.estadosVisitados) + " estados:\n  " + passos);

    for (const Movimento& movimento : resultado.caminho) {
        animarEAplicar(movimento);
        visualizador_->renderizar(estadoAtual_);
    }
}

void Controller::tratarTrocarView(const std::string& argumento) {
    TipoView tipo;
    if (argumento == "opengl") {
        tipo = TipoView::OPENGL;
    } else if (argumento == "terminal") {
        tipo = TipoView::TERMINAL;
    } else {
        visualizador_->mostrarMensagem("Uso: trocar terminal|opengl");
        return;
    }

    std::unique_ptr<IVisualizador> novoVisualizador = criarVisualizador(tipo);
    if (novoVisualizador == nullptr) {
        visualizador_->mostrarMensagem(
            "View \"" + argumento + "\" indisponivel neste build (compilado sem -DWITH_OPENGL=ON?).");
        return;
    }

    visualizador_ = std::move(novoVisualizador);
}
