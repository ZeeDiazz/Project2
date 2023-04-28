#pragma once
#include "card.h"
/**
 * This is a Node, where  it points to the next node
 * @author ZeeDiazz (Zaid)
 */
typedef struct Node {
    Card card;
    struct Node * next;
} Node;
/**
 * This holds a pointer to the head of the node, and the size of the list
 * @author ZeeDiazz (Zaid)
 */
typedef struct LinkedList {
    Node* head;
    int size;
}LinkedList;

LinkedList* makeEmptyList();
void addCard(LinkedList* list,Card card);
bool removeCard(LinkedList* list, Card card);

Card getCardAt(LinkedList* list, int index);
