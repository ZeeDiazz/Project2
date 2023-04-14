#include <stdlib.h>
#include <string.h>
#include "commands.h"


const int commandCount = 8;
bool commandCanTakeArguments(CommandName name) {
    return (name == LD || name == SI || name == SD);
}

Command parseCommand(char* commandString) {
    char* commandStrings[commandCount];
    commandStrings[0] = "LD";
    commandStrings[1] = "SW";
    commandStrings[2] = "SI";
    commandStrings[3] = "SR";
    commandStrings[4] = "SD";
    commandStrings[5] = "QQ";
    commandStrings[6] = "P";
    commandStrings[7] = "Q";

    CommandName commandName = -1;
    for (int i = 0; i < commandCount; i++) {
        char* checkingName = commandStrings[i];
        if (strncmp(commandString, checkingName, strlen(checkingName)) == 0) {
            commandName = i + 1;
            break;
        }
    }

    // Check if it's a move command
    if (commandName == -1) {
        // TODO check?
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

    Command command = {commandName, hasArguments, arguments};
    return command;
}

Command makeGameMoveCommand(char* potentialMove) {
    Command invalidColumn = {INVALID, true, "Invalid column"};
    Command invalidFoundation = {INVALID, true, "Invalid foundation"};
    Command malformedCommand = {INVALID, true, "Malformed command (has to be \"to->from\")"};

    int expectedArrowIndex;
    // Check if the move is from a column
    if (potentialMove[0] == 'C') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '7' > 0);
        if (tooLow || tooHigh) {
            return invalidColumn;
        }

        if (potentialMove[2] == ':') {
            if (potentialMove[3] == 'C') {
                tooLow = (potentialMove[1] - '1' < 0);
                tooHigh = (potentialMove[1] - '7' > 0);
                if (tooLow || tooHigh) {
                    return invalidColumn;
                }
            }
            else if (potentialMove[3] == 'F') {
                tooLow = (potentialMove[1] - '1' < 0);
                tooHigh = (potentialMove[1] - '4' > 0);
                if (tooLow || tooHigh) {
                    return invalidFoundation;
                }
            }
            else {
                return malformedCommand;
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
            return invalidColumn;
        }
        expectedArrowIndex = 3;
    }
    // If it's not a column or a foundation
    else {
        return malformedCommand;
    }

    // Check if the arrow is at the expected location
    if (potentialMove[expectedArrowIndex] != '-' || potentialMove[expectedArrowIndex + 1] != '>') {
        return malformedCommand;
    }

    if (potentialMove[3] == 'C') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '7' > 0);
        if (tooLow || tooHigh) {
            return invalidColumn;
        }
    }
    else if (potentialMove[3] == 'F') {
        bool tooLow = (potentialMove[1] - '1' < 0);
        bool tooHigh = (potentialMove[1] - '4' > 0);
        if (tooLow || tooHigh) {
            return invalidFoundation;
        }
    }
    else {
        return malformedCommand;
    }
    
    Command moveCommand = {MOVE, true, potentialMove};
    return moveCommand;
}

char* commandToString(Command command) {
    if (command.name == INVALID || command.name == MOVE) {
        return command.arguments;
    }

    if (command.name > commandCount) {
        return "";
    }

    char* commandStrings[commandCount];
    commandStrings[0] = "LD";
    commandStrings[1] = "SW";
    commandStrings[2] = "SI";
    commandStrings[3] = "SR";
    commandStrings[4] = "SD";
    commandStrings[5] = "QQ";
    commandStrings[6] = "P";
    commandStrings[7] = "Q";

    char* commandName = commandStrings[command.name];
    char* stringRepresentation;
    int stringLength = strlen(commandName);

    if (command.hasArguments) {
        stringLength += 1 + strlen(command.arguments);
    }
    stringRepresentation = malloc(commandName);

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
    return stringRepresentation;
}