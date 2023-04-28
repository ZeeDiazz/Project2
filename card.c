// @author Zigalow

#include <stdio.h>
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

