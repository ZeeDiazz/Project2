#pragma once
#include "card.h"

typedef struct {
    Card card;
    struct Node * next;
} Node;

typedef struct {
    Node* head;
    int size;
}LinkedList;

void addCard(Node* node,Card card);
void addFirst(Node* node, Card card);
void addLast(Node* node, Card card);
void remove(Node* node, Card card);
void printList(Node* node);
bool isEmpty();