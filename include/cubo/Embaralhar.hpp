#pragma once

#include "cubo/EstadoCubo.hpp"

// Parte de estadoResolvido() e aplica nMovimentos giros aleatorios (escolhidos
// deterministicamente a partir de seed) para gerar um cubo embaralhado.
// Mesma seed + mesmo nMovimentos = sempre o mesmo resultado, em qualquer
// compilador (usa std::mt19937 + modulo, nunca std::uniform_int_distribution,
// cujo algoritmo interno varia entre bibliotecas).
EstadoCubo embaralhar(int nMovimentos, unsigned seed);
