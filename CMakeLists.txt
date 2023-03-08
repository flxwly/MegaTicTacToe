cmake_minimum_required(VERSION 3.23)
project(MegaTicTacToe)

set(CMAKE_CXX_STANDARD 23)
set(SFML_DIR "C:\\GCC_Compiler\\SFML-2.5.1x32\\lib\\cmake\\SFML")
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)


configure_file(${CMAKE_SOURCE_DIR}/fonts/arial.ttf ${CMAKE_BINARY_DIR}/fonts/arial.ttf COPYONLY)
add_executable(MegaTicTacToe main.cpp GameWindow.cpp GameWindow.h GameLogic.cpp GameLogic.h AI.cpp AI.h Timer.hpp)

cmake_minimum_required(VERSION 3.20)

target_link_libraries(MegaTicTacToe sfml-graphics)
