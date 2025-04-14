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

add_library(Arctyx SHARED
        include/arctyx/linker/formats/ELF_Architecture.h
        src/linker/formats/ELF_Sym.cpp include/arctyx/linker/formats/ELF_Sym.h
        src/linker/formats/ELF_Section.cpp include/arctyx/linker/formats/ELF_Section.h
        src/linker/formats/ELF_Program.cpp include/arctyx/linker/formats/ELF_Program.h
        src/linker/formats/ELF.cpp include/arctyx/linker/formats/ELF.h
        src/linker/Linker.cpp include/arctyx/linker/Linker.h
        src/compiler/Compiler.cpp include/arctyx/compiler/Compiler.h
        src/compiler/Architecture.cpp include/arctyx/compiler/Architecture.h
        src/compiler/Instruction.cpp include/arctyx/compiler/Instruction.h
        src/compiler/Language.cpp include/arctyx/compiler/Language.h
        src/compiler/Token.cpp include/arctyx/compiler/Token.h
        src/Arctyx.cpp include/arctyx/Arctyx.h
        src/compiler/Combination.cpp
        include/arctyx/compiler/Combination.h
        src/compiler/Keyword.cpp
        include/arctyx/compiler/Keyword.h
)

add_executable(ArctyxTools
        src/main.cpp
)

target_include_directories(Arctyx PUBLIC "${PROJECT_SOURCE_DIR}/include")

if (IS_OS_LINUX)
    add_compile_definitions(LWE_WS_XCB)
endif()

target_link_directories(Arctyx PUBLIC "${USER_HOME_DIRECTORY}/.local/lib")
target_link_directories(Arctyx PUBLIC "${USER_HOME_DIRECTORY}/.local/bin")
target_include_directories(Arctyx PUBLIC "${USER_HOME_DIRECTORY}/.local/include")

target_link_libraries(Arctyx PUBLIC xcb xcb-cursor xcb-xfixes xcb-xinput z asound EHS_Dyn)
target_link_libraries(ArctyxTools PRIVATE Arctyx)