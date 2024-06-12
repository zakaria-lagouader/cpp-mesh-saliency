cmake_minimum_required(VERSION 3.16)
project(example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# NumCpp
include(NumCpp)

# Libigl
include(libigl)

# Enable the target igl::glfw
igl_include(glfw)

# Find Boost
find_package(Boost 1.68 REQUIRED COMPONENTS date_time log log_setup)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

# Add your project files
file(GLOB SRC_FILES *.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Link libigl and Boost to your project
target_link_libraries(${PROJECT_NAME} PUBLIC 
    igl::core
    igl::glfw
    ${Boost_LIBRARIES}
)

# Include NumCpp headers
target_include_directories(${PROJECT_NAME} PUBLIC ${NumCpp_SOURCE_DIR}/include)
