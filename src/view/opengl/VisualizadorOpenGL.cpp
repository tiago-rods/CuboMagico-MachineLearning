#include "view/opengl/VisualizadorOpenGL.hpp"
#include <GL/freeglut.h>
#include <sstream>
#include <thread>
#include <chrono>
#include "cubo/Movimento.hpp"
#include "view/InterpretadorComando.hpp"
#include "RenderCubie.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
    // Mesma ordem de cantos de sempre: ULF=0, URF=1, DLF=2, DRF=3, ULB=4,
    // URB=5, DLB=6, DRB=7. Cada face toca 4 dos 8 cantos.
    constexpr int cantosPorFace[6][4] = {
    {0, 1, 4, 5}, // U
    {2, 3, 6, 7}, // D
    {0, 2, 4, 6}, // L
    {1, 3, 5, 7}, // R
    {0, 1, 2, 3}, // F
    {4, 5, 6, 7}, // B
    };

    // Eixo de rotacao de cada face (mesma convencao de sinais de D.3).
    constexpr float eixoPorFace[6][3] = {
    { 0.0f, +1.0f,  0.0f}, // U
    { 0.0f, -1.0f,  0.0f}, // D
    {-1.0f,  0.0f,  0.0f}, // L
    {+1.0f,  0.0f,  0.0f}, // R
    { 0.0f,  0.0f, +1.0f}, // F
    { 0.0f,  0.0f, -1.0f}, // B
    };

    float anguloPorSentido(Sentido sentido) {
        switch(sentido){
            case Sentido::HORARIO:      return -90.0f;
            case Sentido::ANTI_HORARIO: return +90.0f;
            case Sentido::DUPLO:        return +180.0f;
        }
        return 0.0f;
    }
}


VisualizadorOpenGL* VisualizadorOpenGL::instancia_ = nullptr;

bool VisualizadorOpenGL::cantoNaFace(int indiceCanto, Face face) const {
    const int* cantos = cantosPorFace[static_cast<int>(face)];
    for(int i = 0; i < 4; ++i){
        if (cantos[i] == indiceCanto) return true;
    }
    return false;
}


void VisualizadorOpenGL::animarMovimento(const Movimento& mov) {
    const int totalFrames = 24;
    const float anguloFinal = anguloPorSentido(mov.sentido);

    animando_ = true;
    faceAnimando_ = mov.face;

    for(int frame = 1; frame <= totalFrames; ++frame){
        anguloAnimacao_ = anguloFinal * frame / static_cast<float>(totalFrames);
        glutPostRedisplay();
        glutMainLoopEvent();
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); //(+- 60 fps)
    }
    animando_ = false;
    anguloAnimacao_ = 0.0f;
}

void VisualizadorOpenGL::inicializarJanela(){
    // depois anotar o que cada coisa faz
#ifdef _WIN32
    SetProcessDPIAware(); // evita o Windows esticar/borrar o framebuffer em telas com escala de DPI
#endif
    instancia_ = this;

    int argc = 1;
    char nomePrograma[] = "CuboMagico2x2";
    char* argv[] = { nomePrograma };
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cubo Magico 2x2x2");

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    glutDisplayFunc(callbackDesenhar);
    glutMouseFunc(callbackMouseClick);
    glutMotionFunc(callbackMouseArrasto);
    glutMouseWheelFunc(callbackMouseRoda);
    glutKeyboardFunc(callbackTeclado);

    janelaCriada_ = true;
}

void VisualizadorOpenGL::desenharCena(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera_.aplicar();

    //Convençao de eixos: U=y, D=-y, F=z, B=-z, R=x, L=-x
    //Ordem dos igual a numeração dos stickers
    // ULF, URF, DLF, DRF, ULB, URB, DLB, DRB

    static const float posicoesCantos[8][3] = {
        {-1.0f, +1.0f, +1.0f}, // ULF
        {+1.0f, +1.0f, +1.0f}, // URF
        {-1.0f, -1.0f, +1.0f}, // DLF
        {+1.0f, -1.0f, +1.0f}, // DRF
        {-1.0f, +1.0f, -1.0f}, // ULB
        {+1.0f, +1.0f, -1.0f}, // URB
        {-1.0f, -1.0f, -1.0f}, // DLB
        {+1.0f, -1.0f, -1.0f}, // DRB
    };
    const float espacamento = 0.52f;
    const float tamanhoCubie = 0.98f;

    for(int i = 0; i < 8; ++i){
        glPushMatrix();
        if(animando_ && cantoNaFace(i, faceAnimando_)){
            const float* eixo = eixoPorFace[static_cast<int>(faceAnimando_)];
            glRotatef(anguloAnimacao_, eixo[0], eixo[1], eixo[2]);
        }
        glTranslatef(posicoesCantos[i][0] * espacamento,
                     posicoesCantos[i][1] * espacamento,
                     posicoesCantos[i][2] * espacamento);
        desenharCubie(i, estadoAtual_);
        glPopMatrix();
    }

    desenharAjuda();
    glutSwapBuffers();
}

