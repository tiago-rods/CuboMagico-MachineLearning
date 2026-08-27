#include <doctest/doctest.h>

#include "cubo/EstadoCubo.h"
#include "search/AEstrela.h"
#include "search/BFS.h"
#include "search/IDDFS.h"

TEST_CASE("BFS, IDDFS e A* encontram solucoes do mesmo tamanho otimo") {
    // TODO: gerar scrambles pequenos (1 a 5 movimentos) com seed fixa e
    // comparar resultado.caminho.size() entre os 3 algoritmos.
}

TEST_CASE("A* visita menos estados que BFS em scrambles mais profundos") {
    // TODO
}
