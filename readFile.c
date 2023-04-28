// @author Zigalow

#include <stdio.h>
#include <ctype.h>
#include "readFile.h"
#include <stdbool.h>

#define BUFFER_SIZE 128
/**
 * 
 * @param filename Filename of the file containing the cards that needs to be read
 * @return Returns FileAssessment, which is determined by different factors when reading from file
 */
 
FileAssessment readFromFile(char *filename) {

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
    Card cards[52]; // Deck that will be made
    bool checkCards[4][13] = {{0}}; // Card-array for checking for duplicates
    int cardCounter = 0;
    while (fgets(line, BUFFER_SIZE, pFile) != NULL && cardCounter < 52) {
        currentCard = assignCard(line);
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
    Deck *deck = {cards};
    assessment.deck = *deck;

    return assessment;
}

/**
 * 
 * @param lineFromFile The line read, with information in regards to what value and suit the cards should hold
 * @return Returns the assigned card
 */
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
