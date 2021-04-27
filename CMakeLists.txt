cmake_minimum_required(VERSION 3.19.2)
project(3D_Engine)

set(CMAKE_CXX_STANDARD 14)

# get boost
set(Boost_INCLUDE_DIR D:/libs/cpp/boost_1_76_0/)
set(Boost_LIBRARY_DIR D:/libs/cpp/boost_1_76_0/stage/lib/)

find_package(Boost 1.76.0)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
#

# get python
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
link_directories(${PYTHON_LIBRARIES})
#

add_library(${PROJECT_NAME} SHARED
        hello.cpp
        )
target_link_libraries(${PROJECT_NAME}
        ${Boost_LIBRARIES}
        ${PYTHON_LIBRARIES}
        )
# add_executable(${PROJECT_NAME} cpp-src/main.cpp)
