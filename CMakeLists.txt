cmake_minimum_required(VERSION 3.20)

project(NoctomorphCore VERSION 0.1.0 LANGUAGES CXX)

add_library(noctomorph_core STATIC
    src/NoctomorphCore.cpp
)

target_include_directories(noctomorph_core PUBLIC include)
target_compile_features(noctomorph_core PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(noctomorph_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(noctomorph_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(noctomorph_core_tests
    tests/core_tests.cpp
)
target_link_libraries(noctomorph_core_tests PRIVATE noctomorph_core)
target_compile_features(noctomorph_core_tests PRIVATE cxx_std_17)

add_executable(noctomorph_smoke_render
    tools/render_smoke.cpp
)
target_link_libraries(noctomorph_smoke_render PRIVATE noctomorph_core)
target_compile_features(noctomorph_smoke_render PRIVATE cxx_std_17)

add_executable(noctomorph_evolution_probe
    tools/evolution_probe.cpp
)
target_link_libraries(noctomorph_evolution_probe PRIVATE noctomorph_core)
target_compile_features(noctomorph_evolution_probe PRIVATE cxx_std_17)

add_executable(noctomorph_real_source_render
    tools/render_real_sources.cpp
)
target_link_libraries(noctomorph_real_source_render PRIVATE noctomorph_core)
target_compile_features(noctomorph_real_source_render PRIVATE cxx_std_17)

enable_testing()
add_test(NAME noctomorph_core_tests COMMAND noctomorph_core_tests)
