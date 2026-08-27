#include "search/BuscaGenerica.h"

ResultadoBusca buscaGenerica(IFrontier& estrutura,
                              const EstadoCubo& estadoInicial,
                              const FuncaoSucessora& sucessora,
                              const FuncaoAvaliadora& ehObjetivo,
                              std::unordered_set<EstadoCubo, HashEstado>* visitados,
                              int limiteProfundidade) {
    // TODO: implementar o laco generico (requisito critico do trabalho):
    //   Adicionar estado inicial na estrutura
    //   Enquanto a estrutura nao estiver vazia:
    //       Remover proximo estado da estrutura
    //       Avaliar estado -> se final, retornar solucao
    //       Adicionar estados seguintes na estrutura
    //   Retornar "Sem solucao"
    // BFS, IDDFS e A* devem chamar esta MESMA funcao, variando so a IFrontier.
    return ResultadoBusca{};
}
