cmake_minimum_required(VERSION 3.25)
project(Cpp26_Reflection_DMP VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure reflection-specific flags are passed if using the Clang P2996 fork
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-freflection)
endif()

# Global warning settings
add_compile_options(-Wall -Wextra -Werror -pedantic)

# Enable export compile commands for clang-tidy/LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Discovery for common utilities
add_subdirectory(src/common EXCLUDE_FROM_ALL)

# Lab targets will be added below by scaffold_lab.sh
# --------------------------------------------------

add_executable(01_Hello_Meta_Info src/01_Hello_Meta_Info/main.cpp)

add_executable(02_Reflecting_Enums src/02_Reflecting_Enums/main.cpp)

add_executable(01_Hello_Meta_info src/01_Hello_Meta_info/main.cpp)
