#include "cubo/Avaliadora.hpp"

bool ehEstadoObjetivo(const EstadoCubo& estado) {
    for(int face = 0; face < 6; ++face){
        Cor corDaFace = estado.stickers[face * 4];
        for (int pos = 1; pos < 4; ++pos){
            if(estado.stickers[face * 4 + pos] != corDaFace){
                return false;
            }
        }
    }
    return true;
}

namespace {
    // Os 8 cantos do cubo: cada um é uma trinca de indices de adesivos
    // que sempre giram juntos. (mesma tabela documentada no README.md)

    struct Canto { std::array<int, 3> indices; };
    
    const std::array<Canto, 8> CANTOS = {{
        {{2, 8, 17}},   // ULF
        {{3, 9, 20}},   // URF
        {{4, 10, 19}},  // DLF
        {{5, 11, 22}},  // DRF
        {{0, 13, 16}},  // ULB
        {{1, 12, 21}},  // URB
        {{6, 15, 18}},  // DLB
        {{7, 14, 23}},  // DRB
    }};
}

int heuristicaCantos(const EstadoCubo& estado) {
    static const EstadoCubo resolvido = estadoResolvido();
    int cantosIncorretos = 0;
    for (const Canto& canto : CANTOS){
        bool correto = true;
        for (int i : canto.indices){
            if (estado.stickers[i] != resolvido.stickers[i]){
                correto = false;
                break;
            }
        }
        if (!correto){
            ++cantosIncorretos;
        }
    }
    return (cantosIncorretos + 3) / 4; // ceil(cantosIncorretos / 4)
}
