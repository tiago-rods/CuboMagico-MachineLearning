#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "cubo/EstadoCubo.hpp"
#include "cubo/Sucessora.hpp"

namespace {
    const Face TODAS_FACES[6] = {Face::U, Face::D, Face::L, Face::R, Face::F, Face::B};
}
TEST_CASE("quatro giros iguais voltam ao estado inicial") {
    EstadoCubo inicial = estadoResolvido();

    for (Face face : TODAS_FACES){
        Movimento mov{face, Sentido::HORARIO};
        SUBCASE(mov.toString().c_str()){
            EstadoCubo estado = inicial;
            for (int i = 0; i < 4; ++i){
                estado = aplicarMovimento(estado, mov);
            }
            CHECK(estado == inicial);
        }
    }
}

    TEST_CASE("giro seguido do inverso volta ao estado inicial") {
        EstadoCubo inicial = estadoResolvido();
        const Sentido TODOS_SENTIDOS[3] = {Sentido::HORARIO, Sentido::ANTI_HORARIO, Sentido::DUPLO};

        for (Face face : TODAS_FACES){
            for(Sentido sentido : TODOS_SENTIDOS){
                Movimento mov{face, sentido};
                SUBCASE(mov.toString().c_str()){
                    EstadoCubo estado = aplicarMovimento(inicial, mov);
                    estado = aplicarMovimento(estado, movimentoInverso(mov));
                    CHECK(estado == inicial);
               }
            }
        }
    }

TEST_CASE("movimento duplo equivale a dois giros simples") {
    EstadoCubo inicial = estadoResolvido();

    for(Face face : TODAS_FACES){
        Movimento movDuplo{face, Sentido::DUPLO};
        SUBCASE(movDuplo.toString().c_str()){
            EstadoCubo duplo = aplicarMovimento(inicial, movDuplo);

            EstadoCubo doisSimples = aplicarMovimento(inicial, Movimento{face, Sentido::HORARIO});
            doisSimples = aplicarMovimento(doisSimples, Movimento{face, Sentido::HORARIO});

            CHECK(duplo == doisSimples);
        }
    }
}

