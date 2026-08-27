#include "cubo/Movimento.h"

bool Movimento::operator==(const Movimento& outro) const {
    // TODO
    return false;
}

std::string Movimento::toString() const {
    // TODO: ex. "U", "U'", "U2"
    return "";
}

Movimento parseMovimento(const std::string& texto) {
    // TODO: converter texto tipo "U", "U'", "U2" em Movimento.
    return Movimento{};
}

Movimento movimentoInverso(const Movimento& mov) {
    // TODO
    return mov;
}
