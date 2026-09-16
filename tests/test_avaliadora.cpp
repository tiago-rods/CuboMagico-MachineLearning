#include <doctest/doctest.h>

#include "cubo/Avaliadora.hpp"
#include "cubo/EstadoCubo.hpp"
#include "cubo/Sucessora.hpp"

TEST_CASE("estado resolvido e objetivo com heuristica zero") {
    EstadoCubo estado = estadoResolvido();
    CHECK(ehEstadoObjetivo(estado));
    CHECK(heuristicaCantos(estado) == 0);
}

TEST_CASE("um movimento deixa de ser objetivo") {
    EstadoCubo estado = aplicarMovimento(estadoResolvido(), Movimento{Face::U, Sentido::HORARIO});
    CHECK_FALSE(ehEstadoObjetivo(estado));
}

TEST_CASE("heuristicaCantos == 1 apos 1 movimento"){
    EstadoCubo estado = aplicarMovimento(estadoResolvido(), Movimento{Face::U, Sentido::HORARIO});
    CHECK(heuristicaCantos(estado) == 1);
}

// TODO: heuristicaCantos == 1 apos 1 movimento (ceil(4/4)=1)
