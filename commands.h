#include "stdbool.h"

typedef enum CommandName {
    INVALID = 0,
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

typedef struct Command {
    CommandName name;
    bool hasArguments;
    char* arguments;
} Command;

bool commandCanTakeArguments(CommandName name);
Command parseCommand(char* command);
Command makeGameMoveCommand(char* potentialMove);
char* commandToString(Command command);