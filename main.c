#include <stdio.h>
#include "card.h"
#include "cli.h"
#include "commands.h"
#include "game.h"
#include "linkedList.h"
#include "moveStack.h"
#include "moveValidation.h"
#include "readFile.h"
#include "shuffleDeck.h"

#define MAX_INPUT_LENGTH 256

int main() {
    Game game = {STARTUP, 0, 0, NULL};
    LinkedList** columns = malloc(7 * sizeof(LinkedList*));
    for (int i = 0; i < 7; i++) {
        columns[i] = makeEmptyList();
    }

    LinkedList** foundations = malloc(4 * sizeof(LinkedList*));
    for (int i = 0; i < 4; i++) {
        foundations[i] = makeEmptyList();
    }

    Command lastCommand = {INVALID, false, ""};
    char inputBuffer[MAX_INPUT_LENGTH];

    Command quitCommand = {QQ, false, ""};
    while (game.phase != QUITTING) {
        printBoard(columns, foundations, lastCommand, inputBuffer);
        fgets(inputBuffer, MAX_INPUT_LENGTH, stdin);
        lastCommand = parseCommand(inputBuffer);
        printBoard(columns, foundations, lastCommand, inputBuffer);
        useCommand(&game, quitCommand);
    }
    printf("Hello, World!\n");
    return 0;
}
