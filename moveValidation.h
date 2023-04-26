#pragma once
#include <stdbool.h>
#include "card.h"

bool canMoveToColumn(Card moving, Card movingTo);
bool canMoveToFoundation(Card moving, Card movingTo);
