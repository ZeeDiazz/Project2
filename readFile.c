#include <stdio.h>
#include "readFile.h"

int readFromFile(char *filename) {
    const int bufferSize = 156;

    FILE *pFile = fopen("cards.txt", "r");
    char buffer[bufferSize];

    if(pFile == NULL){
        return -1;
    }

    while (fgets(buffer, bufferSize, pFile) != NULL) {

    }
    fclose(pFile);



}

Deck loadDeck(char* filename){

}



