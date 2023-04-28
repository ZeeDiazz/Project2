#include <string.h>
#include "game.h"
#include "moveValidation.h"

bool canUseCommand(Game game, Command command) {
    if (command.name == QQ || !command.isValid) {
        return true;
    }

    switch (game.phase)
    {
        case STARTUP:
            switch (command.name)
            {
                case LD:
                case SW:
                case SI:
                case SR:
                case SD:
                case P:
                    return true;
                default:
                    return false;
            }
            break;
        case PLAYING:
            switch (command.name)
            {
                case Q:
                case MOVE:
                    return true;
                default:
                    return false;
            }
            break;
        default:
            return false;
    }
    return false; // should never reach
}

void performCommand(Game* game, Command command, LinkedList** columns, LinkedList** foundations) {
    switch (command.name)
    {
        case LD:
            if (command.hasArguments) {
                // read the file
            }
            else {
                // make a new deck of cards
            }
            break;
        case SW:
            // show all cards
            break;
        case SI:
            // shuffle the cards
            break;
        case SR:
            // shuffle the cards
            break;
        case SD:
            // save the cards to a file
            break;
        case MOVE:
            // TODO make better when there are more linked list methods
            LinkedList *from, *to;
            Card moving, movingTo;
            bool canMove;
            // Find the place we are moving from
            if (command.arguments[0] == 'C') {
                from = columns[command.arguments[1] - '1'];
            }
            else {
                from = foundations[command.arguments[1] - '1'];
            }

            if (from->size == 0) {
                // TODO error message?
                return;
            }

            // Move the card on top of foundation / at the bottom of column
            if (strlen(command.arguments) == 6) {
                // Get the last card in the moving pile
                moving = getCardAt(from, from->size - 1);

                // Check whether the to is a column or a foundation
                if (command.arguments[4] == 'C') {
                    to = columns[command.arguments[5] - '1'];
                    canMove = canMoveToColumn(moving, getCardAt(to, to->size - 1));
                }
                else {
                    to = foundations[command.arguments[5] - '1'];
                    canMove = canMoveToFoundation(moving, getCardAt(to, to->size - 1));
                }

                // If they can move, do it
                if (canMove) {
                    removeCard(from, moving);
                    addCard(to, moving);
                }
                return;
            }
            // move specific card


            game->totalMoves++;
            game->currentMove++;
            // addMove(game->moves, command.arguments);
            break;
        case QQ:
            game->phase = QUITTING;
            break;
        case P:
            game->phase = PLAYING;
            break;
        case Q:
            game->phase = STARTUP;
            break;
        // Unknown command
        default:
            break;
    }
}
