#pragma once

#include "search/IAlgoritmoBusca.hpp"

class BFS : public IAlgoritmoBusca {
public:
    ResultadoBusca resolver(const EstadoCubo& estadoInicial) override;
    const char* nome() const override { return "Busca em Largura (BFS)"; }
};
