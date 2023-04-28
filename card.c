// @author Zigalow

#include <stdio.h>
#include <stdlib.h>
#include "card.h"
/**
 * 
 * @return Returns unshuffled deck
 */
Deck makeDeck() {
    Card cards[52];
    int currentIndex = 0;

    for (CardSuit suit = Clubs; suit <= Spades; suit++) {
        for (CardValue cardValue = 1; cardValue <= 13; cardValue++) {
            Card currentCard = {suit, cardValue};
            cards[currentIndex] = currentCard;
            currentIndex++;
        }
    }

    Deck *deck = {cards};
    return *deck;
}

char* cardToString(Card card) {
    char charSuits[4] = "CDHS";
    char charValues[13] = "A23456789TJQK";

    if (card.suit < 1 || card.suit > 4 || card.value < 1 || card.value > 13) {
        return "";
    }

    char* string = malloc(sizeof(char) * 3);
    string[0] = charValues[card.value - 1];
    string[1] = charSuits[card.suit - 1];
    string[2] = '\0';
    return string;
}

