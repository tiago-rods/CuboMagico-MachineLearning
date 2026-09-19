#include <doctest/doctest.h>

#include <unordered_map>

#include "cubo/Avaliadora.hpp"
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

    // TODO: 1 movimento e "caminho resolve" com embaralhar(n, seed) da Pessoa C.
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

    // TODO: 1 movimento e "caminho resolve" com embaralhar(n, seed) da Pessoa C.
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

    // TODO: 1 movimento e "caminho resolve" com embaralhar(n, seed) da Pessoa C.
}

TEST_CASE("BFS, IDDFS e A* encontram solucoes do mesmo tamanho otimo") {
    // TODO: gerar scrambles pequenos (1 a 5 movimentos) com embaralhar(n, seed)
    // e comparar resultado.caminho.size() entre os 3 algoritmos.
}

TEST_CASE("A* visita menos estados que BFS em scrambles mais profundos") {
    // TODO: <= ou < ainda em aberto (docs/decisoes-pessoa-b.md).
}
