cmake_minimum_required(VERSION 3.16)
project(mesh-preprocessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Libigl
include(libigl)

# xtensor, xtl, xtensor-io, and xtensor-blas
include(xtensor)

# Enable the target igl::glfw
igl_include(glfw)

# Add your project files
file(GLOB SRC_FILES *.cpp)

foreach(file ${SRC_FILES})
    get_filename_component(name ${file} NAME_WE)
    add_executable(${name} ${file})
    target_link_libraries(${name} PUBLIC 
        igl::core
        igl::glfw
        xtensor
        xtensor-io
        xtensor-blas
    )
endforeach()
