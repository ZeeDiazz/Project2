#pragma once

#include <stdbool.h>
#include "card.h"
#include "moveStack.h"
#include "commands.h"
#include "linkedList.h"
#include "board.h"

typedef enum GamePhase
{
    STARTUP,
    PLAYING,
    FINISHED,
    QUITTING,
} GamePhase;


typedef struct GameState
{
    GamePhase phase;
    int totalMoves;
    int currentMove;
    Board* board;
    MoveStack* moves;
} GameState;


bool canUseCommand(GamePhase phase, Command command);
char* performCommand(GameState* game, Command command);
