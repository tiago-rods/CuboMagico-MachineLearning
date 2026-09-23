#pragma once

#include <string>

#include "view/Comando.hpp"

// Interpreta uma linha de texto digitada pelo usuario (terminal ou OpenGL,
// ambos leem texto bruto do mesmo jeito) e devolve o Comando correspondente.
// Compartilhado pelas duas views pra elas nunca ficarem entendendo um
// conjunto diferente de palavras-chave uma da outra.
Comando interpretarComando(const std::string& textoBruto);
