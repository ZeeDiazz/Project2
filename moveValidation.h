#pragma once
#include "card.h"
#include "linkedList.h"

typedef enum MoveInfo {
    NONE = 0,
    SHOWED_CARD,
    SAME_SUIT,
    SAME_COLOR, // not used currently
    DIFFERENT_SUIT,
    EMPTY_COLUMN,
    EMPTY_FOUNDATION,
    COLUMN_ERROR,
    FOUNDATION_ERROR,

    NO_CARDS,
    NO_MATCHES,
    ONLY_ONE_CARD_TO_FOUNDATION,
    NO_EFFECT,
} MoveInfo;

MoveInfo canMoveToColumn(Card moving, LinkedList* movingTo);
MoveInfo canMoveToFoundation(Card moving, LinkedList* movingTo);
