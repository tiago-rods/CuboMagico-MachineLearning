#include "app/FactoryAlgoritmo.hpp"

#include "search/AEstrela.hpp"
#include "search/BFS.hpp"
#include "search/IDDFS.hpp"

std::unique_ptr<IAlgoritmoBusca> criarAlgoritmo(TipoBusca tipo) {
    // Sem 'default' de proposito: se um novo valor entrar em TipoBusca e nao
    // for tratado aqui, o compilador avisa (-Wswitch).
    switch (tipo) {
        case TipoBusca::BFS:   return std::make_unique<BFS>();
        case TipoBusca::IDDFS: return std::make_unique<IDDFS>();
        case TipoBusca::ASTAR: return std::make_unique<AEstrela>();
    }
    return nullptr; // inalcancavel para valores validos do enum
}
