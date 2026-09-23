#include "search/FrontierPilha.hpp"

// Frontier nao e dona dos nos: so guarda ponteiros. Quem aloca e libera os
// NoBusca* e o buscaGenerica (ver BuscaGenerica.cpp) - nada de delete aqui.

void FrontierPilha::inserir(NoBusca* no) {
    pilha_.push(no);
}

// LIFO: remove o no mais recente (o ultimo que entrou) -> aprofunda primeiro.
NoBusca* FrontierPilha::remover() {
    if (pilha_.empty()) return nullptr;
    NoBusca* no = pilha_.top();
    pilha_.pop();
    return no;
}

bool FrontierPilha::vazia() const {
    return pilha_.empty();
}

std::size_t FrontierPilha::tamanho() const {
    return pilha_.size();
}
