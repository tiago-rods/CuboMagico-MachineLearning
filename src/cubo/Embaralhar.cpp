#include "cubo/Embaralhar.hpp"

#include <random>

#include "cubo/Movimento.hpp"
#include "cubo/Sucessora.hpp"

namespace {
    constexpr Face kFaces[6] = {Face::U, Face::D, Face::L, Face::R, Face::F, Face::B};
    constexpr Sentido kSentidos[3] = {Sentido::HORARIO, Sentido::ANTI_HORARIO, Sentido::DUPLO};
}

EstadoCubo embaralhar(int nMovimentos, unsigned seed) {
    std::mt19937 gerador(seed);
    EstadoCubo estado = estadoResolvido();

    for (int i = 0; i < nMovimentos; ++i) {
        unsigned indice = gerador() % 18;
        Movimento movimento{kFaces[indice / 3], kSentidos[indice % 3]};
        estado = aplicarMovimento(estado, movimento);
    }

    return estado;
}
