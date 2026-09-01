#pragma once

#include "cubo/EstadoCubo.hpp"

// Funcao objetivo: cada face do cubo tem uma unica cor?
bool ehEstadoObjetivo(const EstadoCubo& estado);

// Heuristica admissivel para o A*: ceil(cantos incorretos / 4), pois cada
// giro de face move exatamente 4 dos 8 cantos (ver justificativa no CLAUDE.md).
int heuristicaCantos(const EstadoCubo& estado);
