// @author Zigalow
#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include <stdbool.h>

#define BUFFER_SIZE 255
/**
 * 
 * @param filename Filename of the file containing the cards that needs to be read
 * @return Returns FileAssessment, which is determined by different factors when reading from file
 */
 
FileAssessment readDeckFromFile(char *filename) {

    FileAssessment assessment;

    FILE *pFile = fopen(filename, "r");
    char line[BUFFER_SIZE];

    // If provided with the wrong filename
    if (pFile == NULL) {
        assessment.statusCode = FILENOTFOUND;
        assessment.errorMessage = "Wrong filepath/Couldn't load the file with the give filename";
        return assessment;
    }


    Card currentCard;
    Card* cards = malloc(52 * sizeof(Card)); // Deck that will be made
    bool checkCards[4][13] = {{0}}; // Card-array for checking for duplicates
    int cardCounter = 0;
    while (fgets(line, BUFFER_SIZE, pFile) != NULL && cardCounter < 52) {
        currentCard = stringToCard(line);
        // If cards is in a wrong card format
        if (currentCard.value == NULL || currentCard.suit == NULL) {
            
            char errorLine[BUFFER_SIZE];

            sprintf(errorLine, "There was a card with the wrong card format on line %d", cardCounter + 1);

            assessment.statusCode = WRONGCARDFORMAT;
            assessment.errorMessage = errorLine;
            fclose(pFile);
            return assessment;
            
            // If a cards has already been added to the cardArray : Duplicate
        } else if (checkCards[currentCard.suit - 1][currentCard.value - 1]) {
            
            char errorLine[BUFFER_SIZE];

            sprintf(errorLine, "There was a duplicate card on line %d", cardCounter + 1);

            assessment.statusCode = DUPLICATE;
            assessment.errorMessage = errorLine;
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
        
        char errorLine[BUFFER_SIZE];

        sprintf(errorLine,
                "There was not enough cards to make a full deck with the given file. The last read line is line %d",
                cardCounter + 1);

        assessment.statusCode = MISSINGCARDS;
        assessment.errorMessage = errorLine;

        return assessment;
    }

    // If everything goes well

    assessment.statusCode = SUCCESS;
    assessment.errorMessage = "OK";
    assessment.deck = cards;

    return assessment;
}

void saveDeckToFile(char* filename, Card* cards) {
    FILE *file = fopen(filename, "w+");
    char* cardsText = malloc(3 * 52);
    int textIndex = 0;
    for (int i = 0; i < 52; i++) {
        bool wasSeen = cards[i].seen;
        cards[i].seen = true;
        char* cardText = cardToString(cards[i]);
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
