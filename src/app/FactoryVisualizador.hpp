#pragma once

#include <memory>

#include "view/IVisualizador.hpp"

enum class TipoView { TERMINAL, OPENGL };

// Factory Method: isola a escolha da interface (terminal ou 3D) num unico lugar.
std::unique_ptr<IVisualizador> criarVisualizador(TipoView tipo);
