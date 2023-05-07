#pragma once
#include "linkedList.h"

typedef struct Board
{
    Card* deck;
    LinkedList** columns;
    LinkedList** foundations;
} Board;

Board* makeBoard(Card* deck);
void emptyBoard(Board* board);
void showAll(Board* board);
void showcaseMode(Board* board);
void playMode(Board* board);
