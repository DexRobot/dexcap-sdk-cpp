cmake_minimum_required(VERSION 3.10)
project(DexGloveDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)

set(INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/../include
    ${PROJECT_SOURCE_DIR}/../include/cpp)

if(UNIX)
    set(DEP_LIB_DIR ${PROJECT_SOURCE_DIR}/../libs/linux)
else()
    set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable yaml-cpp tests" FORCE)
    set(YAML_BUILD_SHARED_LIBS ON CACHE BOOL "Build yaml-cpp shared library" FORCE)
    add_subdirectory(external/yaml)
endif()

if(UNIX)
    set(TARGET_DIR ${PROJECT_SOURCE_DIR}/bin/linux)
else()
    set(TARGET_DIR ${PROJECT_SOURCE_DIR}/bin/windows)
endif()


ADD_EXECUTABLE(DexCapDemo example.cpp example_defs.h)
ADD_EXECUTABLE(DexCapDemo_C c_example.c example_defs.h)

if(UNIX)
    target_include_directories(DexCapDemo PUBLIC ${INCLUDE_DIRECTORIES})
    target_include_directories(DexCapDemo_C PUBLIC ${INCLUDE_DIRECTORIES})
    set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -g -mavx2 -mfma -O2 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g -mavx2 -mfma -O0 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g -mavx2 -mfma -O2 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    add_compile_options(-m64 -shared -fPIC -fno-strict-aliasing -Wstringop-overflow=0 -Wpessimizing-move)

    target_link_libraries(DexCapDemo PUBLIC ${DEP_LIB_DIR}/libserial.so ${DEP_LIB_DIR}/libyaml-cpp.so ${DEP_LIB_DIR}/libusb-1.0.so ${DEP_LIB_DIR}/libDexCap.so)
    target_link_libraries(DexCapDemo_C PUBLIC ${DEP_LIB_DIR}/libserial.so ${DEP_LIB_DIR}/libyaml-cpp.so ${DEP_LIB_DIR}/libusb-1.0.so ${DEP_LIB_DIR}/libDexCap.so)
else()
    target_compile_definitions(DexCapDemo PRIVATE DexCap _AMD64_)
    target_include_directories(DexCapDemo PUBLIC ${INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/external/yaml/include/)

    add_definitions(
            /GS /W3 /Gy /Ob2 /Zc:wchar_t /Zi /Gm- /Zc:inline /Zc:__cplusplus /fp:precise /D "WIN32"
            /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /Gd /FC /Fa /GF
            /D "NOMINMAX" /utf-8 /EHsc /nologo /diagnostics:classic /D _CRT_SECURE_NO_WARNINGS
    )
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DUNICODE -D_UNICODE)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Od -D_DEBUG /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 /MT")
    link_libraries(ws2_32)
    
    target_link_libraries(DexCapDemo PUBLIC DexCap)
    get_target_property(YAMLCPP_TYPE yaml-cpp TYPE)
endif()

set_target_properties(DexCapDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TARGET_DIR}")
set_target_properties(DexCapDemo_C PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TARGET_DIR}")

if(UNIX)
    add_custom_target(COPY_CONF_FILE ALL
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../conf/config.yaml ${TARGET_DIR}/config.yaml
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../conf/env_setup.sh ${TARGET_DIR}/env_setup.sh
    )
else()
    add_custom_target(COPY_TARGET_FILES_WIN ALL
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/../conf/config.yaml
            ${TARGET_DIR}/config.yaml
        )
endif()
