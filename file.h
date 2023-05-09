// @author Zigalow

#pragma once

#include "card.h"
#include "moveStack.h"
#include "game.h"

/**
 * @author Zigalow
 */
typedef enum StatusCode {
    SUCCESS = 1,
    DUPLICATE,
    WRONGCARDFORMAT,
    FILENOTFOUND,
    MISSINGCARDS,
    ERRORINMOVESTACK
} StatusCode;


/**
 * FileAssessment is the struct that's going to be returned from readFile. 
 * It contains different kinds of information 
 */

typedef struct FileAssessment {
    StatusCode statusCode;
    char *errorMessage;
    Card *deck;
} FileAssessment;

typedef struct LoadInfo {
    StatusCode statusCode;
    char *errorMessage;
} LoadInfo;

/**
 * 
 * @param filename The filename of the file that needs to be read
 * @return Returns the FileAssessment of the file read
 */
FileAssessment readDeckFromFile(char *filename);

void saveDeckToFile(char *filename, Card *cards);

void saveGame(char *filename, GameState gameState);

LoadInfo loadFromFile(char *filename, GameState* gameState);
