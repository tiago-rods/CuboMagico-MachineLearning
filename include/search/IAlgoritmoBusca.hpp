#pragma once

#include "cubo/EstadoCubo.h"
#include "search/BuscaGenerica.h"

// Strategy: cada algoritmo de busca (BFS, IDDFS, A*) implementa esta
// interface. O Controller so conhece IAlgoritmoBusca, nunca as classes
// concretas diretamente.
class IAlgoritmoBusca {
public:
    virtual ~IAlgoritmoBusca() = default;

    virtual ResultadoBusca resolver(const EstadoCubo& estadoInicial) = 0;
    virtual const char* nome() const = 0;
};
