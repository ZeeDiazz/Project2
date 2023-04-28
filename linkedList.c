#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include "linkedList.h"


LinkedList* makeEmptyList(){
    LinkedList* list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->size = 0;
    return list; //return a pointer to the new empty list
}

/**
 * This function add a card at the beginning of the list.
 * @author ZedDiazz (Zaid)
 * @param list
 * @param card
 */
void addCard(LinkedList* list, Card card){
    Node* node = malloc(sizeof(Node));
    node->card = card;

    list->head = node;
    list->size++;
}
/**
 * @author ZedDiazz (Zaid)
 * @param list
 * @param card
 */

void addFirst(LinkedList* list, Card card){
    //allocate memory
    Node* node = malloc(sizeof(Node));
    node->card = card;

    node->next = list->head->next;

    list->head = node;
    list->size++;
}

void addLast(LinkedList* list, Card card){
    Node* node = malloc(sizeof(Node));
    node->card = card;
    node->next = NULL; //Null since its the last element

    while (list->head != NULL){
        list->head = node;
    }
}


void removeCard(LinkedList* list, Card card) {
    Node *temp = list->head;
    Node *prev = NULL;

    while (temp != NULL) {
        if (temp->card.value == card.value && temp->card.suit == card.suit) {
            list->head->next = temp->next;
            free(temp);
            list->size--; //since we are removing
            return; // to stop the while loop
        }
        prev->card = temp->card;
        temp = temp->next; //iterate through temp
    }
}

Card getCardAt(LinkedList* list, int index){
    //a for loop
    for(int i = 0; i < index;i++){

    }
}

void printList(LinkedList* list){
    //while (node){
    //}
}

bool isEmpty(LinkedList* list) {
    return list->head == NULL;
}
