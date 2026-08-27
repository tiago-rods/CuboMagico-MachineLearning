include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
)
FetchContent_MakeAvailable(doctest)

add_executable(cubo_tests
    test_movimentos.cpp
    test_avaliadora.cpp
    test_busca.cpp
)
target_link_libraries(cubo_tests PRIVATE cubo_core doctest::doctest)

include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(cubo_tests)
