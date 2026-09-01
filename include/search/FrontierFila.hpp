#pragma once

#include <queue>

#include "search/IFrontier.hpp"
#include "search/NoBusca.hpp"

// FIFO usada pela Busca em Largura (BFS).
class FrontierFila : public IFrontier {
public:
    void inserir(NoBusca* no) override;
    NoBusca* remover() override;
    bool vazia() const override;
    std::size_t tamanho() const override;

private:
    std::queue<NoBusca*> fila_;
};
