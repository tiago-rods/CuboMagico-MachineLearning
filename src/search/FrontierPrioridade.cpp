#include "search/FrontierPrioridade.hpp"

// Frontier nao e dona dos nos: so guarda ponteiros. Quem aloca e libera os
// NoBusca* e o buscaGenerica (ver BuscaGenerica.cpp) - nada de delete aqui.

// std::priority_queue sempre remove o MAIOR elemento segundo o comparador.
// Para o menor f = g + h sair primeiro (min-heap), a comparacao e invertida:
// "a vem depois de b" quando a tem f maior.
//
// Empate em f: menor g (profundidade) sai primeiro. Nao e so estetico: a
// sucessora marca estados como visitados na GERACAO, entao o primeiro no que
// gera um estado "trava" o g dele. Priorizando menor g no empate, o no do
// caminho otimo sempre e expandido antes de um no de mesmo f com g maior.
// Nao desempatar por endereco de ponteiro: o resultado deixaria de ser
// reprodutivel entre execucoes.
bool FrontierPrioridade::ComparadorF::operator()(const NoBusca* a, const NoBusca* b) const {
    if (a->f() != b->f()) return a->f() > b->f();
    return a->profundidade > b->profundidade;
}

void FrontierPrioridade::inserir(NoBusca* no) {
    fila_.push(no);
}

// Remove o no de menor f (e, no empate, de menor g).
NoBusca* FrontierPrioridade::remover() {
    if (fila_.empty()) return nullptr;
    NoBusca* no = fila_.top();
    fila_.pop();
    return no;
}

bool FrontierPrioridade::vazia() const {
    return fila_.empty();
}

std::size_t FrontierPrioridade::tamanho() const {
    return fila_.size();
}
