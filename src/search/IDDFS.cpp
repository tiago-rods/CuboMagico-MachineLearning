#include "search/IDDFS.hpp"

#include "cubo/Avaliadora.hpp"
#include "cubo/Sucessora.hpp"
#include "search/FrontierPilha.hpp"

ResultadoBusca IDDFS::resolver(const EstadoCubo& estadoInicial) {
    long totalVisitados = 0;

    // Limite 0 so testa a raiz (cubo ja resolvido). Sem 'visitados' de
    // proposito: num DFS, marcar um estado alcancado por um caminho longo
    // bloquearia o caminho curto ate ele e quebraria a otimalidade.
    for (int limite = 0; limite <= profundidadeMaxima_; ++limite) {
        FrontierPilha pilha;
        ResultadoBusca resultado = buscaGenerica(pilha, estadoInicial, sucessoraCubo,
                                                 ehEstadoObjetivo, nullptr, limite);
        totalVisitados += resultado.estadosVisitados;

        if (resultado.encontrou) {
            resultado.estadosVisitados = totalVisitados;
            return resultado;
        }
    }

    ResultadoBusca semSolucao;
    semSolucao.estadosVisitados = totalVisitados;
    return semSolucao;
}
