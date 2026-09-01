#pragma once

#include "search/IAlgoritmoBusca.hpp"

// Iterative deepening: chama o mesmo BuscaGenerica repetidas vezes, com uma
// FrontierPilha nova a cada rodada, aumentando o limite de profundidade de
// 1 ate profundidadeMaxima_ (God's Number do 2x2x2 = 11 movimentos HTM).
class IDDFS : public IAlgoritmoBusca {
public:
    explicit IDDFS(int profundidadeMaxima = 11) : profundidadeMaxima_(profundidadeMaxima) {}

    ResultadoBusca resolver(const EstadoCubo& estadoInicial) override;
    const char* nome() const override {
        return "Busca em Profundidade Limitada Iterativa (IDDFS)";
    }

private:
    int profundidadeMaxima_;
};
