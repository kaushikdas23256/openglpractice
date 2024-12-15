# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set project name
project(MyProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set the path to the include directories for GLFW, GLAD, and GLM
include_directories(${CMAKE_SOURCE_DIR}/include)

# Set the path to the GLFW static library
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Add the source files to the project, including glad.c
add_executable(MyApp src/application.cpp src/glad.c)

# Link GLFW static library and OpenGL
target_link_libraries(MyApp glfw3 opengl32)
