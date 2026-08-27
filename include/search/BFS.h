#pragma once

#include "search/IAlgoritmoBusca.h"

class BFS : public IAlgoritmoBusca {
public:
    ResultadoBusca resolver(const EstadoCubo& estadoInicial) override;
    const char* nome() const override { return "Busca em Largura (BFS)"; }
};
