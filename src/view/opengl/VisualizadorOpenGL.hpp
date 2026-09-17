#pragma once

#include "view/IVisualizador.hpp"
#include "view/opengl/Camera.hpp"
class VisualizadorOpenGL : public IVisualizador {
public:
    void renderizar(const EstadoCubo& estado) override;
    Comando lerComando() override;
    void mostrarMensagem(const std::string& mensagem) override;

private: 
    void inicializarJanela();
    void desenharCena();
    void processarBuffer();

    static void callbackDesenhar();
    static void callbackMouseClick(int botao, int estado, int x, int y);
    static void callbackMouseArrasto(int x, int y);
    static void callbackTeclado(unsigned char tecla, int x, int y);

    Camera camera_;
    EstadoCubo estadoAtual_;
    bool janelaCriada_ = false;
    bool arrastando_ = false;
    int mouseX_ = 0;
    int mouseY_ = 0;

    std::string bufferComando_;
    bool comandoPronto_ = false;
    Comando comandoLido_; 

    static VisualizadorOpenGL* instancia_; // erro nessa linah ver o motivo
};
