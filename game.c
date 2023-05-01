#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "moveValidation.h"
#include "file.h"

/* PRIVATE METHODS */
void showDeck(Card* deck, LinkedList** columns, LinkedList** foundations);
void showDeck(Card* deck, LinkedList** columns, LinkedList** foundations) {       
    // Empty the columns before putting more stuff into them
    for (int i = 0; i < 7; i++) {
        emptyList(columns[i]);
    }
    // Empty the foundations as well
    for (int i = 0; i < 4; i++) {
        emptyList(foundations[i]);
    }

    // Add the cards
    for (int i = 0; i < 52; i++) {
        addCard(columns[i % 7], deck[i]);
    }
}

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
                case AUTO:
                case MOVE:
                case RESTART:
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
                FileAssessment assessment = readDeckFromFile(command.arguments);
                switch (assessment.statusCode)
                {
                    case SUCCESS:
                        game->deck = assessment.deck;
                        break;
                    default:
                        return assessment.errorMessage;
                }
            }
            else {
                game->deck = makeDeck();
            }
            showDeck(game->deck, columns, foundations);
            return "OK";
        case SW:
            if (game->deck == NULL) {
                return "No deck";
            }
            for (int i = 0; i < 52; i++) {
                game->deck[i].seen = true;
            }
            showDeck(game->deck, columns, foundations);
            return "OK";
        case SI:
            if (game->deck == NULL) {
                return "No deck";
            }
            // shuffle the cards
            return "OK";
        case SR:
            if (game->deck == NULL) {
                return "No deck";
            }
            // shuffle the cards
            return "OK";
        case SD:
            if (game->deck == NULL) {
                return "No deck";
            }
            char* filename = (command.hasArguments) ? command.arguments : "cards.txt";
            saveDeckToFile(filename, game->deck);
            return "OK";
        // TODO can make use of perform command?
        case AUTO:
            bool movedSomething;

            do {
                movedSomething = false;
                for (int columnIndex = 0; columnIndex < 7; columnIndex++) {
                    LinkedList* column = columns[columnIndex];
                    if (column->size == 0) {
                        continue;
                    }
                    Card bottomCard = getCardAt(column, column->size - 1);

                    for (int foundationIndex = 0; foundationIndex < 4; foundationIndex++) {
                        MoveError error = canMoveToFoundation(bottomCard, foundations[foundationIndex]);
                        if (error == NONE) {
                            // Move the card, and mark it as seen
                            bottomCard.seen = true;
                            removeCard(column, bottomCard);
                            addCard(foundations[foundationIndex], bottomCard);
                            movedSomething = true;

                            if (column->size > 0) {
                                // Get the new bottom card, to mark it as seen
                                bottomCard = getCardAt(column, column->size - 1);
                                removeCard(column, bottomCard);
                                bottomCard.seen = true;
                                addCard(column, bottomCard);
                            }
                            break;
                        }
                    }
                }
            } while (movedSomething);

            int cardsInFoundation = 0;
            for (int i = 0; i < 4; i++) {
                cardsInFoundation += foundations[i]->size;
            }
            // If all 52 cards are in the foundations, the game is finished
            if (cardsInFoundation == 52) {
                game->phase = STARTUP;
                return "You won!";
            }
            return "OK";
        case RESTART:
            if (game->deck == NULL) {
                // TODO undo until at start
                break;
            }
            else {
                Command quit = {Q, NO_ERROR, false, NULL};
                performCommand(game, quit, columns, foundations);
                Command play = {P, NO_ERROR, false, NULL};
                return performCommand(game, play, columns, foundations);
            }
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

            if (to == from) {
                return "Your move did nothing";
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
            
            // If there are cards left in the from column
            if (from->size > 0) {
                Node* temp = from->head;
                for (int i = 0; i < from->size - 1; i++) {
                    temp = temp->next;
                }
                temp->card.seen = true;
            }

            game->totalMoves++;
            game->currentMove++;
            // TODO
            // addMove(game->moves, command.arguments);
            return "OK";
        case QQ:
            game->phase = QUITTING;
            return "OK";
        case P:
            if (game->deck == NULL) {
                return "No deck";
            }
            // Empty the columns before putting more stuff into them
            for (int i = 0; i < 7; i++) {
                emptyList(columns[i]);
            }
            // Empty the foundations as well
            for (int i = 0; i < 4; i++) {
                emptyList(foundations[i]);
            }

            Card card = game->deck[0];
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
                card = game->deck[currentCardIndex++];
                card.seen = (rowCount > currentColumnIndex);
                addCard(column, card);
            }

            game->phase = PLAYING;
            return "OK";
        case Q:
            game->phase = STARTUP;
            // TODO empty the move stack
            return "OK";
        // Unknown command
        default:
            return "Unknown command";
    }
    // Should never reach
    return "Unknown error";
}
