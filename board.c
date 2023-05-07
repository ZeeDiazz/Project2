#include <stdio.h>
#include "board.h"

Board* makeBoard(Card* deck) {
    Board* board = malloc(sizeof(Board));
    board->deck = deck;
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
    for (int i = 0; i < 7; i++) {
        addCard(board->columns[i % 7], board->deck[i]);
    }
}

void playMode(Board* board) {
    emptyBoard(board);
    board->deck[0].seen = true;
    addCard(board->columns[0], board->deck[0]);

    int rowCount = 0;
    int columnIndex = 1;
    int deckIndex = 1;
    while (deckIndex < 52) {
        columnIndex++;
        if (columnIndex > 6) {
            columnIndex = 0;
            rowCount++;
        }

        if (rowCount > (columnIndex + 5)) {
            continue;
        }
        board->deck[deckIndex].seen = (rowCount < columnIndex);
        addCard(board->columns[columnIndex + 1], board->deck[deckIndex]);

        deckIndex++;
    }

    for (int i = 0; i < 51; i++) {
        int columnIndex = (i % 6) + 1;
        int cardsInColumn = (i / 6);
        if (cardsInColumn > (columnIndex + 5)) {
            continue;
        }

        board->deck[i + 1].seen = (cardsInColumn < columnIndex);
        addCard(board->columns[(i % 6) + 1], board->deck[i + 1]);
    }
}