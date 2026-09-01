#include "cubo/EstadoCubo.hpp"

// Dois estados sao iguais se, e somente se, os 24 stickers coincidem na mesma ordem.
bool EstadoCubo::operator==(const EstadoCubo& outro) const {
    return stickers == outro.stickers;
}

// Hash polinomial simples sobre os 24 stickers. So precisa ser consistente com
// operator== (mesmo estado -> mesmo hash); colisoes sao resolvidas pelo
// unordered_set via operator==, entao nao precisa ser criptografico.
std::size_t HashEstado::operator()(const EstadoCubo& estado) const {
    std::size_t hash = 0;
    for(Cor cor : estado.stickers){
        hash = hash * 31 + static_cast<std::size_t>(cor);
    }
    return hash;
}

// Mapeamento fixo face->cor (esquema de cores padrao: branco/amarelo opostos,
// verde/azul opostos, laranja/vermelho opostos). A ordem do array segue
// EstadoCubo.hpp: U, D, F, B, L, R - a mesma ordem usada pelas tabelas de
// permutacao de aplicarMovimento em Sucessora.cpp.
EstadoCubo estadoResolvido() {
    EstadoCubo estado{};
    const Cor corPorFace[6] = {
        Cor::BRANCO, Cor::AMARELO, Cor::VERDE, Cor::AZUL, Cor::LARANJA, Cor::VERMELHO
    };

    for (int face = 0; face < 6; ++face){
        for (int pos = 0; pos < 4; ++pos){
            estado.stickers[face * 4 + pos] = corPorFace[face];
        }
    }
    return estado;
}
