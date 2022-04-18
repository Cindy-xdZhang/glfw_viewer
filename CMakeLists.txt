cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
# Project
project(GLFW_Viewer)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CURRENT_BINARY_DIR "out")
# CMake
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
if (UNIX)
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
elseif (WIN32)
    set(CMAKE_FIND_LIBRARY_PREFIXES "" "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
endif ()



# External dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/" "${CMAKE_CURRENT_BINARY_DIR}/external/")

########################################################################################################################
# Source files, compiler, linker
########################################################################################################################

# Adding C++ source files
file(GLOB SRC_FILES
        src/*.cpp
        src/viewer/*.cpp)

# Adding compile units to executable
add_executable(${PROJECT_NAME}   ${SRC_FILES})



# C++ compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ffast-math -O2")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(_USE_MATH_DEFINES)
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")




# Include directories
#target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/" ${INCLUDE_DIRS})
#target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/imgui")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/eigen" )




# Linking libraries
target_link_libraries(${PROJECT_NAME} glad glfw )
#target_link_libraries(${PROJECT_NAME} OpenMeshCore OpenMeshTools)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_libraries(${PROJECT_NAME} gomp gfortran pthread openblas X11)
endif ()
