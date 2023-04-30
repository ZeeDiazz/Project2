#include "moveValidation.h"



MoveError canMoveToColumn(Card moving, LinkedList* movingTo) {
    // If the column is empty only a king can be placed
    if (movingTo->size == 0) {
        return (moving.value == King) ? NONE : EMPTY_COLUMN;
    }

    Card bottomCard = getCardAt(movingTo, movingTo->size - 1);
    if ((bottomCard.value - 1) == moving.value) {
        return (bottomCard.suit != moving.suit) ? NONE : SAME_SUIT;
    }
    return COLUMN_ERROR;
}

MoveError canMoveToFoundation(Card moving, LinkedList* movingTo) {
    // If the foundation is empty, only an ace can be placed
    if (movingTo->size == 0) {
        return (moving.value == Ace) ? NONE : EMPTY_FOUNDATION;
    }

    Card topCard = getCardAt(movingTo, movingTo->size - 1);
    if (topCard.suit == moving.suit) {
        return ((moving.value - 1) == topCard.value) ? NONE : FOUNDATION_ERROR;
    }
    return DIFFERENT_SUIT;
}

