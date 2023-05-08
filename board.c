#include <stdlib.h>
#include <stdio.h>
#include "board.h"

Board* makeBoard(Card* deck) {
    Board* board = malloc(sizeof(Board));
    board->deck = NULL;
    board->columns = malloc(sizeof(Board*) * 7);
    for (int i = 0; i < 7; i++) {
        board->columns[i] = makeEmptyList();
    }
    board->foundations = malloc(sizeof(Board*) * 4);
    for (int i = 0; i < 4; i++) {
        board->foundations[i] = makeEmptyList();
    }

    return board;
}

void setDeck(Board* board, Card* deck) {
    if (hasDeck(board)) {
        free(board->deck);
    }
    board->deck = deck;
}

bool hasDeck(Board* board) {
    return !(board->deck == NULL);
}

void emptyBoard(Board* board) {
    for (int i = 0; i < 7; i++) {
        emptyList(board->columns[i]);
    }
    for (int i = 0; i < 4; i++) {
        emptyList(board->foundations[i]);
    }
}

void showAll(Board* board) {
    for (int i = 0; i < 7; i++) {
        Node* current = board->columns[i]->head;
        while (current != NULL) {
            current->card.seen = true;
            current = current->next;
        }
    }
}

void showcaseMode(Board* board) {
    emptyBoard(board);
    for (int i = 0; i < 52; i++) {
        addCard(board->columns[i % 7], board->deck[i]);
    }
}

void playMode(Board* board) {
    emptyBoard(board);
    board->deck[0].seen = true;
    addCard(board->columns[0], board->deck[0]);

    int rowCount = 0;
    int columnIndex = -1;
    int deckIndex = 1;
    while (deckIndex < 52) {
        columnIndex++;
        if (columnIndex > 5) {
            columnIndex = 0;
            rowCount++;
        }

        if (rowCount > (columnIndex + 5)) {
            continue;
        }
        board->deck[deckIndex].seen = (rowCount > columnIndex);
        addCard(board->columns[columnIndex + 1], board->deck[deckIndex]);

        deckIndex++;
    }
}

MoveInfo performMove(Board* board, Command command) {
    LinkedList *from, *to;
    Card moving;
    MoveInfo moveError;
    // Find the place we are moving from
    if (command.arguments[0] == 'C') {
        from = board->columns[command.arguments[1] - '1'];
    }
    else {
        from = board->foundations[command.arguments[1] - '1'];
    }

    if (from->size == 0) {
        return NO_CARDS;
    }

    int movingIndex;
    int commandLength = strlen(command.arguments);

    if (commandLength == 6) {
        // Move the very last card
        movingIndex = from->size - 1;

    }
    else {
        char* commandCardString = malloc(3);
        commandCardString[0] = command.arguments[3];
        commandCardString[1] = command.arguments[4];
        commandCardString[2] = '\0';
        Card commandCard = stringToCard(commandCardString);
        free(commandCardString);
        movingIndex = getCardIndex(from, commandCard);
        // If the card could not be found in the from column/foundation
        if (movingIndex == -1) {
            return NO_MATCHES;
        }
    }

    moving = getCardAt(from, movingIndex);
    // If the card hasn't been seen yet, pretend it isn't in the column/foundation
    if (!moving.seen) {
        return NO_MATCHES;
    }

    if (command.arguments[commandLength - 2] == 'C') {
        to = board->columns[command.arguments[commandLength - 1] - '1'];
        moveError = canMoveToColumn(moving, to);
    }
    else {
        // Can only move to foundation a single card at a time
        if (movingIndex != from->size - 1) {
            return ONLY_ONE_CARD_TO_FOUNDATION;
        }
        to = board->foundations[command.arguments[commandLength - 1] - '1'];
        moveError = canMoveToFoundation(moving, to);
    }

    if (to == from) {
        return NO_EFFECT;
    }

    if (moveError != NONE) {
        return moveError;
    }
    
    LinkedList* movingStack = splitList(from, movingIndex);
    addList(to, movingStack);
    
    // If there are cards left in the from column
    if (from->size > 0) {
        Node* temp = from->head;
        for (int i = 0; i < from->size - 1; i++) {
            temp = temp->next;
        }
        if (temp->card.seen) {
            return NONE;
        }
        else {
            temp->card.seen = true;
            return SHOWED_CARD;
        }
    }
    return NONE;
}

