#pragma once

#include <stdbool.h>
#include "card.h"
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
    Card* deck;
    MoveStack* moves;
} Game;


bool canUseCommand(GamePhase phase, Command command);
char* performCommand(Game* game, Command command, LinkedList** columns, LinkedList** foundations);
