#include <doctest/doctest.h>

#include <unordered_map>

#include "cubo/Avaliadora.hpp"
#include "cubo/Embaralhar.hpp"
#include "cubo/EstadoCubo.hpp"
#include "cubo/Sucessora.hpp"
#include "search/AEstrela.hpp"
#include "search/BFS.hpp"
#include "search/IDDFS.hpp"

namespace {
    // A solucao so vale se aplicar o caminho no estado inicial chega a um objetivo.
    EstadoCubo aplicarCaminho(EstadoCubo estado, const std::vector<Movimento>& caminho) {
        for (const Movimento& mov : caminho) estado = aplicarMovimento(estado, mov);
        return estado;
    }
}

TEST_CASE("sucessoraComMelhorG so gera estados novos ou com g menor") {
    NoBusca raiz;
    raiz.estado = estadoResolvido();
    std::unordered_map<EstadoCubo, int, HashEstado> melhorG{{raiz.estado, 0}};

    std::vector<NoBusca*> filhos = sucessoraComMelhorG(&raiz, melhorG);
    REQUIRE(filhos.size() == 18);
    CHECK(melhorG.size() == 19);
    for (NoBusca* filho : filhos) {
        CHECK(filho->profundidade == 1);
        CHECK(filho->h == heuristicaCantos(filho->estado));
    }

    // Todos os filhos ja estao registrados com g = 1: nada novo e gerado.
    CHECK(sucessoraComMelhorG(&raiz, melhorG).empty());

    // Um estado registrado com g pior volta a ser gerado, e o g e atualizado.
    EstadoCubo aposR = aplicarMovimento(raiz.estado, Movimento{Face::R, Sentido::HORARIO});
    melhorG[aposR] = 5;
    std::vector<NoBusca*> regerados = sucessoraComMelhorG(&raiz, melhorG);
    REQUIRE(regerados.size() == 1);
    CHECK(regerados[0]->estado == aposR);
    CHECK(melhorG[aposR] == 1);

    for (NoBusca* no : filhos) delete no;
    for (NoBusca* no : regerados) delete no;
}

TEST_CASE("BFS") {
    BFS bfs;

    SUBCASE("cubo ja resolvido: caminho vazio e 1 estado visitado") {
        ResultadoBusca resultado = bfs.resolver(estadoResolvido());
        REQUIRE(resultado.encontrou);
        CHECK(resultado.caminho.empty());
        CHECK(resultado.estadosVisitados == 1);
        CHECK(ehEstadoObjetivo(aplicarCaminho(estadoResolvido(), resultado.caminho)));
    }

    SUBCASE("scramble de 1 movimento: solucao de 1 passo que resolve o cubo") {
        EstadoCubo embaralhado = embaralhar(1, 42);
        ResultadoBusca resultado = bfs.resolver(embaralhado);
        REQUIRE(resultado.encontrou);
        CHECK(resultado.caminho.size() == 1);
        CHECK(ehEstadoObjetivo(aplicarCaminho(embaralhado, resultado.caminho)));
    }
}

TEST_CASE("IDDFS") {
    IDDFS iddfs;

    SUBCASE("cubo ja resolvido: caminho vazio e 1 estado visitado") {
        ResultadoBusca resultado = iddfs.resolver(estadoResolvido());
        REQUIRE(resultado.encontrou);
        CHECK(resultado.caminho.empty());
        CHECK(resultado.estadosVisitados == 1); // achado na rodada de limite 0
        CHECK(ehEstadoObjetivo(aplicarCaminho(estadoResolvido(), resultado.caminho)));
    }

    SUBCASE("scramble de 1 movimento: solucao de 1 passo que resolve o cubo") {
        EstadoCubo embaralhado = embaralhar(1, 42);
        ResultadoBusca resultado = iddfs.resolver(embaralhado);
        REQUIRE(resultado.encontrou);
        CHECK(resultado.caminho.size() == 1);
        CHECK(ehEstadoObjetivo(aplicarCaminho(embaralhado, resultado.caminho)));
    }
}

TEST_CASE("A*") {
    AEstrela aEstrela;

    SUBCASE("cubo ja resolvido: caminho vazio e 1 estado visitado") {
        ResultadoBusca resultado = aEstrela.resolver(estadoResolvido());
        REQUIRE(resultado.encontrou);
        CHECK(resultado.caminho.empty());
        CHECK(resultado.estadosVisitados == 1);
        CHECK(ehEstadoObjetivo(aplicarCaminho(estadoResolvido(), resultado.caminho)));
    }

    SUBCASE("scramble de 1 movimento: solucao de 1 passo que resolve o cubo") {
        EstadoCubo embaralhado = embaralhar(1, 42);
        ResultadoBusca resultado = aEstrela.resolver(embaralhado);
        REQUIRE(resultado.encontrou);
        CHECK(resultado.caminho.size() == 1);
        CHECK(ehEstadoObjetivo(aplicarCaminho(embaralhado, resultado.caminho)));
    }
}

TEST_CASE("BFS, IDDFS e A* encontram solucoes do mesmo tamanho otimo") {
    // BFS e IDDFS sao garantidamente otimos (custo uniforme, busca completa) e
    // por isso tem que bater entre si sempre. heuristicaCantos ja foi
    // documentada pelo grupo como NAO admissivel em alguns casos (ver
    // docs/decisoes-pessoa-b.md > "Heuristica - em aberto"; ex. reproduzido la:
    // "L2 R R R' L' U2" da BFS/IDDFS=2 e A*=3), entao aqui so cobramos que a
    // solucao do A* e valida e nunca mais curta que o otimo - nao cobramos
    // igualdade estrita com A* ate a heuristica ser corrigida em grupo.
    for (int nMovimentos = 1; nMovimentos <= 5; ++nMovimentos) {
        CAPTURE(nMovimentos);
        unsigned seed = 1000u + static_cast<unsigned>(nMovimentos);
        EstadoCubo embaralhado = embaralhar(nMovimentos, seed);

        BFS bfs;
        IDDFS iddfs;
        AEstrela aEstrela;

        ResultadoBusca doBfs = bfs.resolver(embaralhado);
        ResultadoBusca doIddfs = iddfs.resolver(embaralhado);
        ResultadoBusca doAstar = aEstrela.resolver(embaralhado);

        REQUIRE(doBfs.encontrou);
        REQUIRE(doIddfs.encontrou);
        REQUIRE(doAstar.encontrou);

        CHECK(doBfs.caminho.size() == doIddfs.caminho.size());
        CHECK(ehEstadoObjetivo(aplicarCaminho(embaralhado, doAstar.caminho)));
        CHECK(doAstar.caminho.size() >= doBfs.caminho.size());
    }
}

TEST_CASE("A* visita menos estados que BFS em scrambles mais profundos") {
    // "<=" e nao "<": o grupo ainda nao decidiu qual das duas cobrar
    // (docs/decisoes-pessoa-b.md); no pior caso empatam, mas A* com uma
    // heuristica informativa nunca deveria visitar mais estados que o BFS.
    EstadoCubo embaralhado = embaralhar(7, 777);

    BFS bfs;
    AEstrela aEstrela;

    ResultadoBusca doBfs = bfs.resolver(embaralhado);
    ResultadoBusca doAstar = aEstrela.resolver(embaralhado);

    REQUIRE(doBfs.encontrou);
    REQUIRE(doAstar.encontrou);
    CHECK(doAstar.estadosVisitados <= doBfs.estadosVisitados);
}
