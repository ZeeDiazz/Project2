cmake_minimum_required(VERSION 3.24)
project(Project2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Project2 main.c moveValidation.c card.c linkedList.c cli.c commands.c readFile.c moveStack.c shuffleDeck.c game.c)
