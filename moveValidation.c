#include "moveValidation.h"



bool canMoveToColumn(Card moving, Card movingTo) {
    if ((movingTo.value - 1) == moving.value) {
        return movingTo.suit != moving.suit;
    }
    return false;
}

bool canMoveToFoundation(Card moving, Card movingTo) {
    if((moving.value - 1) == movingTo.value){
        return movingTo.suit == moving.suit;
    }
    return false;
}

