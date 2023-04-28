#include "game.h"

bool canUseCommand(Game game, Command command) {
    if (command.name == QQ) {
        return true;
    }

    switch (game.phase)
    {
        case STARTUP:
            switch (command.name)
            {
                case LD:
                case SW:
                case SI:
                case SR:
                case SD:
                case P:
                    return true;
                default:
                    return false;
            }
            break;
        case PLAYING:
            switch (command.name)
            {
                case Q:
                case MOVE:
                    return true;
                default:
                    return false;
            }
            break;
        default:
            return false;
    }
    return false; // should never reach
}

void useCommand(Game* game, Command command) {
    switch (command.name)
    {
        case QQ:
            game->phase = QUITTING;
            break;
        case MOVE:
            game->totalMoves++;
            game->currentMove++;
            addMove(game->moves, command.arguments);
            break;
        case Q:
            game->phase = STARTUP;
            break;
        case P:
            game->phase = PLAYING;
            break;
        default:
            break;
    }
}
