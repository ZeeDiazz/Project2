#pragma once

#include <stdlib.h>
#include "card.h"
#include "linkedList.h"
#include "commands.h"

void printBoard(LinkedList** columns, LinkedList** foundations, Command previousCommand, char* lastInput);
char* getUserInput();