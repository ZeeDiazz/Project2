#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"

#define COMMAND_COUNT 8

bool commandCanTakeArguments(CommandName name) {
    return (name == LD || name == SI || name == SD);
}

Command parseCommand(char* commandString) {
    Command command = {UNKNOWN, true, false, NULL};

    char* commandStrings[COMMAND_COUNT];
    commandStrings[0] = "LD";
    commandStrings[1] = "SW";
    commandStrings[2] = "SI";
    commandStrings[3] = "SR";
    commandStrings[4] = "SD";
    commandStrings[5] = "QQ";
    commandStrings[6] = "P";
    commandStrings[7] = "Q";

    int inputLength = strlen(commandString);
    for (int i = 0; i < COMMAND_COUNT; i++) {
        char* checkingName = commandStrings[i];
        int checkingLength = strlen(checkingName);
        if (strncmp(commandString, checkingName, checkingLength) == 0) {
            command.name = (CommandName)i+1;
            // Command is not valid if it cannot take arguments, and the command is longer than the name
            command.isValid = (commandCanTakeArguments(command.name) || inputLength == checkingLength) && command.isValid;
            break;
        }
    }

    // Check if it's a move command
    if (command.name == UNKNOWN) {
        if (inputLength != 6 && inputLength != 9) {
            // Cannot be a move
            command.isValid = false;
            return command;
        }
        return makeGameMoveCommand(commandString);
    }

    int commandNameLength = strlen(commandStrings[command.name - 1]);
    int argumentLength = inputLength - commandNameLength - 1;
    if (argumentLength > 0) {
        command.hasArguments = true;
        command.isValid = (commandString[commandNameLength] == ' ') && command.isValid;
    }
    else {
        argumentLength = 0;
    }

    if (command.hasArguments && command.isValid) {
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


#define INVALID_COLUMN "Invalid column"
#define INVALID_FOUNDATION "Invalid foundation"
#define MALFORMED_MOVE_COMMAND "Malformed command (has to be \"to->from\")"
Command makeGameMoveCommand(char* potentialMove) {
    Command command = {MOVE, true, true, ""};

    int expectedArrowIndex;
    // Check if the move is from a column
    if (potentialMove[0] == 'C') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '7' > 0);
        if (tooLow || tooHigh) {
            command.isValid = false;
            command.arguments = INVALID_COLUMN;
            return command;
        }

        if (potentialMove[2] == ':') {
            if (potentialMove[3] == 'C') {
                tooLow = (potentialMove[1] - '1' < 0);
                tooHigh = (potentialMove[1] - '7' > 0);
                if (tooLow || tooHigh) {
                    command.isValid = false;
                    command.arguments = INVALID_COLUMN;
                    return command;
                }
            }
            else if (potentialMove[3] == 'F') {
                tooLow = (potentialMove[1] - '1' < 0);
                tooHigh = (potentialMove[1] - '4' > 0);
                if (tooLow || tooHigh) {
                    command.isValid = false;
                    command.arguments = INVALID_FOUNDATION;
                    return command;
                }
            }
            else {
                command.isValid = false;
                command.arguments = MALFORMED_MOVE_COMMAND;
                return command;
            }

            expectedArrowIndex = 5;
        }
        else {
            expectedArrowIndex = 3;
        }
    }
    // Check if move is from a foundation
    else if (potentialMove[0] == 'F') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '4' > 0);
        if (tooLow || tooHigh) {
            command.isValid = false;
            command.arguments = INVALID_COLUMN;
            return command;
        }
        expectedArrowIndex = 3;
    }
    // If it's not a column or a foundation
    else {
        command.isValid = false;
        command.arguments = MALFORMED_MOVE_COMMAND;
        return command;
    }

    // Check if the arrow is at the expected location
    if (potentialMove[expectedArrowIndex] != '-' || potentialMove[expectedArrowIndex + 1] != '>') {
        command.isValid = false;
        command.arguments = MALFORMED_MOVE_COMMAND;
        return command;
    }

    if (potentialMove[3] == 'C') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '7' > 0);
        if (tooLow || tooHigh) {
            command.isValid = false;
            command.arguments = INVALID_COLUMN;
            return command;
        }
    }
    else if (potentialMove[3] == 'F') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '4' > 0);
        if (tooLow || tooHigh) {
            command.isValid = false;
            command.arguments = INVALID_FOUNDATION;
            return command;
        }
    }
    else {
        command.isValid = false;
        command.arguments = MALFORMED_MOVE_COMMAND;
        return command;
    }
    
    command.arguments = potentialMove;
    return command;
}

char* commandToString(Command command) {
    if (command.name == MOVE) {
        return command.arguments;
    }

    if (command.name > COMMAND_COUNT) {
        return "";
    }

    char* commandStrings[COMMAND_COUNT + 1];
    commandStrings[0] = "???";
    commandStrings[1] = "LD";
    commandStrings[2] = "SW";
    commandStrings[3] = "SI";
    commandStrings[4] = "SR";
    commandStrings[5] = "SD";
    commandStrings[6] = "QQ";
    commandStrings[7] = "P";
    commandStrings[8] = "Q";

    char* commandName = commandStrings[command.name];
    char* stringRepresentation;
    int stringLength = strlen(commandName);

    if (command.hasArguments && command.isValid) {
        stringLength += 1 + strlen(command.arguments);
    }
    stringRepresentation = malloc(stringLength + 1);

    int currentIndex = 0;
    for (int i = 0; i < strlen(commandName); i++) {
        stringRepresentation[currentIndex] = commandName[i];
        currentIndex++;
    }

    if (command.hasArguments && command.isValid) {
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