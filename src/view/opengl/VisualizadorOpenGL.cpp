#include "view/opengl/VisualizadorOpenGL.hpp"
#include <GL/freeglut.h>
#include "RenderCubie.hpp"

VisualizadorOpenGL* VisualizadorOpenGL::instancia_ = nullptr;

void VisualizadorOpenGL::inicializarJanela(){
    // depois anotar o que cada coisa faz
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
        glTranslatef(posicoesCantos[i][0] * espacamento,
                     posicoesCantos[i][1] * espacamento,
                     posicoesCantos[i][2] * espacamento);
        desenharCubie(i, estadoAtual_);
        glPopMatrix();
    }
    glutSwapBuffers();
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

void VisualizadorOpenGL::renderizar(const EstadoCubo& estado) {
    estadoAtual_ = estado;

    if(!janelaCriada_) inicializarJanela();

    glutPostRedisplay();
    glutMainLoopEvent();
}

Comando VisualizadorOpenGL::lerComando() {
    // TODO (D.5)
    return Comando{};
}

void VisualizadorOpenGL::mostrarMensagem(const std::string& mensagem) {
    // TODO (D.5/D.6)
}
