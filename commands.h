#pragma once
#include "stdbool.h"

typedef enum CommandName {
    UNKNOWN = 0,
    LD = 1,
    SW,
    SI,
    SR,
    SD,
    QQ,
    P,
    Q,
    MOVE,
} CommandName;

typedef enum CommandError {
    NO_ERROR = 0,
    WRONG_TIME,
    MALFORMED,
    TOO_MANY_ARGUMENTS,
    TOO_FEW_ARGUMENTS,
} CommandError;

typedef struct Command {
    CommandName name;
    CommandError error;
    bool hasArguments;
    char* arguments;
} Command;

bool commandCanTakeArguments(CommandName name);
Command parseCommand(char* command);
Command makeGameMoveCommand(char* potentialMove);
char* commandToString(Command command);