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
        // TODO clean the input
        fgets(inputBuffer, MAX_INPUT_LENGTH, stdin);
        // TODO make parse better (take the current phase?)
        lastCommand = parseCommand(inputBuffer);
        if (!canUseCommand(game, lastCommand)) {
            lastCommand.name = INVALID;
            lastCommand.hasArguments = true;
            lastCommand.arguments = "Cannot use this command at this time";
        }
        // If the command was invalid, don't bother the actual game state
        if (lastCommand.name == INVALID) {
            continue;
        }

        performCommand(&game, lastCommand, columns, foundations);
    }
    printf("Hello, World!\n");
    return 0;
}
