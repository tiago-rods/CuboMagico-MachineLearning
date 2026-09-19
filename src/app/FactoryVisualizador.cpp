#include "app/FactoryVisualizador.hpp"
#include "view/terminal/VisualizadorTerminal.hpp"

#ifdef COM_OPENGL
#include "view/opengl/VisualizadorOpenGL.hpp"
#endif

std::unique_ptr<IVisualizador> criarVisualizador(TipoView tipo) {
    switch(tipo) {
        case TipoView::TERMINAL: return std::make_unique<VisualizadorTerminal>();
        case TipoView::OPENGL:
        #ifdef COM_OPENGL
            return std::make_unique<VisualizadorOpenGL>();
        #else
            return nullptr; // build sem flag do opengl
        #endif
    }
    
    return nullptr;
}
