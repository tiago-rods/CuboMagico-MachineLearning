#include "search/BuscaGenerica.hpp"

#include <algorithm>
#include <cassert>

namespace {

// Dono de todos os NoBusca* de uma chamada de buscaGenerica.
//
// Invariante: todo no alocado (a raiz aqui, os filhos em Sucessora.cpp) esta,
// a qualquer momento, em exatamente UM destes lugares:
//   - dentro da estrutura (ainda nao foi removido);
//   - em 'pais' (ja foi removido e gerou pelo menos um filho);
//   - sendo o 'no' atual do laco;
//   - ja liberado (foi removido e nao gerou nenhum filho).
// O destrutor libera tudo o que sobrou, entao os dois jeitos de sair do laco
// (achou solucao / esgotou a estrutura) nao precisam de codigo de limpeza, e
// uma excecao (ex: bad_alloc numa busca grande) tambem nao vaza a arvore.
struct DonoDosNos {
    IFrontier& estrutura;
    std::vector<NoBusca*> pais;

    explicit DonoDosNos(IFrontier& e) : estrutura(e) {}

    ~DonoDosNos() {
        while (!estrutura.vazia()) delete estrutura.remover();
        for (NoBusca* no : pais) delete no;
    }

    DonoDosNos(const DonoDosNos&) = delete;
    DonoDosNos& operator=(const DonoDosNos&) = delete;
};

// Sobe pelos ponteiros 'pai' do no objetivo ate a raiz e devolve os
// movimentos na ordem em que devem ser aplicados (raiz -> objetivo).
std::vector<Movimento> reconstruirCaminho(const NoBusca* objetivo) {
    std::vector<Movimento> caminho;
    for (const NoBusca* no = objetivo; no->pai != nullptr; no = no->pai) {
        caminho.push_back(no->movimentoAplicado);
    }
    std::reverse(caminho.begin(), caminho.end());
    return caminho;
}

} // namespace

// Pre-condicao: 'visitados' e nullptr (IDDFS) ou chega VAZIO. Um set
// reaproveitado entre rodadas do IDDFS esconderia os estados da rodada
// anterior e a busca responderia "sem solucao" para um cubo soluvel.
//
// limiteProfundidade: -1 = sem limite. Se >= 0, nos com profundidade >= limite
// sao avaliados mas nao expandidos (solucoes de ate 'limite' movimentos sao
// encontradas).
//
// estadosVisitados = quantos estados foram removidos e avaliados NESTA chamada.
ResultadoBusca buscaGenerica(IFrontier& estrutura,
                              const EstadoCubo& estadoInicial,
                              const FuncaoSucessora& sucessora,
                              const FuncaoAvaliadora& ehObjetivo,
                              std::unordered_set<EstadoCubo, HashEstado>* visitados,
                              int limiteProfundidade) {
    assert(visitados == nullptr || visitados->empty());

    ResultadoBusca resultado;
    resultado.estadoFinal = estadoInicial;
    DonoDosNos dono(estrutura);

    // Adicionar estado inicial na estrutura
    NoBusca* raiz = new NoBusca();
    raiz->estado = estadoInicial;
    if (visitados != nullptr) visitados->insert(estadoInicial);
    estrutura.inserir(raiz);

    // Enquanto a estrutura nao estiver vazia:
    while (!estrutura.vazia()) {
        // Remover proximo estado da estrutura
        NoBusca* no = estrutura.remover();
        ++resultado.estadosVisitados;

        // Avaliar estado -> SE estado final: mostrar solucao e encerrar
        if (ehObjetivo(no->estado)) {
            resultado.encontrou = true;
            resultado.caminho = reconstruirCaminho(no);
            resultado.estadoFinal = no->estado;
            delete no; // os ancestrais estao em dono.pais e saem no destrutor
            return resultado;
        }

        // Adicionar estados seguintes na estrutura
        bool atingiuLimite = limiteProfundidade >= 0 && no->profundidade >= limiteProfundidade;
        std::vector<NoBusca*> filhos;
        if (!atingiuLimite) filhos = sucessora(no, visitados);

        if (filhos.empty()) {
            // Ninguem aponta para 'no': 'pai' so e escrito na criacao de um
            // filho (Sucessora.cpp), e este no nao gerou nenhum. Seguro enquanto
            // nenhum codigo trocar o 'pai' de um no ja criado - se isso mudar
            // (ex: reabrir nos no A*), aqui passa a precisar de contagem de
            // referencias.
            delete no;
            continue;
        }
        dono.pais.push_back(no);
        for (NoBusca* filho : filhos) estrutura.inserir(filho);
    }

    // Retornar "Sem solucao"
    return resultado;
}
