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
    void desenharAjuda();
    void desenharTexto(float x, float y, const std::string& texto);
    void processarBuffer();

    void animarMovimento(const Movimento& mov);
    bool cantoNaFace(int indiceCanto, Face face) const;

    static void callbackDesenhar();
    static void callbackMouseClick(int botao, int estado, int x, int y);
    static void callbackMouseArrasto(int x, int y);
    static void callbackMouseRoda(int roda, int direcao, int x, int y);
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
    std::string ultimaMensagem_;

    bool animando_ = false;
    Face faceAnimando_ = Face::U;
    float anguloAnimacao_ = 0.0f;

    static VisualizadorOpenGL* instancia_; // erro nessa linah ver o motivo
};
