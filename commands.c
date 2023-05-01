#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"
#include "card.h"

bool commandCanTakeArguments(CommandName name) {
    return (name == LD || name == SI || name == SD);
}

Command parseCommand(char* commandString) {
    printf("Command string: %s\n", commandString);
    Command command = {UNKNOWN, NO_ERROR, false, NULL};

    char* commandStrings[COMMAND_COUNT];
    commandStrings[LD] = "LD";
    commandStrings[SW] = "SW";
    commandStrings[SI] = "SI";
    commandStrings[SR] = "SR";
    commandStrings[SD] = "SD";
    commandStrings[QQ] = "QQ";
    commandStrings[P] = "P";
    commandStrings[Q] = "Q";
    commandStrings[AUTO] = "AUTO";
    commandStrings[RESTART] = "RESTART";

    int inputLength = strlen(commandString);
    // has to be count - 1, because move is not identified by it's name
    for (int i = 0; i < COMMAND_COUNT - 1; i++) {
        char* checkingName = commandStrings[i];
        if (checkingName == NULL) {
            continue;
        }
        int checkingLength = strlen(checkingName);
        if (strncmp(commandString, checkingName, checkingLength) == 0) {
            command.name = (CommandName)i;
            break;
        }
    }

    // Check if it's a move command
    if (command.name == UNKNOWN) {
        if (inputLength != 6 && inputLength != 9) {
            // Cannot be a move
            command.error = MALFORMED;
            return command;
        }
        return makeGameMoveCommand(commandString);
    }

    int commandNameLength = strlen(commandStrings[command.name]);
    int argumentLength = inputLength - commandNameLength - 1;
    if (argumentLength > 0) {
        // Command is not valid if it cannot take arguments, and the command is longer than the name
        if (commandCanTakeArguments(command.name)) {
            command.hasArguments = true;
        }
        else if (command.error == NO_ERROR) {
            command.error = TOO_MANY_ARGUMENTS;
        }
    }
    else {
        argumentLength = 0;
    }

    if (command.hasArguments && command.error == NO_ERROR) {
        command.arguments = malloc(argumentLength);
        if (command.hasArguments) {
            int argumentOffset = strlen(commandString) - argumentLength;
            for (int i = 0; i < argumentLength; i++) {
                command.arguments[i] = commandString[i + argumentOffset];
            }
        }
    }
    else {
        command.arguments = NULL;
    }

    return command;
}


Command makeGameMoveCommand(char* potentialMove) {
    printf("Making move command\n");
    Command command = {MOVE, NO_ERROR, true, potentialMove};

    int expectedArrowIndex;
    // Check if the move is from a column
    if (potentialMove[0] == 'C') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '7' > 0);
        if (tooLow || tooHigh) {
            command.error = MALFORMED;
            return command;
        }

        if (potentialMove[2] == ':') {
            char* card = malloc(3);
            card[0] = potentialMove[3];
            card[1] = potentialMove[4];
            card[2] = '\0';
            Card potentialCard = stringToCard(card);
            free(card);
            // TODO don't make null check
            if (potentialCard.suit == NULL || potentialCard.value == NULL) {
                command.error = MALFORMED;
            }
            expectedArrowIndex = 5;
        }
        else {
            expectedArrowIndex = 2;
        }
    }
    // Check if move is from a foundation
    else if (potentialMove[0] == 'F') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '4' > 0);
        if (tooLow || tooHigh) {
            command.error = MALFORMED;
            return command;
        }
        expectedArrowIndex = 2;
    }
    // If it's not a column or a foundation
    else {
        command.error = MALFORMED;
        return command;
    }

    // Check if the arrow is at the expected location
    if (potentialMove[expectedArrowIndex] != '-' || potentialMove[expectedArrowIndex + 1] != '>') {
        command.error = MALFORMED;
        return command;
    }

    int movingToIndex = expectedArrowIndex + 2;
    if (potentialMove[movingToIndex] == 'C') {
        bool tooLow = (potentialMove[movingToIndex + 1] - '1' < 0);
        bool tooHigh = (potentialMove[movingToIndex + 1] - '7' > 0);
        if (tooLow || tooHigh) {
            command.error = MALFORMED;
            return command;
        }
    }
    else if (potentialMove[movingToIndex] == 'F') {
        bool tooLow = (potentialMove[movingToIndex + 1] - '1' < 0);
        bool tooHigh = (potentialMove[movingToIndex + 1] - '4' > 0);
        if (tooLow || tooHigh) {
            command.error = MALFORMED;
            return command;
        }
    }
    else {
        command.error = MALFORMED;
        return command;
    }
    
    printf("Correctly handling parsing\n");
    return command;
}

char* commandToString(Command command) {
    if (command.name == MOVE) {
        return (command.error == NO_ERROR) ? command.arguments : "MOVE";
    }

    if (command.name > COMMAND_COUNT) {
        return "";
    }

    char* commandStrings[COMMAND_COUNT];
    commandStrings[UNKNOWN] = "???";
    commandStrings[LD] = "LD";
    commandStrings[SW] = "SW";
    commandStrings[SI] = "SI";
    commandStrings[SR] = "SR";
    commandStrings[SD] = "SD";
    commandStrings[QQ] = "QQ";
    commandStrings[P] = "P";
    commandStrings[Q] = "Q";
    commandStrings[AUTO] = "AUTO";
    commandStrings[RESTART] = "RESTART";
    commandStrings[MOVE] = "MOVE";

    char* commandName = commandStrings[command.name];
    char* stringRepresentation;
    int stringLength = strlen(commandName);

    if (command.hasArguments && command.error == NO_ERROR) {
        stringLength += 1 + strlen(command.arguments);
    }
    stringRepresentation = malloc(stringLength + 1);

    int currentIndex = 0;
    for (int i = 0; i < strlen(commandName); i++) {
        stringRepresentation[currentIndex] = commandName[i];
        currentIndex++;
    }

    if (command.hasArguments && command.error == NO_ERROR) {
        stringRepresentation[currentIndex] = ' ';
        currentIndex++;
        for (int i = 0; i < strlen(command.arguments); i++) {
            stringRepresentation[currentIndex] = command.arguments[i];
            currentIndex++;
        }    
    }
    stringRepresentation[stringLength] = '\0';
    return stringRepresentation;
}