typedef enum {
    Clubs = 1,
    Diamonds,
    Spades,
    Hearts,
} CardSuit;

typedef enum {
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

typedef struct {
    CardSuit suit;
    CardValue value;
} Card;

typedef struct {
    Card* cards[52];
} Deck;


Deck makeDeck();




