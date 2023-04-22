#include <stdio.h>
#include <ctype.h>
#include "readFile.h"


int readFromFile(char *filename) {
    const int bufferSize = 128;

    // Doesn't work with filename alone. Insert absolute path
    FILE *pFile = fopen("cards.txt", "r");
    char line[bufferSize];

    // Fails if provided with the wrong filename
    if (pFile == NULL) {
        return -53;
    }


    Card currentCard;
    Card cards[52]; // Deck that will be made
    int checkCards[4][13] = {{0}}; // For checking if deck contains all cards
    int i = 0;
    while (fgets(line, bufferSize, pFile) != NULL && i < 52) {
        currentCard = assignCard(line);
        if (currentCard.value == NULL || currentCard.suit == NULL) {
            return -i - 1;
        }
        cards[i] = currentCard;
        i++;
        checkCards[currentCard.suit - 1][currentCard.value - 1] = 1;
    }
    fclose(pFile);


    // Checking if deck contains all the cards
    for (int j = 0; j < 4; ++j) {
        for (int k = 0; k < 13; ++k) {
            if (checkCards[j][k] != 1) {
                return -54;
            }
        }
    }

    Deck *deck1 = {cards};

    return 1;
}

// EXIT CODES ------------------------------------------------------------------------------------------
// 1 = A full deck can be made by reading from the file

// -52 < exit code < 0 = Represents the line number (absolute value) at which there was an error
// Will produce this exit code if the first to characters on a line aren't a correct card representation
// Will still work if there is written something after the first two characters on a line

// -53 = Wrong filepath/Couldn't load the filename

// -54 The deck contains duplicates or are missing some cards 
// It could also mean that the first 52 lines didn't contain the card information in the correct format
// The two first characters on a line should represent only one card with the correct representation
// All other formats will produce this exit code




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
    }
    return card;
}








