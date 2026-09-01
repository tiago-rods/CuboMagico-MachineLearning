#pragma once

#include "cubo/EstadoCubo.h"

// TODO (Semana 3): desenha um cubie (canto) colorido nas 3 faces visiveis
// usando glutSolidCube + glTranslatef/glRotatef (pipeline fixo).
void desenharCubie(int indiceCanto, const EstadoCubo& estado);
