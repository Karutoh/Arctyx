cmake_minimum_required(VERSION 3.18.4)
project(Arctyx C CXX)

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    set(IS_OS_WINDOWS TRUE)
    set(USER_HOME_DIRECTORY $ENV{USERPROFILE})
    message("Building for the Windows operating system.")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(IS_OS_LINUX TRUE)
    set(USER_HOME_DIRECTORY $ENV{HOME})
    add_compile_options(-Wno-stringop-overflow)
    message("Building for the Linux operating system.")
elseif (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    set(IS_OS_MAC TRUE)
    set(USER_HOME_DIRECTORY $ENV{HOME})
    message("Building for the Mac operating system.")
endif ()

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64" OR ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "x86_64")
    message("Building for AMD64 architecture.")
    set(IS_ARCH_AMD64 TRUE)
elseif ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM64" OR "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")
    message("Building for ARM64 architecture.")
    set(IS_ARCH_ARM64 TRUE)
endif ()

set(CMAKE_CXX_STANDARD 20)

add_executable(Arctyx
        include/arctyx/ELF_Architecture.h
        src/ELF_Sym.cpp include/arctyx/ELF_Sym.h
        src/ELF_Section.cpp include/arctyx/ELF_Section.h
        src/ELF_Program.cpp include/arctyx/ELF_Program.h
        src/ELF.cpp include/arctyx/ELF.h
        src/Linker.cpp include/arctyx/Linker.h
        src/Compiler.cpp include/arctyx/Compiler.h
        src/main.cpp
)

target_include_directories(Arctyx PRIVATE "${PROJECT_SOURCE_DIR}/include")

if (IS_OS_LINUX)
    add_compile_definitions(LWE_WS_XCB)
endif()

target_link_directories(Arctyx PRIVATE "${USER_HOME_DIRECTORY}/.local/lib")
target_include_directories(Arctyx PRIVATE "${USER_HOME_DIRECTORY}/.local/include")

target_link_libraries(Arctyx PRIVATE xcb xcb-cursor xcb-xfixes xcb-xinput z asound EHS)