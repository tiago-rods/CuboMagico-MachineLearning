#pragma once

#include <queue>
#include <vector>

#include "search/IFrontier.h"
#include "search/NoBusca.h"

// Fila de prioridade usada pelo A*, ordenada por f = g + h (menor primeiro).
class FrontierPrioridade : public IFrontier {
public:
    void inserir(NoBusca* no) override;
    NoBusca* remover() override;
    bool vazia() const override;
    std::size_t tamanho() const override;

private:
    struct ComparadorF {
        bool operator()(const NoBusca* a, const NoBusca* b) const;
    };

    std::priority_queue<NoBusca*, std::vector<NoBusca*>, ComparadorF> fila_;
};
