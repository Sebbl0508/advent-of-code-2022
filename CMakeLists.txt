cmake_minimum_required(VERSION 3.13)

project(aoc-2022 C CXX)

# Set local include directory and source files
include_directories(include)
file(GLOB_RECURSE ${CMAKE_PROJECT_NAME}_SRC src/*.c include/*.h)

# add executable from source files
add_executable(${CMAKE_PROJECT_NAME} ${${CMAKE_PROJECT_NAME}_SRC})


# set standards & compiler options
set(CMAKE_C_STANDARD 11)
if(MSVC)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Symlink the inputs directory to the output
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/inputs
            ${CMAKE_BINARY_DIR}/inputs
)