#pragma once

#include <stdlib.h>
#include "card.h"
#include "linkedList.h"
#include "commands.h"
#include "board.h"

void printBoard(Board board, Command previousCommand, char* lastInput, char* message);
char* getUserInput();