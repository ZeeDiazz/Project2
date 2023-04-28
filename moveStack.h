//
// Created by zegho on 28-04-2023.
//
#pragma once


#include <stdbool.h>

typedef struct MoveStack {
    char* move;
    struct MoveStack *next;
} MoveStack;

MoveStack* addMove(MoveStack* head, char* move);
MoveStack* removeMove(MoveStack* head);
char* getMove(MoveStack* head);
bool isEmpty(MoveStack* head);