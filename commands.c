#include <stdlib.h>
#include <string.h>
#include "commands.h"

bool commandCanTakeArguments(CommandName name) {
    return (name == LD || name == SI || name == SD);
}

Command parseCommand(char* commandString) {
    const int commandCount = 8;
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
            commandName = i;
            break;
        }
    }

    // Check if it's a move command
    if (commandName == -1) {
        // TODO
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