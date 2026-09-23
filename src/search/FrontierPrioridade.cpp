#include "search/FrontierPrioridade.hpp"

// Frontier nao e dona dos nos: so guarda ponteiros. Quem aloca e libera os
// NoBusca* e o buscaGenerica (ver BuscaGenerica.cpp) - nada de delete aqui.

// std::priority_queue sempre remove o MAIOR elemento segundo o comparador.
// Para o menor f = g + h sair primeiro (min-heap), a comparacao e invertida:
// "a vem depois de b" quando a tem f maior.
//
// Empate em f: MAIOR g (profundidade) sai primeiro, ou seja, mergulha em
// direcao ao objetivo em vez de varrer o nivel. Nao e so estetico: como
// heuristicaCantos so vale 0, 1 ou 2, quase todo no empata em f.
//
// O A* nao usa o set de visitados (passa nullptr): ele chama
// sucessoraComMelhorG, que guarda o melhor g por estado e regera um estado
// quando aparece um caminho com g menor. Por isso nao existe o risco de um no
// de g maior "travar" um estado com g subotimo, e o desempate pode priorizar
// o g maior. Medido (Pessoa B, 19-09-2026): maior g visita ~metade dos estados
// (scramble de 7 movimentos: 1.625 contra 3.161), com a mesma solucao.
//
// Nao desempatar por endereco de ponteiro: o resultado deixaria de ser
// reprodutivel entre execucoes.
bool FrontierPrioridade::ComparadorF::operator()(const NoBusca* a, const NoBusca* b) const {
    if (a->f() != b->f()) return a->f() > b->f();
    return a->profundidade < b->profundidade;
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
