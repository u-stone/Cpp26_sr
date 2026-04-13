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

add_executable(03_Reflecting_Structs_Classes src/03_Reflecting_Structs_Classes/main.cpp)

add_executable(04_Attributes_and_Functions src/04_Attributes_and_Functions/main.cpp)

add_executable(05_Type_Synthesis src/05_Type_Synthesis/main.cpp)

add_executable(06_Tuple_Generation src/06_Tuple_Generation/main.cpp)

add_executable(07_JSON_Serializer src/07_JSON_Serializer/main.cpp)

add_executable(08_Command_Line_Parser src/08_Command_Line_Parser/main.cpp)
