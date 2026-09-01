#pragma once

#include <memory>

#include "search/IAlgoritmoBusca.hpp"

enum class TipoBusca { BFS, IDDFS, ASTAR };

// Factory Method: isola a escolha do algoritmo (pelo menu) num unico lugar.
std::unique_ptr<IAlgoritmoBusca> criarAlgoritmo(TipoBusca tipo);
