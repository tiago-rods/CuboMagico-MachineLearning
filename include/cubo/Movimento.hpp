#pragma once

#include <string>

enum class Face { U, D, L, R, F, B };
enum class Sentido { HORARIO, ANTI_HORARIO, DUPLO };

struct Movimento {
    Face face{};
    Sentido sentido{};

    bool operator==(const Movimento& outro) const;
    std::string toString() const;
};

// Converte texto tipo "U", "U'", "U2" em Movimento. Lanca std::invalid_argument
// se o texto nao for reconhecido.
Movimento parseMovimento(const std::string& texto);

// Usado pela sucessora para podar o movimento que desfaz o ultimo aplicado.
Movimento movimentoInverso(const Movimento& mov);
