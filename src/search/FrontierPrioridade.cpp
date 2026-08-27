#include "search/FrontierPrioridade.h"

bool FrontierPrioridade::ComparadorF::operator()(const NoBusca* a, const NoBusca* b) const {
    // TODO: menor f = g+h deve ter prioridade (min-heap).
    return false;
}

void FrontierPrioridade::inserir(NoBusca* no) {
    // TODO
}

NoBusca* FrontierPrioridade::remover() {
    // TODO
    return nullptr;
}

bool FrontierPrioridade::vazia() const {
    // TODO
    return true;
}

std::size_t FrontierPrioridade::tamanho() const {
    // TODO
    return 0;
}
