cmake_minimum_required(VERSION 3.15)
project(xadrez)

set (SOURCES
  ../src/uci.cpp
  ../src/chessboard.cpp
  ../src/position.cpp
  ../src/movement.cpp
  ../src/engine.cpp
  ../src/variation.cpp
)

find_package(Boost 1.75.0)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable( check 
                ${SOURCES}
                testChessboard.cpp
                testPosition.cpp
                testMovement.cpp
                #testPerformance.cpp
                testEngine.cpp
                #testettune.cpp
                )
endif()

#target_link_libraries(check render graph ui math "-framework GLUT -framework OpenGl")
install(TARGETS check DESTINATION build)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src )

#target_compile_options(check PRIVATE -Wall -Wextra -pedantic -Werror -std=c++17 -Wc++11-extensions)
target_compile_options(check PRIVATE -std=c++17 -Wc++11-extensions -O0 -g3)
