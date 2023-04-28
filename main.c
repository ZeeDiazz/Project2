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

    char* userInput = NULL;
    while (game.phase != QUITTING) {
        if (userInput == NULL) {
            printf("Input is null\n");
        }
        else {
            printf("%d\n", strlen(userInput));
        }
        printBoard(columns, foundations, lastCommand, userInput);

        if (userInput != NULL) {
            free(userInput);
        }
        userInput = getUserInput();
        if (userInput == NULL) {
            printf("Ooga booga\n");
            continue;
        }
        // TODO make parse better (take the current phase?)
        lastCommand = parseCommand(userInput);
        if (!canUseCommand(game, lastCommand)) {
            lastCommand.name = INVALID;
            lastCommand.hasArguments = true;
            lastCommand.arguments = "Cannot use this command at this time";
        }
        // If the command was invalid, don't bother the actual game state
        if (lastCommand.name == INVALID || lastCommand.name == UNKNOWN) {
            continue;
        }

        performCommand(&game, lastCommand, columns, foundations);
    }
    printf("Hello, World!\n");
    return 0;
}
