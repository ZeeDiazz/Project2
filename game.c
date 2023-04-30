#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "moveValidation.h"
#include "readFile.h"

bool canUseCommand(GamePhase phase, Command command) {
    if (command.name == QQ || command.error != NO_ERROR) {
        return true;
    }

    switch (phase)
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
            if (command.hasArguments) {
                FileAssessment assessment = readFromFile(command.arguments);
                switch (assessment.statusCode)
                {
                    case SUCCESS:
                        game->startingDeck = assessment.deck;
                        break;
                    default:
                        return assessment.errorMessage;
                }
            }
            else {
                game->startingDeck = makeDeck();
            }

            // Empty the columns before putting more stuff into them
            for (int i = 0; i < 7; i++) {
                LinkedList* column = columns[i];
                int length = column->size;
                Node* current = column->head;
                Node* next;
                for (int j = 0; j < length; j++) {
                    next = current->next;
                    free(current);
                    current = next;
                }
                column->size = 0;
            }

            // Add the cards
            for (int i = 0; i < 52; i++) {
                addCard(columns[i % 7], game->startingDeck[i]);
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
            LinkedList *from, *to;
            Card moving;
            MoveError moveError;
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
                    return "Could not find the specified card";
                }
            }

            moving = getCardAt(from, movingIndex);
            // If the card hasn't been seen yet, pretend it isn't in the column/foundation
            if (!moving.seen) {
                return "Could not find the specified card";
            }

            if (command.arguments[commandLength - 2] == 'C') {
                to = columns[command.arguments[commandLength - 1] - '1'];
                moveError = canMoveToColumn(moving, to);
            }
            else {
                // Can only move to foundation a single card at a time
                if (movingIndex != from->size - 1) {
                    return "Cannot move more than one card to foundation at a time";
                }
                to = foundations[command.arguments[commandLength - 1] - '1'];
                moveError = canMoveToFoundation(moving, to);

                int cardsInFoundation = 0;
                for (int i = 0; i < 4; i++) {
                    cardsInFoundation += foundations[i]->size;
                }
                // If all 52 cards are in the foundations, the game is finished
                if (cardsInFoundation == 51 && moveError == NONE) {
                    game->phase = STARTUP;
                    return "You won!";
                }
            }

            switch (moveError)
            {
                case NONE:
                    break;
                case SAME_SUIT:
                    return "Cannot move card onto another card of the same suit";
                case DIFFERENT_SUIT:
                    return "Cards in foundation have to be the same suit";
                case EMPTY_COLUMN:
                    return "Only kings can move to empty columns";
                case EMPTY_FOUNDATION:
                    return "Only aces can move to empty foundations";
                case COLUMN_ERROR:
                    return "The card you're moving has to be 1 value lower than the card you're moving to";
                case FOUNDATION_ERROR:
                    return "The cards have to be in order when moving to the foundation";
                default:
                    return "Illegal move";
            }
            if (moveError != NONE) {
                return "This move is illegal";
            }

            LinkedList* movingStack = splitList(from, movingIndex);
            addList(to, movingStack);

            game->totalMoves++;
            game->currentMove++;
            // TODO
            // addMove(game->moves, command.arguments);
            return "OK";
        case QQ:
            game->phase = QUITTING;
            return "OK";
        case P:
            if (game->startingDeck == NULL) {
                return "No deck";
            }
            // Empty the columns before putting more stuff into them
            for (int i = 0; i < 7; i++) {
                LinkedList* column = columns[i];
                int length = column->size;
                Node* current = column->head;
                Node* next;
                for (int j = 0; j < length; j++) {
                    next = current->next;
                    free(current);
                    current = next;
                }
                column->size = 0;
            }

            Card card = game->startingDeck[0];
            card.seen = true;
            addCard(columns[0], card);

            int currentCardIndex = 1;
            int currentColumnIndex = -1;
            int rowCount = 0;
            while (currentCardIndex < 52) {
                currentColumnIndex++;
                if (currentColumnIndex > 5) {
                    currentColumnIndex = 0;
                    rowCount++;
                }

                if (rowCount - currentColumnIndex > 5) {
                    continue;
                }

                LinkedList* column = columns[currentColumnIndex + 1];
                card = game->startingDeck[currentCardIndex++];
                card.seen = (rowCount > currentColumnIndex);
                addCard(column, card);
            }

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
