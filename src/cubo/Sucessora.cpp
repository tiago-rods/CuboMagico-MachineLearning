#include "cubo/Sucessora.h"

EstadoCubo aplicarMovimento(const EstadoCubo& estado, Movimento movimento) {
    // TODO: tabelas de permutacao de stickers por Face/Sentido.
    return estado;
}

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
