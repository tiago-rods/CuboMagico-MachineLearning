#pragma once

#include <stack>

#include "search/IFrontier.h"
#include "search/NoBusca.h"

// LIFO usada pela Busca em Profundidade Limitada Iterativa (IDDFS).
class FrontierPilha : public IFrontier {
public:
    void inserir(NoBusca* no) override;
    NoBusca* remover() override;
    bool vazia() const override;
    std::size_t tamanho() const override;

private:
    std::stack<NoBusca*> pilha_;
};
