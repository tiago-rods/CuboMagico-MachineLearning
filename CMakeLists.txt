cmake_minimum_required(VERSION 3.20)
project(CuboMagico2x2x2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(WITH_OPENGL "Compilar o VisualizadorOpenGL (freeglut)" OFF)

include_directories(include src)

add_library(cubo_core
    src/cubo/EstadoCubo.cpp
    src/cubo/Movimento.cpp
    src/cubo/Sucessora.cpp
    src/cubo/Avaliadora.cpp
    src/search/BuscaGenerica.cpp
    src/search/FrontierFila.cpp
    src/search/FrontierPilha.cpp
    src/search/FrontierPrioridade.cpp
    src/search/BFS.cpp
    src/search/IDDFS.cpp
    src/search/AEstrela.cpp
)

add_library(cubo_view_terminal
    src/view/terminal/VisualizadorTerminal.cpp
)
target_link_libraries(cubo_view_terminal PUBLIC cubo_core)

add_executable(cubo_magico
    src/main.cpp
    src/app/Controller.cpp
    src/app/FactoryAlgoritmo.cpp
    src/app/FactoryVisualizador.cpp
)
target_link_libraries(cubo_magico PRIVATE cubo_core cubo_view_terminal)

if(WITH_OPENGL)
    find_package(OpenGL REQUIRED)
    include(FetchContent)
    FetchContent_Declare(
        freeglut
        GIT_REPOSITORY https://github.com/freeglut/freeglut.git
        GIT_TAG        v3.6.0
    )
    set(FREEGLUT_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
    set(FREEGLUT_BUILD_STATIC_LIBS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(freeglut)

    add_library(cubo_view_opengl
        src/view/opengl/VisualizadorOpenGL.cpp
        src/view/opengl/Camera.cpp
        src/view/opengl/RenderCubie.cpp
    )
    target_link_libraries(cubo_view_opengl PUBLIC cubo_core freeglut OpenGL::GLU OpenGL::GL)
    target_include_directories(cubo_view_opengl PUBLIC ${freeglut_SOURCE_DIR}/include)
    target_link_libraries(cubo_magico PRIVATE cubo_view_opengl)
    target_compile_definitions(cubo_magico PRIVATE COM_OPENGL)

    add_executable(cubo_opengl_preview src/view/opengl/PreviewMain.cpp)
    target_link_libraries(cubo_opengl_preview PRIVATE cubo_view_opengl)
endif()

enable_testing()
add_subdirectory(tests)
