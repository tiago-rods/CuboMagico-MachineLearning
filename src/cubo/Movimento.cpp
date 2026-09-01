#include "cubo/Movimento.hpp"

#include <stdexcept>

// Dois movimentos sao iguais se face e sentido coincidem.
bool Movimento::operator==(const Movimento& outro) const {
    return face == outro.face && sentido == outro.sentido;
}

// Monta a notacao padrao: letra da face + sufixo do sentido ("", "'" ou "2").
std::string Movimento::toString() const {
    std::string texto;
    switch (face) {
        case Face::U: texto = "U"; break;
        case Face::D: texto = "D"; break;
        case Face::L: texto = "L"; break;
        case Face::R: texto = "R"; break;
        case Face::F: texto = "F"; break;
        case Face::B: texto = "B"; break;
    }

    switch (sentido) {
        case Sentido::HORARIO: break;
        case Sentido::ANTI_HORARIO: texto += "'"; break;
        case Sentido::DUPLO: texto += "2"; break;
    }

    return texto;
}

// Inverso de toString(): le a letra da face e o sufixo opcional ("'" ou "2").
Movimento parseMovimento(const std::string& texto) {
    if (texto.empty() || texto.size() > 2) {
        throw std::invalid_argument("Movimento invalido: " + texto);
    }

    Face face{};
    switch (texto[0]) {
        case 'U': face = Face::U; break;
        case 'D': face = Face::D; break;
        case 'L': face = Face::L; break;
        case 'R': face = Face::R; break;
        case 'F': face = Face::F; break;
        case 'B': face = Face::B; break;
        default: throw std::invalid_argument("Movimento invalido: " + texto);
    }

    Sentido sentido = Sentido::HORARIO;
    if (texto.size() == 2) {
        switch (texto[1]) {
            case '\'': sentido = Sentido::ANTI_HORARIO; break;
            case '2': sentido = Sentido::DUPLO; break;
            default: throw std::invalid_argument("Movimento invalido: " + texto);
        }
    }

    return Movimento{face, sentido};
}

// Mesma face; horario e anti-horario se trocam, duplo (180) e o proprio inverso.
Movimento movimentoInverso(const Movimento& mov) {
    Sentido sentidoInverso{};
    switch (mov.sentido) {
        case Sentido::HORARIO: sentidoInverso = Sentido::ANTI_HORARIO; break;
        case Sentido::ANTI_HORARIO: sentidoInverso = Sentido::HORARIO; break;
        case Sentido::DUPLO: sentidoInverso = Sentido::DUPLO; break;
    }

    return Movimento{mov.face, sentidoInverso};
}
