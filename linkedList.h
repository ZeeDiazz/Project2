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

LinkedList* makeEmptyList();
void addCard(LinkedList* list,Card card);
void addFirst(LinkedList* list, Card card);
void addLast(LinkedList* list, Card card);

void remove(LinkedList* list, Card card);

Card getCardAt(LinkedList* list, int index);
void printList(LinkedList* list);

bool isEmpty(LinkedList* list);