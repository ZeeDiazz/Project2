#pragma once
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
} Card;


/**
 * struct named Deck that contains a card-array of 52 cards
 */
typedef struct {
    Card cards[52];
} Deck;


/**
 * 
 * @return Returns unshuffled deck 
 */
Deck makeDeck();

char* cardToString(Card card);
