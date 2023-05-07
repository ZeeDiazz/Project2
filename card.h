#pragma once
#include <stdbool.h>
// @author Zigalow

typedef enum CardSuit {
    Clubs = 1,
    Hearts,
    Diamonds,
    Spades,
    
} CardSuit;

typedef enum CardValue {
    Ace = 1,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King,
} CardValue;

typedef struct Card {
    CardSuit suit;
    CardValue value;
    bool seen;
} Card;


/**
 * 
 * @return Returns unshuffled deck 
 */
Card* makeDeck();

char* cardToString(Card card, bool respectVisibility);
Card stringToCard(char* string);
