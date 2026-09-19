#include "search/BFS.hpp"

#include "cubo/Avaliadora.hpp"
#include "cubo/Sucessora.hpp"
#include "search/FrontierFila.hpp"

ResultadoBusca BFS::resolver(const EstadoCubo& estadoInicial) {
    FrontierFila fila;

    // Valor inicial; ajustar depois de medir ate que profundidade de
    // embaralhamento o BFS responde em tempo aceitavel.
    std::unordered_set<EstadoCubo, HashEstado> visitados;
    visitados.reserve(1 << 20);
    // A sucessora so marca os filhos; a raiz e marcada aqui (docs/decisoes-pessoa-b.md).
    visitados.insert(estadoInicial);

    return buscaGenerica(fila, estadoInicial, sucessoraCubo, ehEstadoObjetivo, &visitados);
}
