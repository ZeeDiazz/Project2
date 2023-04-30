#include <stdbool.h>
#include <stdlib.h>
#include "linkedList.h"

/**
 * This function creates a empty list
 * @author ZeeDiazz (Zaid)
 * @return
 */
LinkedList* makeEmptyList(){
    LinkedList* list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->size = 0;
    return list; //return a pointer to the new empty list
}

/**
 * This function adds a card at end of the list.
 * @author ZeeDiazz (Zaid)
 * @param list
 * @param card
 */
void addCard(LinkedList* list, Card card) {
    Node* node = malloc(sizeof(Node));
    node->card = card;
    node->next = NULL; //Null since it's the last element

    if (list->size == 0) {
        list->head = node;
        list->size++;
        return;
    }

    Node* current = list->head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node; //update the last element in the list
    list->size++;
}

/**
 * This method can remove a card from the list, by using the free.
 * Checks for the edge cases.
 * @author ZeeDiazz (Zaid)
 * @param list
 * @param card
 */
bool removeCard(LinkedList* list, Card card) {
    Node *temp = list->head;
    Node *prev = NULL;

    while (temp != NULL) {
        // if cards are the same
        if (temp->card.value == card.value && temp->card.suit == card.suit) {
            list->size--;
            // if it is the head
            if (prev == NULL) {
                list->head = list->head->next;
            }
            else {
                prev->next = temp->next;
            }
            free(temp);
            return true; // to stop the while loop
        }
        prev = temp;
        temp = temp->next; //iterate through the list
    }
    return false;
}
/**
 * Adds a list on top og another list
 * Adds it at the end of the main list
 * @author ZeeDiazz (Zaid)
 * @param list
 * @param addList
 */
void addList(LinkedList* list, LinkedList* addList) {
    // If the list is empty
    if (list->size == 0) {
        list->head = addList->head;
    }
    else {
        //Iterate to get the last element of list
        Node* temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        //Add addList from where the list ends
        temp->next = addList->head;

    }
    //Update the main list size
    list->size += addList->size;

    //Make the addList empty
    addList->head = NULL;
    addList->size = 0;
}

/**
 * it splits a list into two, and returns the new splited list
 * @author ZeeDiazz (Zaid)
 * @param list
 * @param index
 * @return
 */
LinkedList* splitList(LinkedList* list, int index) {

    LinkedList *tempList = makeEmptyList();
    if (index < 0 || index > list->size - 1) {
        return tempList;
    }

    if (index == 0) {
        tempList->head = list->head;
        tempList->size = list->size;

        list->head = NULL;
        list->size = 0;
        return tempList;
    }

    //iterate through the list to the given index
    Node *temp = list->head;
    for (int i = 0; i < index - 1; i++) {
        temp = temp->next;
    }
    //Set the separated list to temp
    tempList->head = temp->next;

    //Remove all cards above index in the original list
    temp->next = NULL;

    //update the size of tempList
    tempList->size = list->size - index; //includes all from the original list except that before index

    //update the size of original list
    list->size = index; //Because all from index is included in templist

    return tempList; //return temp is temporary
}

/**
 * This method is to get a specific card from the list, and checks if he index is correct
 * @author ZeeDiazz (Zaid)
 * @param list
 * @param index
 * @return
 */
Card getCardAt(LinkedList* list, int index){
    if (index <= 0) {
        return list->head->card;
    } else if (index >= list->size) {
        index = list->size-1;
    }

    //a for loop
    Node *node = list->head;
    for (int i = 0; i < index; i++) {
        node = node->next;
    }
    return node->card;
}

int getCardIndex(LinkedList* list, Card card) {
    Node* current = list->head;
    for (int i = 0; i < list->size; i++) {
        Card currentCard = current->card;
        if (currentCard.suit == card.suit && currentCard.value == card.value) {
            return i;
        }
        current = current->next;
    }
    // Card was not present
    return -1;
}

void emptyList(LinkedList* list) {
    int length = list->size;
    Node* current = list->head;
    Node* next;
    for (int j = 0; j < length; j++) {
        next = current->next;
        free(current);
        current = next;
    }
    list->size = 0;
}
