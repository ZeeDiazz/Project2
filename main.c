#include <stdio.h>
#include <string.h>
#include "card.h"
#include "cli.h"
#include "commands.h"
#include "game.h"
#include "linkedList.h"
#include "moveStack.h"
#include "moveValidation.h"
#include "file.h"
#include "shuffleDeck.h"

int main() {
    GameState game = {STARTUP, 0, 0, makeBoard(), NULL};
    LinkedList** columns = malloc(7 * sizeof(LinkedList*));
    for (int i = 0; i < 7; i++) {
        columns[i] = makeEmptyList();
    }

    LinkedList** foundations = malloc(4 * sizeof(LinkedList*));
    for (int i = 0; i < 4; i++) {
        foundations[i] = makeEmptyList();
    }

    Command lastCommand = {UNKNOWN, false, true, ""};

    char* userInput = NULL;
    char* message = NULL;
    while (game.phase != QUITTING) {
        printBoard(*game.board, lastCommand, userInput, message);

        if (userInput != NULL) {
            free(userInput);
        }
        userInput = getUserInput();
        if (userInput == NULL) {
            continue;
        }
        // TODO make parse better (take the current phase?)
        lastCommand = parseCommand(userInput);
        if (!canUseCommand(game.phase, lastCommand)) {
            lastCommand.error = WRONG_TIME;
            message = "Cannot use this command at this time";
            continue;
        }

        message = performCommand(&game, lastCommand);
        printf("%s\n", message);
    }

    printf("\nProgram terminated\n");
    return 0;
}
