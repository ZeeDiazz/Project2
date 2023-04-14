#include "stdbool.h"

typedef enum {
    LD = 0,
    SW,
    SI,
    SR,
    SD,
    QQ,
    P,
    Q,
    Move,
} CommandName;

typedef struct {
    CommandName name;
    bool hasArguments;
    char* arguments;
} Command;

bool commandCanTakeArguments(CommandName name);
Command parseCommand(char* command);
