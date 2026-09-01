#pragma once

#include "cubo/EstadoCubo.hpp"
#include "cubo/Movimento.hpp"

struct NoBusca {
    EstadoCubo estado;
    NoBusca* pai = nullptr;
    Movimento movimentoAplicado{};
    int profundidade = 0; // g
    int h = 0;            // usado apenas pelo A*

    int f() const { return profundidade + h; }
};
