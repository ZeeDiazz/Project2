#include <string.h>
#include <stdio.h>
#include "game.h"
#include "moveValidation.h"
#include "readFile.h"

bool canUseCommand(Game game, Command command) {
    if (command.name == QQ || command.error != NO_ERROR) {
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

char* performCommand(Game* game, Command command, LinkedList** columns, LinkedList** foundations) {
    switch (command.error)
    {
        case NO_ERROR:
            break;
        case WRONG_TIME:
            return "Cannot use this command at this time";
        case MALFORMED:
            return "Malformed command";
        case TOO_MANY_ARGUMENTS:
            return "Too many arguments";
        case TOO_FEW_ARGUMENTS:
            return "Too few arguements";
        default:
            return "Invalid command";
    }

    switch (command.name)
    {
        case LD:
            Card* deck;
            if (command.hasArguments) {
                FileAssessment assessment = readFromFile(command.arguments);
                switch (assessment.statusCode)
                {
                    case SUCCESS:
                        deck = assessment.deck;
                        break;
                    default:
                        return assessment.errorMessage;
                }
            }
            else {
                deck = makeDeck();
            }

            // Empty the columns before putting more stuff into them
            for (int i = 0; i < 7; i++) {
                LinkedList* column = columns[i];
                int length = column->size;
                Node* current = column->head;
                Node* next = current->next;
                for (int j = 0; j < length; j++) {
                    free(current);
                    current = next;
                    next = current->next;
                }
                column->size = 0;
            }

            // Add the cards
            for (int i = 0; i < 52; i++) {
                addCard(columns[i % 7], deck[i]);
            }
            return "OK";
        case SW:
            for (int columnIndex = 0; columnIndex < 7; columnIndex++) {
                LinkedList* column = columns[columnIndex];
                Node* node = column->head;
                for (int i = 0; i < column->size; i++) {
                    node->card.seen = true;
                    node = node->next;
                }
            }
            return "OK";
        case SI:
            // shuffle the cards
            return "OK";
        case SR:
            // shuffle the cards
            return "OK";
        case SD:
            // save the cards to a file
            return "OK";
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
                return "There are no cards to move there";
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
                    return "OK";
                }
                return "Couldn't move card";
            }
            // move specific card


            game->totalMoves++;
            game->currentMove++;
            // addMove(game->moves, command.arguments);
            return "OK";
        case QQ:
            game->phase = QUITTING;
            return "OK";
        case P:
            game->phase = PLAYING;
            return "OK";
        case Q:
            game->phase = STARTUP;
            return "OK";
        // Unknown command
        default:
            return "Unknown command";
    }
    // Should never reach
    return "Unknown error";
}
