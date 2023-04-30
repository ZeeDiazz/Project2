#pragma once
#include <stdbool.h>
#include "card.h"
#include "linkedList.h"

bool canMoveToColumn(Card moving, LinkedList* movingTo);
bool canMoveToFoundation(Card moving, LinkedList* movingTo);
