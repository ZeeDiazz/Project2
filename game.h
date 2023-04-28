#pragma once

#include <stdbool.h>
#include "moveStack.h"
#include "commands.h"
#include "linkedList.h"

typedef enum GamePhase
{
    STARTUP,
    PLAYING,
    FINISHED,
    QUITTING,
} GamePhase;


typedef struct Game
{
    GamePhase phase;
    int totalMoves;
    int currentMove;
    MoveStack* moves;
} Game;


bool canUseCommand(Game game, Command command);
void performCommand(Game* game, Command command, LinkedList** columns, LinkedList** foundations);
