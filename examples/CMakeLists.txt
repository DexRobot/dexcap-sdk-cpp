cmake_minimum_required(VERSION 3.10)
project(DexCapDemo)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/../include)

if(UNIX)
    set(DEP_LIB_DIR ${PROJECT_SOURCE_DIR}/../libs/linux)

else()
    set(YAML_CPP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/external/include)
    set(YAMLCPP_DEBUG_LIB_DIR ${PROJECT_SOURCE_DIR}/external/Debug)
    set(YAMLCPP_RELEASE_LIB_DIR ${PROJECT_SOURCE_DIR}/external/Release)

    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(YAML_CPP_LIB_DIR ${YAMLCPP_DEBUG_LIB_DIR})
    else()
        set(YAML_CPP_LIB_DIR ${YAMLCPP_RELEASE_LIB_DIR})
    endif()

    set(YAML_LIB $<$<CONFIG:Debug>:yaml-cppd.lib>$<$<CONFIG:Release>:yaml-cpp.lib>)

    include_directories(${YAML_CPP_INCLUDE_DIR})
    link_directories(${YAML_CPP_LIB_DIR})

endif()

if(UNIX)
    set(TARGET_DIR ${PROJECT_SOURCE_DIR}/bin/linux)
else()
    set(TARGET_DIR ${PROJECT_SOURCE_DIR}/bin/windows)
endif()


file(GLOB SRC_FILES
        ./src/common.cpp
        ./src/arm.cpp
        ./src/glove.cpp
        ./src/network.cpp
        ./src/exoskeleton.cpp
        ./src/configuration.cpp
        ./src/WirelessAdapter.cpp
)

ADD_EXECUTABLE(DexCapDemo example.cpp)

if(UNIX)
    set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -g -O2 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g -O0 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g -O2 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    add_compile_options(-m64 -shared -fPIC -fno-strict-aliasing -Wstringop-overflow=0 -Wpessimizing-move)
    target_link_libraries(DexCapDemo PUBLIC ${DEP_LIB_DIR}/libserial.so ${DEP_LIB_DIR}/libyaml-cpp.so ${DEP_LIB_DIR}/libusb-1.0.so ${DEP_LIB_DIR}/libDexCap.so)
else()
    target_compile_definitions(DexCapDemo PRIVATE YAML_CPP_DLL)

    add_definitions(
            /GS /W3 /Gy /Ob2 /Zc:wchar_t /Zi /Gm- /Zc:inline /Zc:__cplusplus /fp:precise /D "WIN32"
            /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /errorReport:prompt /WX- /Zc:forScope /Gd /FC /Fa /GF
            /D "NOMINMAX" /utf-8 /EHsc /nologo /diagnostics:classic /D _CRT_SECURE_NO_WARNINGS
    )
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Od -D_DEBUG /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O2 /MT")
    link_libraries(ws2_32)
    
    target_link_libraries(DexCapDemo PUBLIC DexCap)
endif()

set_target_properties(DexCapDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TARGET_DIR}")

add_custom_target(COPY_CONF_FILE ALL
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../conf/config.yaml ${TARGET_DIR}/config.yaml
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../conf/env_setup.sh ${TARGET_DIR}/env_setup.sh
)
