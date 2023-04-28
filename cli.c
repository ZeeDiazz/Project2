#include <string.h>
#include <stdio.h>
#include "cli.h"
#include "commands.h"

char* top = "C1\tC2\tC3\tC4\tC5\tC6\tC7\n\n";
char* commandText = "LAST Command:";
char* messageText = "Message:";
char* inputText = "INPUT >";

int columnWidth = 2 * 7 + 6;
int foundationWidth = 2 + 1 + 2;
void printBoard(CardList** columns, CardList** foundations, Command previousCommand) {
    int rows = 7; // minimum 7
    for (int i = 0; i < 7; i++) {
        if (columns[i]->length > rows) {
            rows = columns[i]->length;
        }
    }
    int columnsLength = rows * (columnWidth + 1);
    int foundationsLength = 4 * (2 + foundationWidth);

    char* commandString;
    char* message;

    if (previousCommand.name == NONE) {
        commandString = "";
        message = "";
    }
    else {
        commandString = commandToString(previousCommand);
        if (previousCommand.name == INVALID) {
            message = previousCommand.arguments;
        }
        else {
            message = "OK";
        }
    }

    int commandLength = strlen(commandText) + strlen(commandString) + 1;
    int messageLength = strlen(messageText) + strlen(message) + 1;
    int inputLength = strlen(inputText) + 1;

    int totalLength = strlen(top) + columnsLength + foundationsLength + 1 + commandLength + messageLength + inputLength;

    char* board = malloc(totalLength);

    int index = 0;
    for (int i = 0; i < strlen(top); i++) {
        board[index++] = top[i];
    }
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < 7; column++) {
            CardList* currentList = columns[column];
            char* cardString;
            if (row < currentList->length) {
                Card card = getAt(currentList, row);
                cardString = (card.seen) ? cardToString(getAt(currentList, row)) : "[]";
            }
            else {
                // Empty card
                cardString = "  ";
            }
            board[index++] = cardString[0];
            board[index++] = cardString[1];
    
            if (column != 6) {
                board[index++] = '\t';
            }
        }
        if (row == 0 || row == 2 || row == 4 || row == 6) {
            board[index++] = '\t';
            board[index++] = '\t';
            int foundationIndex = row / 2;
            CardList* foundation = foundations[foundationIndex];
            if (foundation->length == 0) {
                board[index++] = '[';
                board[index++] = ']';
            }
            else {
                Card topCard = getAt(foundation, foundation->length - 1);
                char* topCardString = cardToString(topCard);
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
    for (int i = 0; i < strlen(commandString); i++) {
        board[index++] = commandString[i];
    }
    board[index++] = '\n';
    for (int i = 0; i < strlen(messageText); i++) {
        board[index++] = messageText[i];
    }
    for (int i = 0; i < strlen(message); i++) {
        board[index++] = message[i];
    }
    board[index++] = '\n';
    for (int i = 0; i < strlen(inputText); i++) {
        board[index++] = inputText[i];
    }
    board[index++] = '\0';
    
    printf("%s", board);
}