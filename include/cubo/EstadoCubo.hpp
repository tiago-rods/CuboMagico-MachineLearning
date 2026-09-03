#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

enum class Cor : uint8_t { BRANCO, AMARELO, VERDE, AZUL, LARANJA, VERMELHO };

// 6 faces x 4 stickers cada, indice = face*4 + posicao.
// Ordem das faces no array: U, D, F, B, L, R.
struct EstadoCubo {
    std::array<Cor, 24> stickers{}; // array com 24 stickers que representam as cores das faces do cubo

    bool operator==(const EstadoCubo& outro) const;
};

struct HashEstado {
    std::size_t operator()(const EstadoCubo& estado) const;
};

// Estado com cada face composta por uma unica cor.
EstadoCubo estadoResolvido();
