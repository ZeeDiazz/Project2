// @author Zigalow
#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 255

/**
 * 
 * @param filename Filename of the file containing the cards that needs to be read
 * @return Returns FileAssessment, which is determined by different factors when reading from file
 */

FileAssessment readDeckFromFile(char *filename) {
    FileAssessment assessment;
    char* error;

    FILE *pFile = fopen(filename, "r");
    char line[BUFFER_SIZE];

    // If provided with the wrong filename
    if (pFile == NULL) {
        assessment.statusCode = FILENOTFOUND;
        error = "Wrong filepath/Couldn't load the file with the give filename";
        assessment.errorMessage = malloc(strlen(error) + 1);
        for (int i = 0; i < strlen(error); i++) {
            assessment.errorMessage[i] = error[i];
        }
        assessment.errorMessage[strlen(error)] = '\0';
        return assessment;
    }


    Card currentCard;
    Card *cards = malloc(52 * sizeof(Card)); // Deck that will be made
    bool checkCards[4][13] = {{0}}; // Card-array for checking for duplicates
    int cardCounter = 0;
    while (fgets(line, BUFFER_SIZE, pFile) != NULL && cardCounter < 52) {
        currentCard = stringToCard(line);
        // If cards is in a wrong card format
        if (currentCard.value == 0 || currentCard.suit == 0) {
            error = "There was a card with the wrong card format on line ";

            int onesValue = (cardCounter + 1) % 10;
            int tensValue = (cardCounter + 1) / 10;

            assessment.errorMessage = malloc(strlen(error) + 1 + (tensValue > 0) + 1);
            for (int i = 0; i < strlen(error); i++) {
                assessment.errorMessage[i] = error[i];
            }

            int numberIndex = 52;
            if (tensValue > 0) {
                assessment.errorMessage[numberIndex++] = tensValue + '0';
            }
            assessment.errorMessage[numberIndex++] = onesValue + '0';
            assessment.errorMessage[numberIndex++] = '\0';

            // sprintf(errorLine, "There was a card with the wrong card format on line %d", cardCounter + 1);

            assessment.statusCode = WRONGCARDFORMAT;
            fclose(pFile);
            return assessment;

            // If a cards has already been added to the cardArray : Duplicate
        } else if (checkCards[currentCard.suit - 1][currentCard.value - 1]) {
            error = "There was a duplicate card on line ";

            int onesValue = (cardCounter + 1) % 10;
            int tensValue = (cardCounter + 1) / 10;

            assessment.errorMessage = malloc(strlen(error) + 1 + (tensValue > 0) + 1);
            for (int i = 0; i < strlen(error); i++) {
                assessment.errorMessage[i] = error[i];
            }

            int numberIndex = 35;
            if (tensValue > 0) {
                assessment.errorMessage[numberIndex++] = tensValue + '0';
            }
            assessment.errorMessage[numberIndex++] = onesValue + '0';
            assessment.errorMessage[numberIndex++] = '\0';

            assessment.statusCode = DUPLICATE;
            fclose(pFile);
            return assessment;
        }
        cards[cardCounter] = currentCard;
        cardCounter++;
        // Sets the checkCards array to true, according the currentCard in the checkArray 
        checkCards[currentCard.suit - 1][currentCard.value - 1] = true;
    }
    fclose(pFile);

    // If there wasn't enough cards to make a full deck
    if (cardCounter != 52) {
        error = "There was not enough cards to make a full deck with the given file. The last read line is line ";

        int onesValue = (cardCounter + 1) % 10;
        int tensValue = (cardCounter + 1) / 10;

        assessment.errorMessage = malloc(strlen(error) + 1 + (tensValue > 0) + 1);
        for (int i = 0; i < strlen(error); i++) {
            assessment.errorMessage[i] = error[i];
        }

        int numberIndex = 95;
        if (tensValue > 0) {
            assessment.errorMessage[numberIndex++] = tensValue + '0';
        }
        assessment.errorMessage[numberIndex++] = onesValue + '0';
        assessment.errorMessage[numberIndex++] = '\0';

        assessment.statusCode = MISSINGCARDS;
        return assessment;
    }

    // If everything goes well

    error = "OK";
    assessment.errorMessage = malloc(strlen(error) + 1);
    for (int i = 0; i < strlen(error); i++) {
        assessment.errorMessage[i] = error[i];
    }
    assessment.errorMessage[strlen(error)] = '\0';
    assessment.statusCode = SUCCESS;
    assessment.deck = cards;
    return assessment;
}

void saveDeckToFile(char *filename, Card *cards) {
    FILE *file = fopen(filename, "w+");
    char *cardsText = malloc(3 * 52);
    int textIndex = 0;
    for (int i = 0; i < 52; i++) {
        bool wasSeen = cards[i].seen;
        cards[i].seen = true;
        char *cardText = cardToString(cards[i], false);
        cards[i].seen = wasSeen;
        for (int j = 0; j < 2; j++) {
            cardsText[textIndex++] = cardText[j];
        }
        cardsText[textIndex++] = '\n';
        free(cardText);
    }

    fputs(cardsText, file);
    free(cardsText);
    fclose(file);
}

void saveGame(char *filename, GameState gameState) {
    saveDeckToFile(filename, gameState.board->deck);

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        return;
    }

    MoveStack *currentMove = gameState.moves;
    while (currentMove != NULL) {
        fputs(currentMove->move, file);
        fputs("\n", file);
        currentMove = currentMove->next;
    }
    fclose(file);
}

LoadInfo loadFromFile(char *filename, GameState* gameState) {
    LoadInfo loadInfo = {SUCCESS, ""};

    FileAssessment fileAssessment = readDeckFromFile(filename);

    if (fileAssessment.statusCode != SUCCESS) {
        loadInfo.errorMessage = fileAssessment.errorMessage;
        loadInfo.statusCode = fileAssessment.statusCode;
        return loadInfo;
    }
    setDeck(gameState->board, fileAssessment.deck);

    FILE *pFile = fopen(filename, "r");
    char line[BUFFER_SIZE];

    int lineCounter = 0;
    MoveStack *moveStack = NULL;
    while (fgets(line, BUFFER_SIZE, pFile) != NULL) {
        lineCounter++;
        if (lineCounter < 52) {
            continue;
        }

        char* move = malloc(strlen(line));
        for (int i = 0; i < strlen(line) - 1; i++) {
            move[i] = line[i];
        }
        move[strlen(line) - 1] = '\0';
        moveStack = addMove(moveStack, move);
    }
    fclose(pFile);

    performCommand(gameState, (Command){Q, NO_ERROR, false, NULL});
    performCommand(gameState, (Command){P, NO_ERROR, false, NULL});
    while (!isEmpty(moveStack)) {
        char *moveToRedo = getMove(moveStack);
        char *actualMove = malloc(strlen(moveToRedo) - 2 + 1);
        for (int i = 0; i < strlen(moveToRedo) - 2; i++) {
            actualMove[i] = moveToRedo[i];
        }
        actualMove[strlen(moveToRedo) - 2] = '\0';
        performCommand(gameState, makeGameMoveCommand(actualMove));
        moveStack = removeMove(moveStack);
        free(actualMove);
    }

    return loadInfo;
}