#include "search/FrontierFila.hpp"

// Frontier nao e dona dos nos: so guarda ponteiros. Quem aloca e libera os
// NoBusca* e o buscaGenerica (ver BuscaGenerica.cpp) - nada de delete aqui.

void FrontierFila::inserir(NoBusca* no) {
    fila_.push(no);
}

// FIFO: remove o no mais antigo (o primeiro que entrou).
NoBusca* FrontierFila::remover() {
    if (fila_.empty()) return nullptr;
    NoBusca* no = fila_.front();
    fila_.pop();
    return no;
}

bool FrontierFila::vazia() const {
    return fila_.empty();
}

std::size_t FrontierFila::tamanho() const {
    return fila_.size();
}
