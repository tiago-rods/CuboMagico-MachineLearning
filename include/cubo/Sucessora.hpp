#pragma once

#include <unordered_set>
#include <vector>

#include "cubo/EstadoCubo.hpp"
#include "cubo/Movimento.hpp"
#include "search/NoBusca.hpp"

// Pura: aplica um movimento e retorna o novo estado, sem efeitos colaterais.
EstadoCubo aplicarMovimento(const EstadoCubo& estado, Movimento movimento);

// Gera os nos filhos de 'atual' (ate 18 movimentos), podando o movimento
// inverso ao ultimo aplicado. Se 'visitados' != nullptr, descarta estados
// ja visitados (usado por BFS/A*); para IDDFS, passar nullptr.
std::vector<NoBusca*> sucessoraCubo(NoBusca* atual,
                                     std::unordered_set<EstadoCubo, HashEstado>* visitados);

// Igual a sucessoraCubo, mas tambem calcula o campo 'h' de cada filho via
// heuristicaCantos (usado pelo A*).
std::vector<NoBusca*> sucessoraComHeuristica(NoBusca* atual,
                                              std::unordered_set<EstadoCubo, HashEstado>* visitados);
