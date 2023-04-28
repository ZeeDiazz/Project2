#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commands.h"

#define COMMAND_COUNT 8

bool commandCanTakeArguments(CommandName name) {
    return (name == LD || name == SI || name == SD);
}

Command parseCommand(char* commandString) {
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
    CommandName commandName = -1;
    for (int i = 0; i < COMMAND_COUNT; i++) {
        char* checkingName = commandStrings[i];
        int checkingLength = strlen(checkingName);
        if (inputLength == checkingLength && strncmp(commandString, checkingName, checkingLength) == 0) {
            commandName = i + 1;
            break;
        }
    }

    // Check if it's a move command
    if (commandName == -1) {
        if (inputLength != 6 && inputLength != 9) {
            // Cannot be a move
            Command unknown = {UNKNOWN, false, true, "Unknown command"};
            return unknown;
        }
        return makeGameMoveCommand(commandString);
    }

    int argumentLength = 0;
    // If the command can take arguments
    if (commandCanTakeArguments(commandName)) {
        // Get the length of the arguments (-1 because of the space)
        int length = strlen(commandString) - strlen(commandStrings[commandName]) - 1;
        if (length > 0) {
            argumentLength = length;
        }
    }

    bool hasArguments = (argumentLength > 0);
    char* arguments = malloc(argumentLength);
    if (hasArguments) {
        int argumentOffset = strlen(commandString) - argumentLength;
        for (int i = 0; i < argumentLength; i++) {
            arguments[i] = commandString[i + argumentOffset];
        }
    }

    Command command = {commandName, true, hasArguments, arguments};
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
    int stringLength = strlen(commandName) + 1;

    if (command.hasArguments) {
        stringLength += 1 + strlen(command.arguments);
    }
    stringRepresentation = malloc(stringLength);

    int currentIndex = 0;
    for (int i = 0; i < strlen(commandName); i++) {
        stringRepresentation[currentIndex] = commandName[i];
        currentIndex++;
    }

    if (command.hasArguments) {
        stringRepresentation[currentIndex] = ' ';
        currentIndex++;
        for (int i = 0; i < strlen(command.arguments); i++) {
            stringRepresentation[currentIndex] = command.arguments[i];
            currentIndex++;
        }    
    }
    stringRepresentation[stringLength - 1] = '\0';
    return stringRepresentation;
}