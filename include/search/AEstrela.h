#pragma once

#include "search/IAlgoritmoBusca.h"

class AEstrela : public IAlgoritmoBusca {
public:
    ResultadoBusca resolver(const EstadoCubo& estadoInicial) override;
    const char* nome() const override { return "Busca A* (heuristica: cantos incorretos)"; }
};
