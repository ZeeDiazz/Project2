// @author Zigalow

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "card.h"
/**
 * 
 * @return Returns unshuffled deck
 */
Card* makeDeck() {
    Card* cards = malloc(52 * sizeof(Card));
    int currentIndex = 0;

    for (CardSuit suit = Clubs; suit <= Spades; suit++) {
        for (CardValue cardValue = 1; cardValue <= 13; cardValue++) {
            Card currentCard = {suit, cardValue, false};
            cards[currentIndex] = currentCard;
            currentIndex++;
        }
    }

    return cards;
}

char* cardToString(Card card) {
    char charSuits[4] = "CHDS";
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

Card stringToCard(char* string) {
    Card card;

    switch (toupper(string[0])) {
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

    switch (toupper(string[1])) {
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
    card.seen = false;
    return card;
}
