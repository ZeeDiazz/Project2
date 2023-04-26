#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "linkedList.h"



void addCard(Node* node, Card card){
    Node* testNode = malloc(sizeof(Node));
    testNode->card = card;
    node->next = testNode;
}
void addFirst(Card card){
    //allocate memory
    Node* testNode = malloc(sizeof(Node));
    testNode->card = card;
}


void addLast(Card card){}

void remove(Card card){
    //Use free to  remove from memory

}

bool isEmpty(Node* node) {
    return node == NULL;
}
