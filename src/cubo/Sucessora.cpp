#include "cubo/Sucessora.hpp"

namespace{
// Um ciclo de 4 indices: o conteudo anda indices[0] -> indices[1] -> indices[2] -> indices[3] -> indices[0].
    struct CicloPermutacao {
        std::array<int, 4> indices;
    };
    
    struct TabelaFace{
        CicloPermutacao proprios;
        CicloPermutacao lateral1;
        CicloPermutacao lateral2;
    };

    const TabelaFace& tabelaFace(Face face){
        static const TabelaFace tabelas[6] = {
            /* U */ {{{0, 1, 3, 2}},     {{8, 16, 12, 20}}, {{9, 17, 13, 21}}},
            /* D */ {{{4, 5, 7, 6}},     {{10, 22, 14, 18}},{{19, 11, 23, 15}}},
            /* L */ {{{16, 17, 19, 18}}, {{0, 8, 4, 15}},   {{13, 2, 10, 6}}},
            /* R */ {{{20, 21, 23, 22}}, {{9, 1, 14, 5}},   {{3, 12, 7, 11}}},
            /* F */ {{{8, 9, 11, 10}},   {{2, 20, 5, 19}},  {{17, 3, 22, 4}}},
            /* B */ {{{12, 13, 15, 14}}, {{1, 16, 6, 23}},  {{21, 0, 18, 7}}},
        };
        return tabelas[static_cast<int>(face)];
    }

    // Roda um ciclo de 4 uma vez, em cima do proprio array sem copia extra.
    // A ordem das atribuicoes e de tras pra frente ([3]<-[2]<-[1]<-[0]<-guardado)
    // porque cada posicao so pode ser sobrescrita DEPOIS que seu valor antigo ja
    // foi copiado pra frente; escrever na ordem direta ([0] primeiro) perderia o
    // valor original de [0] antes de [1] conseguir le-lo. So a ultima escrita
    // (em [0]) usa o valor guardado no inicio, fechando o ciclo.
    void aplicaCicloHorario(std::array<Cor, 24>& stickers, const CicloPermutacao& ciclo){
        Cor guardado = stickers[ciclo.indices[3]];
        stickers[ciclo.indices[3]] = stickers[ciclo.indices[2]];
        stickers[ciclo.indices[2]] = stickers[ciclo.indices[1]];
        stickers[ciclo.indices[1]] = stickers[ciclo.indices[0]];
        stickers[ciclo.indices[0]] = guardado;
    }

    int passosPara(Sentido sentido){
        switch (sentido){
            case Sentido::HORARIO: 
                return 1;
            case Sentido::DUPLO:
                return 2;
            case Sentido::ANTI_HORARIO:
                return 3;
            default:
                return 0; // nunca deve acontecer
        }
        return 1;
    }
// Namespace anonimo: CicloPermutacao, TabelaFace, tabelaFace, aplicaCicloHorario
// e passosPara sao detalhes de implementacao de aplicarMovimento, sem uso fora
// deste arquivo. Ligacao interna evita colisao de nomes com outros .cpp e deixa
// explicito, so pela leitura, que nada aqui faz parte da API publica de Sucessora.hpp.
}

EstadoCubo aplicarMovimento(const EstadoCubo& estado, Movimento movimento) {
    EstadoCubo resultado = estado;
    const TabelaFace& tabela = tabelaFace(movimento.face);

    for(int passo = 0; passo < passosPara(movimento.sentido); ++passo){
        aplicaCicloHorario(resultado.stickers, tabela.proprios);
        aplicaCicloHorario(resultado.stickers, tabela.lateral1);
        aplicaCicloHorario(resultado.stickers, tabela.lateral2);
    }
    return resultado;
}


//==== Depende de heuristicaCantos.hpp, que ainda nao existe. ====

std::vector<NoBusca*> sucessoraCubo(NoBusca* atual,
                                     std::unordered_set<EstadoCubo, HashEstado>* visitados) {
    // TODO: gerar ate 18 filhos (aplicarMovimento para cada Face/Sentido),
    // podar o movimento inverso ao ultimo e, se 'visitados' != nullptr,
    // descartar estados repetidos.
    return {};
}

std::vector<NoBusca*> sucessoraComHeuristica(NoBusca* atual,
                                              std::unordered_set<EstadoCubo, HashEstado>* visitados) {
    // TODO: igual a sucessoraCubo, preenchendo tambem filho->h via heuristicaCantos.
    return {};
}
