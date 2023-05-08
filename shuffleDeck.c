#include <stdlib.h>
#include <stdio.h>
#include "shuffleDeck.h"
/** This method is for the command SI.
 * It shuffles the deck, by dividing the cards in two piles and mixing them together.
 * @author ZeeDiazz (Zaid)
 * @param list
 * @param splitIndex
 */
void shuffleInterleaved(LinkedList* list, int splitIndex){
    //If the split Index is smaller or greater than the size of the list
    if (splitIndex < 0 || splitIndex >= list->size) {
        return;
    }

    //Split the list in two piles
    LinkedList* secondPile = splitList(list,splitIndex);

    //Create an empty list to collect the shuffled deck
    LinkedList*  shuffledPile = makeEmptyList();

    //As long as list and secondPile have cards
    while (list->size > 0 && secondPile->size > 0) {
        //Add the first card from list to shuffledPile
        addCard(shuffledPile, getCardAt(list,0));
        //Remove the first card in list
        removeCard(list, getCardAt(list,0));

        //Add the first card from secondPile to shuffledPile
        addCard(shuffledPile, getCardAt(secondPile,0));
        //Remove the first card in secondPile
        removeCard(secondPile, getCardAt(secondPile,0));
    }

    // If the cards are unevenly divided in piles, then add the last card
    if (list->size > 0) {
        addList(shuffledPile, list);
    } else {
        addList(shuffledPile, secondPile);
    }
    //At this point list is empty, and we can now add shuffledPile
    addList(list,shuffledPile);
}

/** SR command
 * To shuffle the deck randomly
 * @author ZeeDiazz (Zaid)
 * @param list
 */
void shuffleRandom(LinkedList* list){

    //start with an empty list
    LinkedList * shuffledPile =makeEmptyList();

    while (list->size > 0) {
        //A random int from 0 to list.size
        int randomIndex;

        if (shuffledPile->size == 0) {
        randomIndex = 0;
        } else {
            randomIndex = rand() % shuffledPile->size;
        }
        //Add that card
        //addCard(shuffledPile, getCardAt(list,randomIndex));
        addCardIndex(shuffledPile, getCardAt(list,0),randomIndex);

        //remove the added card from list
        removeCard(list,getCardAt(list,0));
    }
    // add shuffledPile to an empty list
    addList(list, shuffledPile);
}