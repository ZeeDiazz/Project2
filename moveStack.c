//
// Created by zegho on 28-04-2023.
//

#include <stdlib.h>
#include "moveStack.h"


MoveStack *addMove(MoveStack *head, char *move) {
    MoveStack *newMove = malloc(sizeof(MoveStack));

    newMove->move = move;
    newMove->next = head;

    return newMove;
}


MoveStack *removeMove(MoveStack *head) {
    MoveStack *newHead = head->next;
    free(head);

    return newHead;
}

MoveStack *makeEmpty(MoveStack *head) {
    while (!isEmpty(head)) {
        head = removeMove(head);
    }

    return NULL;
}

char *getMove(MoveStack *head) {
    return head->move;
}

bool isEmpty(MoveStack *head) {
    return head == NULL;
}