void forceMove(Board* board, Command command, bool hideCard);
void forceMove(Board* board, Command command, bool hideCard) {
    LinkedList *from, *to;
    Card moving;

    if (command.arguments[0] == 'C') {
        from = board->columns[command.arguments[1] - '1'];
    }
    else {
        from = board->foundations[command.arguments[1] - '1'];
    }

    int movingIndex;
    int commandLength = strlen(command.arguments);

    if (commandLength == 6) {
        // Move the very last card
        movingIndex = from->size - 1;

    }
    else {
        char* commandCardString = malloc(3);
        commandCardString[0] = command.arguments[3];
        commandCardString[1] = command.arguments[4];
        commandCardString[2] = '\0';
        Card commandCard = stringToCard(commandCardString);
        free(commandCardString);
        movingIndex = getCardIndex(from, commandCard);
    }

    if (command.arguments[commandLength - 2] == 'C') {
        to = board->columns[command.arguments[commandLength - 1] - '1'];
    }
    else {
        // Can only move to foundation a single card at a time
        if (movingIndex != from->size - 1) {
            return ONLY_ONE_CARD_TO_FOUNDATION;
        }
        to = board->foundations[command.arguments[commandLength - 1] - '1'];
    }
    // Set the correct visibility for the cards
    if (to->size > 0) {
        Node* temp = to->head;
        for (int i = 0; i < to->size - 1; i++) {
            temp = temp->next;
        }
        temp->card.seen = !hideCard;
    }

    LinkedList* movingStack = splitList(from, movingIndex);
    addList(to, movingStack);
    if (from->size > 0) {
        Node* temp = from->head;
        for (int i = 0; i < from->size - 1; i++) {
            temp = temp->next;
        }
        temp->card.seen = true;
    }
}

char* performUndo(Board* board, char* moveToUndo) {
    int reversedMoveLength;
    char* reversedMove;

    int moveToUndoLength = strlen(moveToUndo) - 2;

    if (moveToUndoLength == 6 || moveToUndo[4] == 'F') {
        reversedMoveLength = 6;
        reversedMove = malloc(reversedMoveLength + 1);
    }
    else {
        reversedMoveLength = 9;
        reversedMove = malloc(reversedMoveLength + 1);
        reversedMove[2] = ':';
        reversedMove[3] = moveToUndo[3];
        reversedMove[4] = moveToUndo[4];
    }
    reversedMove[reversedMoveLength] = '\0';

    reversedMove[0] = moveToUndo[moveToUndoLength - 2];
    reversedMove[1] = moveToUndo[moveToUndoLength - 1];
    reversedMove[reversedMoveLength - 2] = moveToUndo[0];
    reversedMove[reversedMoveLength - 1] = moveToUndo[1];
    reversedMove[reversedMoveLength - 3] = moveToUndo[moveToUndoLength - 3];
    reversedMove[reversedMoveLength - 4] = moveToUndo[moveToUndoLength - 4];

    bool shouldHideCard = moveToUndo[moveToUndoLength + 1] == '1';

    printf("Move: %s\nReversed: %s\n", moveToUndo, reversedMove);

    forceMove(board, makeGameMoveCommand(reversedMove), shouldHideCard);
    char* undoMove = malloc(strlen(moveToUndo) + 1);
    for (int i = 0; i < reversedMoveLength; i++) {
        undoMove[i] = reversedMove[i];
    }
    undoMove[strlen(moveToUndo) - 2] = ' ';
    undoMove[strlen(moveToUndo) - 1] = !shouldHideCard + '0';
    undoMove[strlen(moveToUndo)] = '\0';
    printf("Move: %s\nUndo: %s\n", moveToUndo, undoMove);
    return undoMove;
}

bool allCardsInFoundation(Board* board) {
    int cardsInFoundation = 0;
    for (int i = 0; i < 4; i++) {
        cardsInFoundation += board->foundations[i]->size;
    }
    return cardsInFoundation == 52;
}
