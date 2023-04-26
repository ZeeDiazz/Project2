#include <stdio.h>
#include <ctype.h>
#include "readFile.h"
#include <stdbool.h>

#define BUFFER_SIZE 128

FileAssessment readFromFile(char *filename) {

    FileAssessment assessment;

    FILE *pFile = fopen(filename, "r");
    char line[BUFFER_SIZE];

    // Fails if provided with the wrong filename
    if (pFile == NULL) {
        assessment.statusCode = FILENOTFOUND;
        assessment.errorMessage = "Wrong filepath/Couldn't load the file with the give filename";
        return assessment;
    }


    Card currentCard;
    Card cards[52]; // Deck that will be made
    bool checkCards[4][13] = {{0}}; // For checking if deck contains all cards
    int cardCounter = 0;
    while (fgets(line, BUFFER_SIZE, pFile) != NULL && cardCounter < 52) {
        currentCard = assignCard(line);
        if (currentCard.value == NULL || currentCard.suit == NULL) {

            // StatusCode
            char errorLine[BUFFER_SIZE];

            sprintf(errorLine, "There was a card with the wrong card format on line %d", cardCounter + 1);

            assessment.statusCode = WRONGCARDFORMAT;
            assessment.errorMessage = errorLine;
            fclose(pFile);
            return assessment;
        } else if (checkCards[currentCard.suit - 1][currentCard.value - 1]) {
            // StatusCode
            char errorLine[BUFFER_SIZE];

            sprintf(errorLine, "There was a duplicate card on line %d", cardCounter + 1);

            assessment.statusCode = DUPLICATE;
            assessment.errorMessage = errorLine;
            fclose(pFile);
            return assessment;
        }
        cards[cardCounter] = currentCard;
        cardCounter++;
        checkCards[currentCard.suit - 1][currentCard.value - 1] = true;
    }
    fclose(pFile);

    if (cardCounter != 52) {
        // StatusCode
        char errorLine[BUFFER_SIZE];

        sprintf(errorLine,
                "There was not enough cards to make a full deck with the given file. The last read line is line %d",
                cardCounter + 1);

        assessment.statusCode = MISSINGCARDS;
        assessment.errorMessage = errorLine;
        assessment.deck = makeDeck();

        return assessment;
    }


    assessment.statusCode = SUCCESS;
    assessment.errorMessage = "OK";
    Deck *deck = {cards};
    assessment.deck = *deck;

    return assessment;
}


Card assignCard(char *lineFromFile) {

    Card card;

    switch (toupper(lineFromFile[0])) {
        case 'A':
            card.value = Ace;
            break;
        case '2':
            card.value = Two;
            break;
        case '3':
            card.value = Three;
            break;
        case '4':
            card.value = Four;
            break;
        case '5':
            card.value = Five;
            break;
        case '6':
            card.value = Six;
            break;
        case '7':
            card.value = Seven;
            break;
        case '8':
            card.value = Eight;
            break;
        case '9':
            card.value = Nine;
            break;
        case 'T':
            card.value = Ten;
            break;
        case 'J':
            card.value = Jack;
            break;
        case 'Q':
            card.value = Queen;
            break;
        case 'K':
            card.value = King;
            break;
        default:
            card.value = (CardValue) NULL;
            return card;
    }

    switch (toupper(lineFromFile[1])) {
        case 'C':
            card.suit = Clubs;
            break;
        case 'H':
            card.suit = Hearts;
            break;
        case 'D':
            card.suit = Diamonds;
            break;
        case 'S':
            card.suit = Spades;
            break;
        default:
            card.suit = (CardValue) NULL;
            return card;

    }
    return card;
}