void VisualizadorOpenGL::desenharTexto(float x, float y, const std::string& texto) {
    glRasterPos2f(x, y);
    for (char c : texto) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
}

void VisualizadorOpenGL::desenharAjuda() {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    int largura = viewport[2];
    int altura = viewport[3];

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, largura, 0, altura);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glColor3f(1.0f, 1.0f, 1.0f);

    desenharTexto(10, altura - 20, "Movimentos: U D L R F B (sentido horario)");
    desenharTexto(10, altura - 40, "Adicione ' para anti-horario, ex: U'");
    desenharTexto(10, altura - 60, "Adicione 2 para giro duplo, ex: U2");
    desenharTexto(10, altura - 80, "Digite e aperte Enter para aplicar");
    desenharTexto(10, altura - 100, "embaralhar N [seed] | bfs | iddfs | astar | trocar terminal | sair");

    float y = altura - 130;
    std::istringstream linhasMensagem(ultimaMensagem_);
    std::string linhaMensagem;
    while (std::getline(linhasMensagem, linhaMensagem)) {
        desenharTexto(10, y, linhaMensagem);
        y -= 20;
    }

    desenharTexto(10, 20, "Comando: " + bufferComando_);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

void VisualizadorOpenGL::callbackDesenhar() {
    if (instancia_) instancia_->desenharCena();
}

void VisualizadorOpenGL::callbackMouseClick(int botao, int estado, int x, int y) {
    if (!instancia_ || botao != GLUT_LEFT_BUTTON) return;

    instancia_->arrastando_ = (estado == GLUT_DOWN);
    instancia_->mouseX_ = x;
    instancia_->mouseY_ = y;
}

void VisualizadorOpenGL::callbackMouseArrasto(int x, int y) {
    if (!instancia_ || !instancia_->arrastando_) return;

    int deltaX = x - instancia_->mouseX_;
    int deltaY = y - instancia_->mouseY_;
    instancia_->camera_.arrastar(deltaX, deltaY);
    instancia_->mouseX_ = x;
    instancia_->mouseY_ = y;

    glutPostRedisplay();
}

void VisualizadorOpenGL::callbackMouseRoda(int roda, int direcao, int x, int y) {
    if (!instancia_) return;

    const float fatorZoom = 0.5f;
    instancia_->camera_.zoom(-direcao * fatorZoom);
    glutPostRedisplay();
}

void VisualizadorOpenGL::callbackTeclado(unsigned char tecla, int x, int y){
    if(!instancia_) return;

    if(tecla == '\r' || tecla == '\n') instancia_->processarBuffer();
    else instancia_->bufferComando_ += static_cast<char>(tecla);

    glutPostRedisplay();
}

void VisualizadorOpenGL::processarBuffer(){
    comandoLido_ = interpretarComando(bufferComando_);

    if (comandoLido_.tipo == TipoComando::MOVIMENTO) {
        animarMovimento(comandoLido_.movimento);
    }

    bufferComando_.clear();
    comandoPronto_ = true;
}

Comando VisualizadorOpenGL::lerComando(){
    comandoPronto_ = false;

    while(!comandoPronto_){
        glutMainLoopEvent();
    }

    return comandoLido_;
}

void VisualizadorOpenGL::renderizar(const EstadoCubo& estado) {
    estadoAtual_ = estado;

    if(!janelaCriada_) inicializarJanela();

    glutPostRedisplay();
    glutMainLoopEvent();
}

void VisualizadorOpenGL::mostrarMensagem(const std::string& mensagem) {
    ultimaMensagem_ = mensagem;

    if(!janelaCriada_) inicializarJanela();

    glutPostRedisplay();
    glutMainLoopEvent();
}
