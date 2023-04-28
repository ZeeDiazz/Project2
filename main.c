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

    Command lastCommand = {INVALID, true, ""};
    char inputBuffer[MAX_INPUT_LENGTH];

    Command quitCommand = {QQ, false, ""};
    while (game.phase != QUITTING) {
        printBoard(columns, foundations, lastCommand, inputBuffer);
        fgets(inputBuffer, MAX_INPUT_LENGTH, stdin);
        lastCommand = parseCommand(inputBuffer);
        if (!canUseCommand(game, lastCommand)) {
            lastCommand.name = INVALID;
            lastCommand.hasArguments = true;
            lastCommand.arguments = "Cannot use this command at this time";
        }
        printBoard(columns, foundations, lastCommand, inputBuffer);
        useCommand(&game, lastCommand);
    }
    printf("Hello, World!\n");
    return 0;
}
