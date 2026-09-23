#include "view/terminal/VisualizadorTerminal.hpp"

#include <iostream>

#include "view/InterpretadorComando.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
    // Sem isso, versoes antigas do console do Windows imprimem os codigos ANSI
    // literalmente em vez de interpretar como cor.
    void garantirAnsiHabilitado() {
        static bool feito = false;
        if (feito) return;
        feito = true;
#ifdef _WIN32
        HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD modo = 0;
        if (saida != INVALID_HANDLE_VALUE && GetConsoleMode(saida, &modo)) {
            SetConsoleMode(saida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
#endif
    }

    const char* corAnsi(Cor cor) {
        switch (cor) {
            case Cor::BRANCO:   return "\033[107m";
            case Cor::AMARELO:  return "\033[103m";
            case Cor::VERDE:    return "\033[102m";
            case Cor::AZUL:     return "\033[104m";
            case Cor::LARANJA:  return "\033[48;5;208m";
            case Cor::VERMELHO: return "\033[101m";
        }
        return "\033[0m";
    }

    constexpr const char* kAnsiReset = "\033[0m";
}

void VisualizadorTerminal::renderizar(const EstadoCubo& estado) {
    garantirAnsiHabilitado();

    auto linha = [&](std::initializer_list<int> indices) {
        for (int indice : indices) {
            std::cout << corAnsi(estado.stickers[indice]) << "  " << kAnsiReset << " ";
        }
        std::cout << "\n";
    };

    const std::string indentacao(6, ' ');

    std::cout << "\n";
    std::cout << indentacao; linha({0, 1});
    std::cout << indentacao; linha({2, 3});
    linha({16, 17, 8, 9, 20, 21, 12, 13});
    linha({18, 19, 10, 11, 22, 23, 14, 15});
    std::cout << indentacao; linha({4, 5});
    std::cout << indentacao; linha({6, 7});
    std::cout << "\n";
}

Comando VisualizadorTerminal::lerComando() {
    std::cout << "> ";
    std::string entrada;
    if (!std::getline(std::cin, entrada)) {
        return Comando{TipoComando::SAIR, {}, ""};
    }

    return interpretarComando(entrada);
}

void VisualizadorTerminal::mostrarMensagem(const std::string& mensagem) {
    std::cout << mensagem << "\n";
}
