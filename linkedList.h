#pragma once
#include "card.h"

typedef struct {
    Card card;
    struct Node * next;
} Node;

void addCard(Node* node,Card card);
void addFirst(Card card);
void addLast(Card card);
void remove(Card card);
bool isEmpty();