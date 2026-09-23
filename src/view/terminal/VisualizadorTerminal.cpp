#include "view/terminal/VisualizadorTerminal.hpp"

#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "cubo/Movimento.hpp"

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

    std::string paraMinusculas(const std::string& texto) {
        std::string resultado = texto;
        for (char& c : resultado) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return resultado;
    }

    std::string aparar(const std::string& texto) {
        auto inicio = texto.find_first_not_of(" \t\r\n");
        if (inicio == std::string::npos) return "";
        auto fim = texto.find_last_not_of(" \t\r\n");
        return texto.substr(inicio, fim - inicio + 1);
    }
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

    // Alguns terminais/redirecionamentos do Windows colocam um BOM UTF-8
    // (3 bytes invisiveis) no comeco da primeira linha lida.
    if (entrada.size() >= 3 &&
        static_cast<unsigned char>(entrada[0]) == 0xEF &&
        static_cast<unsigned char>(entrada[1]) == 0xBB &&
        static_cast<unsigned char>(entrada[2]) == 0xBF) {
        entrada.erase(0, 3);
    }

    entrada = aparar(entrada);
    if (entrada.empty()) {
        return Comando{TipoComando::INVALIDO, {}, entrada};
    }

    std::istringstream fluxo(entrada);
    std::string primeiraPalavra;
    fluxo >> primeiraPalavra;
    std::string resto;
    std::getline(fluxo, resto);
    resto = aparar(resto);

    const std::string palavra = paraMinusculas(primeiraPalavra);

    if (palavra == "sair" || palavra == "exit" || palavra == "quit") {
        return Comando{TipoComando::SAIR, {}, ""};
    }
    if (palavra == "estado" || palavra == "mostrar") {
        return Comando{TipoComando::MOSTRAR_ESTADO, {}, ""};
    }
    if (palavra == "bfs") {
        return Comando{TipoComando::RESOLVER_BFS, {}, ""};
    }
    if (palavra == "iddfs") {
        return Comando{TipoComando::RESOLVER_IDDFS, {}, ""};
    }
    if (palavra == "astar" || palavra == "a*") {
        return Comando{TipoComando::RESOLVER_ASTAR, {}, ""};
    }
    if (palavra == "embaralhar") {
        return Comando{TipoComando::EMBARALHAR, {}, resto};
    }
    if (palavra == "trocar") {
        return Comando{TipoComando::TROCAR_VIEW, {}, paraMinusculas(resto)};
    }

    std::string tokenMovimento = entrada;
    for (char& c : tokenMovimento) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

    try {
        Movimento movimento = parseMovimento(tokenMovimento);
        return Comando{TipoComando::MOVIMENTO, movimento, entrada};
    } catch (const std::invalid_argument&) {
        return Comando{TipoComando::INVALIDO, {}, entrada};
    }
}

void VisualizadorTerminal::mostrarMensagem(const std::string& mensagem) {
    std::cout << mensagem << "\n";
}
