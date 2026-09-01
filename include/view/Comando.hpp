#pragma once

#include <string>

#include "cubo/Movimento.h"

enum class TipoComando {
    MOVIMENTO,
    EMBARALHAR,
    RESOLVER_BFS,
    RESOLVER_IDDFS,
    RESOLVER_ASTAR,
    MOSTRAR_ESTADO,
    TROCAR_VIEW,
    SAIR,
    INVALIDO
};

struct Comando {
    TipoComando tipo = TipoComando::INVALIDO;
    Movimento movimento{};
    std::string argumento; // ex.: quantidade de movimentos para embaralhar
};
