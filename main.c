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
    Command quitCommand = {QQ, false, ""};
    while (game.phase != QUITTING) {
        useCommand(&game, quitCommand);
    }
    printf("Hello, World!\n");
    return 0;
}
