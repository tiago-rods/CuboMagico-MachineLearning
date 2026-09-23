#include "search/AEstrela.hpp"

#include <unordered_map>

#include "cubo/Avaliadora.hpp"
#include "cubo/Sucessora.hpp"
#include "search/FrontierPrioridade.hpp"

ResultadoBusca AEstrela::resolver(const EstadoCubo& estadoInicial) {
    FrontierPrioridade fila;

    // Melhor g conhecido por estado. A raiz e registrada aqui, como no BFS.
    std::unordered_map<EstadoCubo, int, HashEstado> melhorG;
    melhorG[estadoInicial] = 0;

    // FuncaoSucessora recebe um unordered_set*, que nao carrega g: a lambda
    // adapta a assinatura e ignora esse parametro (visitados = nullptr abaixo).
    FuncaoSucessora sucessora = [&melhorG](NoBusca* atual,
                                           std::unordered_set<EstadoCubo, HashEstado>*) {
        return sucessoraComMelhorG(atual, melhorG);
    };

    return buscaGenerica(fila, estadoInicial, sucessora, ehEstadoObjetivo, nullptr);
}
