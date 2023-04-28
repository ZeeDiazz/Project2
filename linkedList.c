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


