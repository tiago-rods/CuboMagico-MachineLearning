#include "view/InterpretadorComando.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>

#include "cubo/Movimento.hpp"

namespace {
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

    // Alguns terminais/redirecionamentos do Windows colocam um BOM UTF-8
    // (3 bytes invisiveis) no comeco da primeira linha lida.
    std::string removerBom(const std::string& texto) {
        if (texto.size() >= 3 &&
            static_cast<unsigned char>(texto[0]) == 0xEF &&
            static_cast<unsigned char>(texto[1]) == 0xBB &&
            static_cast<unsigned char>(texto[2]) == 0xBF) {
            return texto.substr(3);
        }
        return texto;
    }
}

Comando interpretarComando(const std::string& textoBruto) {
    std::string entrada = aparar(removerBom(textoBruto));
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
