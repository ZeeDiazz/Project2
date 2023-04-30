#include "moveValidation.h"



bool canMoveToColumn(Card moving, LinkedList* movingTo) {
    // If the column is empty only a king can be placed
    if (movingTo->size == 0) {
        return moving.value == King;
    }

    Card bottomCard = getCardAt(movingTo, movingTo->size - 1);
    if ((bottomCard.value - 1) == moving.value) {
        return bottomCard.suit != moving.suit;
    }
    return false;
}

bool canMoveToFoundation(Card moving, LinkedList* movingTo) {
    // If the foundation is empty, only an ace can be placed
    if (movingTo->size == 0) {
        return moving.value == Ace;
    }

    Card topCard = getCardAt(movingTo, movingTo->size - 1);
    if((moving.value - 1) == topCard.value){
        return topCard.suit == moving.suit;
    }
    return false;
}

