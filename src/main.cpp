#include "app/Controller.hpp"
#include "app/FactoryVisualizador.hpp"

int main() {
    Controller controller(criarVisualizador(TipoView::TERMINAL));
    controller.executar();
    return 0;
}
