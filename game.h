#pragma once

#include <stdbool.h>
#include "moveStack.h"
#include "commands.h"

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
void useCommand(Game* game, Command command);
