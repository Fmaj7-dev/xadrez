# minimum version
cmake_minimum_required(VERSION 2.8)

# project name
project( xadrez )

# tell cmake to include the current dir
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# output dirs
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/bin)

# include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src )

# tell cmake to search for more CMakeLists.txt files in these folders
add_subdirectory(test)

# main executable
set (SOURCES
  src/main.cpp
  src/uci.cpp
  src/engine.cpp
  src/chessboard.cpp
  src/movement.cpp
  src/position.cpp
  src/variation.cpp
)

find_package(Boost 1.75.0)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(xadrez ${SOURCES})
endif()



#link_directories(/usr/local/mysql/lib)

if(MSVC)
  target_compile_options(xadrez PRIVATE /W4 /WX)
else()
  target_compile_options(xadrez PRIVATE -std=c++17 -Wc++11-extensions -g3 -O0)
endif()

install(TARGETS xadrez DESTINATION bin)
