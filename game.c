#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "game.h"
#include "moveValidation.h"
#include "file.h"
#include "board.h"
#include "shuffleDeck.h"
#include "moveStack.h"

/* PRIVATE METHODS */
void showDeck(Card *deck, LinkedList **columns, LinkedList **foundations);

void showDeck(Card *deck, LinkedList **columns, LinkedList **foundations) {
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

    switch (phase) {
        case STARTUP:
            switch (command.name) {
                case LD:
                case SW:
                case SI:
                case SR:
                case SD:
                case P:
                case L:
                    return true;
                default:
                    return false;
            }
            break;
        case PLAYING:
            switch (command.name) {
                case Q:
                case U:
                case R:
                case S:
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

char *performCommand(GameState *game, Command command) {
    switch (command.error) {
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

    switch (command.name) {
        case LD:
            if (command.hasArguments) {
                FileAssessment assessment = readDeckFromFile(command.arguments);
                switch (assessment.statusCode) {
                    case SUCCESS:
                        setDeck(game->board, assessment.deck);
                        break;
                    default:
                        return assessment.errorMessage;
                }
            } else {
                setDeck(game->board, makeDeck());
            }
            showcaseMode(game->board);
            char* ldMessage = malloc(3);
            ldMessage[0] = 'O';
            ldMessage[1] = 'K';
            ldMessage[2] = '\0';
            game->cardsShown = false;
            return ldMessage;
        case SW:
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            showcaseMode(game->board);
            showAll(game->board);
            game->cardsShown = true;
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
            } else {
                splitIndex = rand() % 52;
            }

            LinkedList *shuffledInterleaved = makeEmptyList();
            for (int i = 0; i < 52; i++) {
                addCard(shuffledInterleaved, game->board->deck[i]);
            }
            shuffleInterleaved(shuffledInterleaved, splitIndex);
            Card *interleavedDeck = malloc(52 * sizeof(Card));
            for (int i = 0; i < 52; i++) {
                interleavedDeck[i] = getCardAt(shuffledInterleaved, 0);
                removeCard(shuffledInterleaved, interleavedDeck[i]);
            }
            setDeck(game->board, interleavedDeck);
            if (game->cardsShown) {
                showcaseMode(game->board);
                showAll(game->board);
            }
            return "OK";
        case SR:
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            LinkedList *randomlyShuffled = makeEmptyList();
            for (int i = 0; i < 52; i++) {
                addCard(randomlyShuffled, game->board->deck[i]);
            }
            shuffleRandom(randomlyShuffled);
            Card *randomDeck = malloc(52 * sizeof(Card));
            for (int i = 0; i < 52; i++) {
                randomDeck[i] = getCardAt(randomlyShuffled, 0);
                removeCard(randomlyShuffled, randomDeck[i]);
            }
            setDeck(game->board, randomDeck);
            if (game->cardsShown) {
                showcaseMode(game->board);
                showAll(game->board);
            }
            return "OK";
        case SD:
            if (!hasDeck(game->board)) {
                return "No deck";
            }
            char *filename = (command.hasArguments) ? command.arguments : "cards.txt";
            saveDeckToFile(filename, game->board->deck);
            return "OK";
        case AUTO:
            bool movedSomething;
            char *moveCommand = malloc(6 + 1);

            moveCommand[0] = 'C';
            moveCommand[2] = '-';
            moveCommand[3] = '>';
            moveCommand[4] = 'F';
            moveCommand[6] = '\0';

            do {
                movedSomething = false;
                for (int columnIndex = 0; columnIndex < 7; columnIndex++) {
                    moveCommand[1] = columnIndex + '1';
                    for (int foundationIndex = 0; foundationIndex < 4; foundationIndex++) {
                        moveCommand[5] = foundationIndex + '1';
                        // Literally just try the move
                        MoveInfo moveInfoCode = performMove(game->board, makeGameMoveCommand(moveCommand));
                        if (moveInfoCode == NONE || moveInfoCode == SHOWED_CARD) {
                            movedSomething = true;
                            game->undoneMoves = makeEmpty(game->undoneMoves);

                            char *moveInfo = malloc(strlen(moveCommand) + 2 + 1);
                            for (int i = 0; i < strlen(moveCommand); i++) {
                                moveInfo[i] = moveCommand[i];
                            }
                            moveInfo[strlen(moveCommand)] = ' ';
                            moveInfo[strlen(moveCommand) + 1] = moveInfoCode + '0';
                            moveInfo[strlen(moveCommand) + 2] = '\0';
                            game->moves = addMove(game->moves, moveInfo);
                        }
                    }
                }
            } while (movedSomething);
            free(moveCommand);

            if (allCardsInFoundation(game->board)) {
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
            MoveInfo moveInfoCode = performMove(game->board, command);
            switch (moveInfoCode) {
                case NONE:
                case SHOWED_CARD:
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
            game->undoneMoves = makeEmpty(game->undoneMoves);
            char *moveInfo = malloc(strlen(command.arguments) + 2 + 1);
            for (int i = 0; i < strlen(command.arguments); i++) {
                moveInfo[i] = command.arguments[i];
            }
            moveInfo[strlen(command.arguments)] = ' ';
            moveInfo[strlen(command.arguments) + 1] = moveInfoCode + '0';
            moveInfo[strlen(command.arguments) + 2] = '\0';
            game->moves = addMove(game->moves, moveInfo);
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
            game->moves = makeEmpty(game->moves);
            return "OK";
        case U:
            if (isEmpty(game->moves)) {
                return "Cannot undo";
            }
            char *moveToUndo = getMove(game->moves);
            performUndo(game->board, moveToUndo);
            game->undoneMoves = addMove(game->undoneMoves, moveToUndo);
            game->moves = removeMove(game->moves);
            return "OK";
        case R:
            if (isEmpty(game->undoneMoves)) {
                return "Cannot redo";
            }
            char *moveToRedo = getMove(game->undoneMoves);
            char *actualMove = malloc(strlen(moveToRedo) - 2 + 1);
            for (int i = 0; i < strlen(moveToRedo) - 2; i++) {
                actualMove[i] = moveToRedo[i];
            }
            actualMove[strlen(moveToRedo) - 2] = '\0';
            performMove(game->board, makeGameMoveCommand(actualMove));
            game->moves = addMove(game->moves, moveToRedo);
            game->undoneMoves = removeMove(game->undoneMoves);
            return "OK";
        case S:
            saveGame(command.arguments, *game);
            return "OK";

        case L:

            LoadInfo loadInfo = loadFromFile(command.arguments, game);
            if (loadInfo.statusCode != SUCCESS) {
                printf("Some kind of error\n");
                return loadInfo.errorMessage;
            }
            return "OK";
        // Unknown command
        default:
            return "Unknown command";
    }
    // Should never reach
    return "Unknown error";
}
