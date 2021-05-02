cmake_minimum_required(VERSION 3.19)
project(Minecraft-pygame-cpp)

set(CMAKE_CXX_STANDARD 14)

set(GCC_COVERAGE_COMPILE_FLAGS "-march=native -O2 -m64 -D_hypot=hypot -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

message(STATUS "start running cmake...")

set(Boost_DEBUG                 ON)
set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     ON)
set(Boost_USE_STATIC_RUNTIME    ON)

find_package(SFML 2.5 COMPONENTS audio graphics window system REQUIRED)
find_package(Boost 1.68.0 COMPONENTS python37 REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    set(Boost_LIBRARY_DIR "${Boost_Path}/lib/x64/lib")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARY_DIR}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARIES})
endif()

find_package(PythonInterp 3.7 REQUIRED)
find_package(PythonLibs 3.7 REQUIRED)
if(PYTHONLIBS_FOUND)
    message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
    message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
    message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
    include_directories(${PYTHON_INCLUDE_DIRS})
    link_directories(${PYTHON_LIBRARIES})
endif()

enable_testing()

message(STATUS "Copying *.py files...")
file(COPY ${PYTHON_FILES} DESTINATION .)

PYTHON_ADD_MODULE(opengl_main_cpp
        cpp-src/main.cpp
        cpp-src/debug.cpp
        cpp-src/debug.h

        cpp-src/GL/Chunk.h
        cpp-src/GL/GL.h
        cpp-src/GL/Texture.h
        cpp-src/GL/vertex.h
        cpp-src/GL/Shader.h
    )
PYTHON_ADD_MODULE(debug_module cpp-src/debug_module.cpp cpp-src/debug.cpp cpp-src/debug.h)

ADD_TEST(NAME ${PROJECT_NAME} COMMAND ../compile.exe)

target_include_directories(opengl_main_cpp SYSTEM PRIVATE  ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} D:/mingw/mingw64/include)
target_link_libraries(opengl_main_cpp ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
target_link_libraries(opengl_main_cpp sfml-window sfml-network opengl32 sfml-system glew32.dll)


target_include_directories(debug_module SYSTEM PRIVATE  ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} D:/mingw/mingw64/include)
target_link_libraries(debug_module ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
