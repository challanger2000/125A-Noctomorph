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

add_executable(noctomorph_performance_probe
    tools/performance_probe.cpp
)
target_link_libraries(noctomorph_performance_probe PRIVATE noctomorph_core)
target_compile_features(noctomorph_performance_probe PRIVATE cxx_std_17)

add_executable(noctomorph_real_source_render
    tools/render_real_sources.cpp
)
target_link_libraries(noctomorph_real_source_render PRIVATE noctomorph_core)
target_compile_features(noctomorph_real_source_render PRIVATE cxx_std_17)

enable_testing()
add_test(NAME noctomorph_core_tests COMMAND noctomorph_core_tests)


option(NOCTOMORPH_BUILD_VST3 "Build the Noctomorph VST3 prototype wrapper" OFF)
set(NOCTOMORPH_GENERATED_ASSET_DIR "" CACHE PATH
    "Optional generated Windows embedded-asset directory")

if(NOCTOMORPH_BUILD_VST3)
    include(FetchContent)

    set(SMTG_ENABLE_VSTGUI_SUPPORT ON CACHE BOOL "" FORCE)
    set(SMTG_ENABLE_VST3_PLUGIN_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SMTG_ENABLE_VST3_HOSTING_EXAMPLES OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        vst3sdk
        GIT_REPOSITORY https://github.com/steinbergmedia/vst3sdk.git
        GIT_TAG 3cdf9ca5d1f5b1b21e0a86832aa4abe55607bd96
        GIT_SHALLOW FALSE
    )
    FetchContent_MakeAvailable(vst3sdk)
    smtg_enable_vst3_sdk()

    smtg_add_vst3plugin(Noctomorph
        source/version.h
        source/ids.h
        source/parameters.h
        source/state_format.h
        source/processor.h
        source/processor.cpp
        source/PrototypeAssetBank.h
        source/PrototypeAssetBank.cpp
        source/controller.h
        source/controller.cpp
        source/editor.cpp
        source/DemoGui.h
        source/DemoGui.cpp
        source/entry.cpp
        include/NoctomorphCore.h
        src/NoctomorphCore.cpp
    )

    target_include_directories(Noctomorph PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/source"
    )

    if(WIN32 AND NOCTOMORPH_GENERATED_ASSET_DIR)
        if(NOT EXISTS
           "${NOCTOMORPH_GENERATED_ASSET_DIR}/noctomorph_assets_generated.h")
            message(FATAL_ERROR "Generated Noctomorph asset header missing")
        endif()
        if(NOT EXISTS
           "${NOCTOMORPH_GENERATED_ASSET_DIR}/noctomorph_assets_generated.rc")
            message(FATAL_ERROR "Generated Noctomorph asset RC missing")
        endif()

        target_include_directories(Noctomorph PRIVATE
            "${NOCTOMORPH_GENERATED_ASSET_DIR}"
        )
        target_sources(Noctomorph PRIVATE
            "${NOCTOMORPH_GENERATED_ASSET_DIR}/noctomorph_assets_generated.rc"
        )
        target_compile_definitions(Noctomorph PRIVATE
            NOCTOMORPH_HAS_EMBEDDED_ASSETS=1
        )
    endif()

    target_link_libraries(Noctomorph PRIVATE sdk vstgui_support)

    smtg_target_add_plugin_resources(Noctomorph RESOURCES
        "resource/noctomorph-demo.uidesc"
        "resource/Noctomorph_Intensity_Knob_185x185_128.png"
    )
    target_compile_features(Noctomorph PRIVATE cxx_std_17)
    smtg_target_configure_version_file(Noctomorph)

    add_executable(noctomorph_state_recall_probe
        tests/vst3_state_recall_probe.cpp
        "${vst3sdk_SOURCE_DIR}/public.sdk/source/common/memorystream.cpp"
        "${vst3sdk_SOURCE_DIR}/public.sdk/source/vst/hosting/module_win32.cpp"
    )
    target_link_libraries(noctomorph_state_recall_probe PRIVATE
        sdk_hosting sdk_common base pluginterfaces
    )
    target_compile_definitions(noctomorph_state_recall_probe PRIVATE
        _CRT_SECURE_NO_WARNINGS NOMINMAX
    )
    target_compile_features(noctomorph_state_recall_probe PRIVATE cxx_std_17)

    add_executable(noctomorph_process_contract_probe
        tests/vst3_process_contract_probe.cpp
        "${vst3sdk_SOURCE_DIR}/public.sdk/source/vst/hosting/eventlist.cpp"
        "${vst3sdk_SOURCE_DIR}/public.sdk/source/vst/hosting/parameterchanges.cpp"
        "${vst3sdk_SOURCE_DIR}/public.sdk/source/vst/hosting/module_win32.cpp"
    )
    target_link_libraries(noctomorph_process_contract_probe PRIVATE
        sdk_hosting sdk_common base pluginterfaces
    )
    target_compile_definitions(noctomorph_process_contract_probe PRIVATE
        _CRT_SECURE_NO_WARNINGS NOMINMAX
    )
    target_compile_features(noctomorph_process_contract_probe PRIVATE cxx_std_17)
endif()
