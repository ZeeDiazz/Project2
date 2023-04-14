#include "cli.h"
#include "stdlib.h"

const int spaceBetweenColumns = 2;
const char* unknownCard = "[]";

void printBoard(Command previousCommand) {
    int width = 2*7 + spaceBetweenColumns*6;

    char* firstLine = malloc(width * 10 + 9);
    int currentIndex = 0;

    for (int i = 1; i < 8; i++) {
        firstLine[currentIndex++] = 'C';
        firstLine[currentIndex++] = i + '0';

        for (int j = 0; j < spaceBetweenColumns; j++) {
            firstLine[currentIndex++] = ' ';
        }
    }
    firstLine[currentIndex++] = '\n';
    for (int i = 0; i < width; i++) {
        firstLine[currentIndex++] = ' ';
    }
    
    for (int a = 0; a < 7; a++) {
        firstLine[currentIndex++] = '\n';
        for (int i = 1; i < 8; i++) {
            firstLine[currentIndex++] = '[';
            firstLine[currentIndex++] = ']';

            for (int j = 0; j < spaceBetweenColumns; j++) {
                firstLine[currentIndex++] = ' ';
            }
        }
    }

    printf("%s\n", firstLine);
}