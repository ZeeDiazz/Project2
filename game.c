#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "moveValidation.h"
#include "file.h"
#include "board.h"
#include "shuffleDeck.h"

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

char* performCommand(GameState* game, Command command) {
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
                        setDeck(game->board, assessment.deck);
                        break;
                    default:
                        return assessment.errorMessage;
                }
            }
            else {
                setDeck(game->board, makeDeck());
            }
            showcaseMode(game->board);
            return "OK";
        case SW:
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            showcaseMode(game->board);
            showAll(game->board);
            return "OK";
        case SI:
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            int splitIndex;
            if (command.hasArguments) {
                splitIndex = command.arguments[0] - '0';
                if (strlen(command.arguments) == 2) {
                    splitIndex *= 10;
                    splitIndex += command.arguments[1] - '0';
                }
            }
            else {
                splitIndex = rand() % 52;
            }

            LinkedList* shuffledInterleaved = makeEmptyList();
            for (int i = 0; i < 52; i++) {
                addCard(shuffledInterleaved, game->board->deck[i]);
            }
            shuffleInterleaved(shuffledInterleaved, splitIndex);
            Card* interleavedDeck = malloc(52 * sizeof(Card));
            for (int i = 0; i < 52; i++) {
                interleavedDeck[i] = getCardAt(shuffledInterleaved, 0);
                removeCard(shuffledInterleaved, interleavedDeck[i]);
            }
            setDeck(game->board, interleavedDeck);
            return "OK";
        case SR:
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            LinkedList* randomlyShuffled = makeEmptyList();
            for (int i = 0; i < 52; i++) {
                addCard(randomlyShuffled, game->board->deck[i]);
            }
            shuffleRandom(randomlyShuffled);
            Card* randomDeck = malloc(52 * sizeof(Card));
            for (int i = 0; i < 52; i++) {
                randomDeck[i] = getCardAt(randomlyShuffled, 0);
                removeCard(randomlyShuffled, randomDeck[i]);
            }
            setDeck(game->board, randomDeck);
            return "OK";
        case SD:
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            char* filename = (command.hasArguments) ? command.arguments : "cards.txt";
            saveDeckToFile(filename, game->board->deck);
            return "OK";
        // TODO can make use of perform command?
        case AUTO:
            bool movedSomething;

            do {
                movedSomething = false;
                for (int columnIndex = 0; columnIndex < 7; columnIndex++) {
                    LinkedList* column = game->board->columns[columnIndex];
                    if (column->size == 0) {
                        continue;
                    }
                    Card bottomCard = getCardAt(column, column->size - 1);

                    for (int foundationIndex = 0; foundationIndex < 4; foundationIndex++) {
                        MoveError error = canMoveToFoundation(bottomCard, game->board->foundations[foundationIndex]);
                        if (error == NONE) {
                            // Move the card, and mark it as seen
                            bottomCard.seen = true;
                            removeCard(column, bottomCard);
                            addCard(game->board->foundations[foundationIndex], bottomCard);
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
                cardsInFoundation += game->board->foundations[i]->size;
            }
            // If all 52 cards are in the foundations, the game is finished
            if (cardsInFoundation == 52) {
                game->phase = STARTUP;
                return "You won!";
            }
            return "OK";
        case RESTART:
            Command quit = {Q, NO_ERROR, false, NULL};
            performCommand(game, quit);
            Command play = {P, NO_ERROR, false, NULL};
            return performCommand(game, play);
        case MOVE:
            MoveError moveError = performMove(game->board, command);
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
                case NO_CARDS:
                    return "There are no cards to move there";
                case NO_MATCHES:
                    return "Could not find the specified card";
                case ONLY_ONE_CARD_TO_FOUNDATION:
                    return "Cannot move more than one card to foundation at a time";
                case NO_EFFECT:
                    return "Your move did nothing";
                default:
                    return "This move is illegal";
            }

            // If all 52 cards are in the foundations, the game is finished
            if (allCardsInFoundation(game->board)) {
                game->phase = STARTUP;
                return "You won!";
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
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            playMode(game->board);
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
