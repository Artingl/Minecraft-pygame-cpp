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

find_package(Boost 1.68.0 COMPONENTS python39 REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    set(Boost_LIBRARY_DIR "${Boost_Path}/lib/x64/lib")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARY_DIR}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARIES})
endif()

find_package(PythonInterp 3.9 REQUIRED)
find_package(PythonLibs 3.9 REQUIRED)
if(PYTHONLIBS_FOUND)
    message(STATUS "PYTHON_LIBRARIES = ${PYTHON_LIBRARIES}")
    message(STATUS "PYTHON_EXECUTABLE = ${PYTHON_EXECUTABLE}")
    message(STATUS "PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS}")
    include_directories(${PYTHON_INCLUDE_DIRS})
    link_directories(${PYTHON_LIBRARIES})
endif()

add_library(${PROJECT_NAME} SHARED cpp-src/main.cpp)

if(Boost_FOUND AND PYTHONLIBS_FOUND)
    target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE  ${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
endif()