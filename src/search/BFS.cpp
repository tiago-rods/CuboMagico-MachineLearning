#include "search/BFS.hpp"

#include "cubo/Avaliadora.hpp"
#include "cubo/Sucessora.hpp"
#include "search/FrontierFila.hpp"

ResultadoBusca BFS::resolver(const EstadoCubo& estadoInicial) {
    FrontierFila fila;

    // Valor inicial; ajustar depois de medir ate que profundidade de
    // embaralhamento o BFS responde em tempo aceitavel.
    // Entra vazio de proposito: buscaGenerica exige isso (assert) e ja insere
    // o estado inicial nele.
    std::unordered_set<EstadoCubo, HashEstado> visitados;
    visitados.reserve(1 << 20);

    return buscaGenerica(fila, estadoInicial, sucessoraCubo, ehEstadoObjetivo, &visitados);
}
