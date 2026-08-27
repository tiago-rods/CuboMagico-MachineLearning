#include "cubo/EstadoCubo.h"

bool EstadoCubo::operator==(const EstadoCubo& outro) const {
    // TODO: comparar os 24 stickers.
    return false;
}

std::size_t HashEstado::operator()(const EstadoCubo& estado) const {
    // TODO: gerar um hash a partir dos 24 stickers.
    return 0;
}

EstadoCubo estadoResolvido() {
    // TODO: montar o estado com cada face composta por uma unica cor.
    return EstadoCubo{};
}
