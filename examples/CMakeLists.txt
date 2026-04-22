cmake_minimum_required(VERSION 3.10)
project(DexGloveDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)

if(UNIX)
    set(INCLUDE_DIRECTORIES /usr/lib/x86_64-linux-gnu/glib-2.0/include
    /usr/include/glib-2.0
    /usr/include/gio-unix-2.0
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/linux
    ${PROJECT_SOURCE_DIR}/interfaces
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/linux)
else()
    set(INCLUDE_DIRECTORIES
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/interfaces
    ${PROJECT_SOURCE_DIR}/include/windows
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/windows)
endif()

if(UNIX)
    set(DEP_LIB_DIR ${PROJECT_SOURCE_DIR}/libs/linux)
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


file(GLOB SRC_FILES
        ./src/common.cpp
        src/DexoBody.cpp
        src/DexGlove.cpp
        ./src/network.cpp
        src/DexCap.cpp
        ./src/configuration.cpp
        ./src/wlreceiverhelper.cpp
        ./src/DexCapAdmin.cpp
        ./src/WirelessAdapter.cpp
        src/DexCapSuitAdmin.cpp
)

if(UNIX)
    list(APPEND SRC_FILES src/linux/BluetoothDriver_Linux.cpp)
else()
    list(APPEND SRC_FILES src/windows/BluetoothDriver_Win.cpp)
endif()

ADD_LIBRARY(DexCap SHARED ${SRC_FILES}
        ${BLE_DIRECTORIES}
        include/ConnectionAdapter.h
        src/ConnectionAdapter.cpp
        include/DexCapDevice.h
        src/DexCapDevice.cpp
        include/DexCapProtocols.h
        src/DexCapProtocols.cpp
        include/BluetoothDef.h
        include/DexCapDataHandler.h
        src/DexCapDataHandler.cpp
        src/utils/encryption.cpp
        interfaces/dexcap.h
        interfaces/typedef.h
        src/PyDexCap.cpp
        include/DexIMUnit.h
        src/DexIMUnit.cpp
        interfaces/cpp/Utils.hpp
        src/utils/SIMDFuncs.cpp)
ADD_EXECUTABLE(DexCapDemo examples/example.cpp
        examples/c_example.c
        examples/example_defs.h)

ADD_EXECUTABLE(UnitTests
        src/common.cpp
        tests/bluetooth_tests.cpp
        tests/bluetooth_tests.h
        tests/TestMain.cpp
        tests/DexCapTests.h
        tests/DexCapTests.cpp
        tests/DexCap_CInterfaces_Tests.h
        tests/DexCap_CInterfaces_Tests.cpp)

add_dependencies(DexCapDemo DexCap)
add_dependencies(UnitTests DexCap)

if(UNIX)
    target_include_directories(DexCap PUBLIC ${INCLUDE_DIRECTORIES})
    target_include_directories(UnitTests PUBLIC ${INCLUDE_DIRECTORIES})
    target_include_directories(DexCapDemo PUBLIC ${INCLUDE_DIRECTORIES})
    set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -g -mavx2 -mfma -O2 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g -mavx2 -mfma -O0 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -g -mavx2 -mfma -O2 -Wno-narrowing -Wno-deprecated-enum-enum-conversion")
    add_compile_options(-m64 -shared -fPIC -fno-strict-aliasing -Wstringop-overflow=0 -Wpessimizing-move)

    set_property(TARGET DexCap PROPERTY INTERFACE_LINK_LIBRARIES -Wl,--whole-archive,$<TARGET_FILE:mpfr gmp>, --no-whole-archive)
    target_link_libraries(DexCap PUBLIC gio-2.0 glib-2.0 gobject-2.0 bluetooth ${DEP_LIB_DIR}/libudev.so ${DEP_LIB_DIR}/libserial.so ${DEP_LIB_DIR}/libyaml-cpp.so ${DEP_LIB_DIR}/libusb-1.0.so)
    target_link_libraries(UnitTests PUBLIC gio-2.0 glib-2.0 gobject-2.0 bluetooth ${DEP_LIB_DIR}/libserial.so ${DEP_LIB_DIR}/libyaml-cpp.so ${DEP_LIB_DIR}/libusb-1.0.so ${TARGET_DIR}/libDexCap.so)
    target_link_libraries(DexCapDemo PUBLIC ${DEP_LIB_DIR}/libserial.so ${DEP_LIB_DIR}/libyaml-cpp.so ${DEP_LIB_DIR}/libusb-1.0.so ${TARGET_DIR}/libDexCap.so)

    set_target_properties(DexCap PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${TARGET_DIR}")
    set_target_properties(UnitTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TARGET_DIR}")
else()
    target_include_directories(DexCap PUBLIC ${INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/external/yaml/include/)
    target_compile_definitions(DexCap PRIVATE BUILD_DLL _AMD64_)
    target_compile_definitions(DexCapDemo PRIVATE DexCap _AMD64_)
    target_include_directories(DexCapDemo PUBLIC ${INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/external/yaml/include/)
    target_include_directories(UnitTests PUBLIC ${INCLUDE_DIRECTORIES} ${PROJECT_SOURCE_DIR}/external/yaml/include/)
    target_compile_definitions(UnitTests PRIVATE _AMD64_)

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
    
    target_link_libraries(DexCap PUBLIC yaml-cpp)
    target_link_libraries(DexCapDemo PUBLIC DexCap)
    target_link_libraries(UnitTests PUBLIC DexCap)
    target_link_libraries(DexCap PUBLIC windowsapp RuntimeObject setupapi)

    set_target_properties(DexCap PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set_target_properties(DexCap PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "${TARGET_DIR}" 
        ARCHIVE_OUTPUT_DIRECTORY "${TARGET_DIR}"
        PREFIX "")

    get_target_property(YAMLCPP_TYPE yaml-cpp TYPE)
    if(YAMLCPP_TYPE STREQUAL "SHARED_LIBRARY")
        add_custom_command(TARGET DexCap POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:yaml-cpp>
            "${TARGET_DIR}/$<TARGET_FILE_NAME:yaml-cpp>")
    endif()
endif()

set_target_properties(DexCapDemo PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${TARGET_DIR}")

if(UNIX)
    add_custom_target(COPY_CONF_FILE ALL
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/examples/config.yaml ${TARGET_DIR}/config.yaml
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/env_setup.sh ${TARGET_DIR}/env_setup.sh
    )
else()
    add_custom_target(COPY_TARGET_FILES_WIN ALL
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy
            ${PROJECT_SOURCE_DIR}/examples/config.yaml
            ${TARGET_DIR}/config.yaml
        )
endif()
