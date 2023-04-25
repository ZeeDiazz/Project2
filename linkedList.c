#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "linkedList.h"

struct Node *header = NULL, *current = NULL;

void addCard(Node* node,Card card){
}
void addFirst(Card card){
    //allocate memory
}

void addLast(Card card){}

void remove(Card card){
    //Use free to  remove from memory

}

bool isEmpty(){
    return header == NULL; //returns true if the empty
}

