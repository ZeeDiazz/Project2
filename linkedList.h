#pragma once
#include "card.h"

typedef struct {
    Card card;
    struct Node * next;
} Node;

void addCard(Node* node,Card card);
void addFirst(Node* node, Card card);
void addLast(Node* node, Card card);
void remove(Node* node, Card card);
bool isEmpty();