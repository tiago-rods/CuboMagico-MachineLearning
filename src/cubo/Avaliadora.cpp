#include "cubo/Avaliadora.h"

bool ehEstadoObjetivo(const EstadoCubo& estado) {
    // TODO: cada face deve ter os 4 stickers da mesma cor.
    return false;
}

int heuristicaCantos(const EstadoCubo& estado) {
    // TODO: ceil(cantos incorretos / 4.0) - ver justificativa no CLAUDE.md.
    return 0;
}
