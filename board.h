#pragma once

#include <stdbool.h>
#include "linkedList.h"
#include "commands.h"
#include "moveValidation.h"

typedef struct Board
{
    Card* deck;
    LinkedList** columns;
    LinkedList** foundations;
} Board;

Board* makeBoard();
void setDeck(Board* board, Card* deck);
bool hasDeck(Board* board);
void emptyBoard(Board* board);
void showAll(Board* board);
void showcaseMode(Board* board);
void playMode(Board* board);

MoveInfo performMove(Board* board, Command command);
char* performUndo(Board* board, char* moveToUndo);
bool allCardsInFoundation(Board* board);
