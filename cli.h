#pragma once

#include <stdlib.h>
#include "card.h"
#include "list.h"
#include "commands.h"

void printBoard(CardList** columns, CardList** foundations, Command previousCommand);