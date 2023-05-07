#include <string.h>
#include <stdio.h>
#include "cli.h"
#include "commands.h"

#define MAX_INPUT_LENGTH 100 + 1

char* top = "C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n";
char* commandText = "LAST Command:";
char* messageText = "Message:";
char* inputText = "INPUT >";

int columnWidth = 2 * 7 + 6;
int foundationWidth = 2 + 1 + 2;
void printBoard(Board board, Command previousCommand, char* lastInput, char* message) {
    int rows = 7; // minimum 7
    for (int i = 0; i < 7; i++) {
        if (board.columns[i]->size > rows) {
            rows = board.columns[i]->size;
        }
    }
    int columnsLength = rows * (columnWidth + 1);
    int foundationsLength = 4 * (2 + foundationWidth);

    char* actualMessage = (message == NULL) ? "" : message;
    char* lastCommand = (lastInput == NULL) ? "" : commandToString(previousCommand);

    int commandLength = strlen(commandText) + strlen(lastCommand) + 1;
    int messageLength = strlen(messageText) + strlen(actualMessage) + 1;
    int inputLength = strlen(inputText) + 1;

    int totalLength = strlen(top) + columnsLength + foundationsLength + 1 + commandLength + messageLength + inputLength;

    char* boardString = malloc(totalLength * sizeof(char));

    int index = 0;
    for (int i = 0; i < strlen(top); i++) {
        boardString[index++] = top[i];
    }
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < 7; column++) {
            LinkedList* currentList = board.columns[column];
            char* cardString;
            if (row < currentList->size) {
                Card card = getCardAt(currentList, row);
                cardString = cardToString(card, true);
            }
            else {
                // Empty card
                cardString = malloc(3);
                cardString[0] = ' ';
                cardString[1] = ' ';
                cardString[2] = '\0';
            }
            boardString[index++] = cardString[0];
            boardString[index++] = cardString[1];

            free(cardString);
    
            if (column != 6) {
                boardString[index++] = '\t';
            }
        }
        if (row == 0 || row == 2 || row == 4 || row == 6) {
            boardString[index++] = '\t';
            boardString[index++] = '\t';
            int foundationIndex = row / 2;
            LinkedList* foundation = board.foundations[foundationIndex];
            if (foundation->size == 0) {
                boardString[index++] = '[';
                boardString[index++] = ']';
            }
            else {
                Card topCard = getCardAt(foundation, foundation->size - 1);
                char* topCardString = cardToString(topCard, true);
                boardString[index++] = topCardString[0];
                boardString[index++] = topCardString[1];
            }
            boardString[index++] = '\t';
            boardString[index++] = 'F';
            boardString[index++] = '1' + foundationIndex;
        }
        boardString[index++] = '\n';
    }
    boardString[index++] = '\n';
    for (int i = 0; i < strlen(commandText); i++) {
        boardString[index++] = commandText[i];
    }
    for (int i = 0; i < strlen(lastCommand); i++) {
        boardString[index++] = lastCommand[i];
    }
    boardString[index++] = '\n';
    for (int i = 0; i < strlen(messageText); i++) {
        boardString[index++] = messageText[i];
    }
    for (int i = 0; i < strlen(actualMessage); i++) {
        boardString[index++] = actualMessage[i];
    }
    boardString[index++] = '\n';
    for (int i = 0; i < strlen(inputText); i++) {
        boardString[index++] = inputText[i];
    }
    boardString[index++] = '\0';
    
    printf("%s", boardString);
}

char* getUserInput() {
    char* inputBuffer = malloc(MAX_INPUT_LENGTH * sizeof(char));

    int readCount = 0;
    int length;
    do {
        fgets(inputBuffer, MAX_INPUT_LENGTH, stdin);
        length = strlen(inputBuffer);
        readCount++;
    } while (length == (MAX_INPUT_LENGTH - 1) && inputBuffer[MAX_INPUT_LENGTH - 2] != '\n');

    if (readCount != 1) {
        return NULL;
    }

    // We read the whole input in 1 go
    for (int i = 0; i < length; i++) {
        if (inputBuffer[i] == '\n') {
            inputBuffer[i] = '\0';
            break;
        }
    }
    return inputBuffer;
}
