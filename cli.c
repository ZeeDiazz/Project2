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
void printBoard(LinkedList** columns, LinkedList** foundations, Command previousCommand, char* lastInput, char* message) {
    int rows = 7; // minimum 7
    for (int i = 0; i < 7; i++) {
        if (columns[i]->size > rows) {
            rows = columns[i]->size;
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

    char* board = malloc(totalLength * sizeof(char));

    int index = 0;
    for (int i = 0; i < strlen(top); i++) {
        board[index++] = top[i];
    }
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < 7; column++) {
            LinkedList* currentList = columns[column];
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
            board[index++] = cardString[0];
            board[index++] = cardString[1];

            free(cardString);
    
            if (column != 6) {
                board[index++] = '\t';
            }
        }
        if (row == 0 || row == 2 || row == 4 || row == 6) {
            board[index++] = '\t';
            board[index++] = '\t';
            int foundationIndex = row / 2;
            LinkedList* foundation = foundations[foundationIndex];
            if (foundation->size == 0) {
                board[index++] = '[';
                board[index++] = ']';
            }
            else {
                Card topCard = getCardAt(foundation, foundation->size - 1);
                char* topCardString = cardToString(topCard, true);
                board[index++] = topCardString[0];
                board[index++] = topCardString[1];
            }
            board[index++] = '\t';
            board[index++] = 'F';
            board[index++] = '1' + foundationIndex;
        }
        board[index++] = '\n';
    }
    board[index++] = '\n';
    for (int i = 0; i < strlen(commandText); i++) {
        board[index++] = commandText[i];
    }
    for (int i = 0; i < strlen(lastCommand); i++) {
        board[index++] = lastCommand[i];
    }
    board[index++] = '\n';
    for (int i = 0; i < strlen(messageText); i++) {
        board[index++] = messageText[i];
    }
    for (int i = 0; i < strlen(actualMessage); i++) {
        board[index++] = actualMessage[i];
    }
    board[index++] = '\n';
    for (int i = 0; i < strlen(inputText); i++) {
        board[index++] = inputText[i];
    }
    board[index++] = '\0';
    
    printf("%s", board);
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
