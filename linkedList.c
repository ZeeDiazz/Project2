#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "linkedList.h"



void addCard(Node* node, Card card){
    Node* testNode = malloc(sizeof(Node));
    testNode->card = card;
    node->next = testNode;
}
void addFirst(Node* node, Card card){
    //allocate memory
    Node* testNode = malloc(sizeof(Node));
    testNode->card = card;
    testNode->next = node->next;
    node->next = testNode;
}

void addLast(Node* node, Card card){
    Node* testNode = malloc(sizeof(Node));
    testNode->card = card;

    while (node->next != NULL){
        node->next = testNode;
    }
}

void remove(Node* node, Card card){

}

void printList(Node* node){
    //while (node){
    //}
}

bool isEmpty(Node* node) {
    return node == NULL;
}
