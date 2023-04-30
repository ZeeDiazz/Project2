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
    char* string;
    int stringLength;

    if (card.suit < 1 || card.suit > 4 || card.value < 1 || card.value > 13) {
        stringLength = 0;
        // save space for NULL
        string = malloc(1);
    }
    else {
        stringLength = 2;
        string = malloc(2 + 1);
        if (card.seen) {
            switch (card.value)
            {
                case Ace:
                    string[0] = 'A';
                    break;
                case Ten:
                    string[0] = 'T';
                    break;
                case Jack:
                    string[0] = 'J';
                    break;
                case Queen:
                    string[0] = 'Q';
                    break;
                case King:
                    string[0] = 'K';
                    break;
                default:
                    string[0] = card.value + '0';
                    break;
            }
            switch (card.suit)
            {
                case Clubs:
                    string[1] = 'C';
                    break;
                case Diamonds:
                    string[1] = 'D';
                    break;
                case Hearts:
                    string[1] = 'H';
                    break;
                case Spades:
                    string[1] = 'S';
                    break;
                default:
                    string[1] = '?';
                    break;
            }
        }
        else {
            string[0] = '[';
            string[1] = ']';
        }
    }

    string[stringLength] = '\0';
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
