#pragma once

#include <functional>
#include <unordered_set>
#include <vector>

#include "cubo/EstadoCubo.hpp"
#include "cubo/Movimento.hpp"
#include "search/IFrontier.hpp"
#include "search/NoBusca.hpp"

struct ResultadoBusca {
    bool encontrou = false;
    std::vector<Movimento> caminho;
    long estadosVisitados = 0;
    EstadoCubo estadoFinal{};
};

using FuncaoSucessora = std::function<std::vector<NoBusca*>(
    NoBusca*, std::unordered_set<EstadoCubo, HashEstado>*)>;
using FuncaoAvaliadora = std::function<bool(const EstadoCubo&)>;

// O laco generico exigido pelo trabalho. NAO deve ser alterado para se
// adequar a um algoritmo especifico: BFS, IDDFS e A* chamam esta mesma
// funcao, variando apenas a implementacao de IFrontier recebida.
//
//   Adicionar estado inicial na estrutura
//   Enquanto a estrutura nao estiver vazia:
//       Remover proximo estado da estrutura
//       Avaliar estado -> se final, mostrar solucao e encerrar
//       Adicionar estados seguintes na estrutura
//   Retornar "Sem solucao"
ResultadoBusca buscaGenerica(IFrontier& estrutura,
                              const EstadoCubo& estadoInicial,
                              const FuncaoSucessora& sucessora,
                              const FuncaoAvaliadora& ehObjetivo,
                              std::unordered_set<EstadoCubo, HashEstado>* visitados = nullptr,
                              int limiteProfundidade = -1);
