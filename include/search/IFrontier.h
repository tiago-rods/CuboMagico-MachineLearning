#pragma once

#include <cstddef>

struct NoBusca;

// Abstracao (padrao Bridge) que permite ao laco generico (BuscaGenerica)
// funcionar identicamente para fila (BFS), pilha (IDDFS) e fila de
// prioridade (A*), sem conhecer a estrutura de dados concreta.
class IFrontier {
public:
    virtual ~IFrontier() = default;

    virtual void inserir(NoBusca* no) = 0;
    virtual NoBusca* remover() = 0;
    virtual bool vazia() const = 0;
    virtual std::size_t tamanho() const = 0;
};
