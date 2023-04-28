// @author Zigalow

#pragma once
#include "card.h"
/**
 * @author Zigalow
 */
typedef enum StatusCode {
    SUCCESS = 1,
    DUPLICATE,
    WRONGCARDFORMAT,
    FILENOTFOUND,
    MISSINGCARDS
} StatusCode;


/**
 * FileAssessment is the struct that's going to be returned from readFile. 
 * It contains different kinds of information 
 */

typedef struct FileAssessment {
    StatusCode statusCode;
    char *errorMessage;
    Deck deck;
} FileAssessment;

/**
 * This methods assigns a card in regards to what is read from a line
 * @param lineFromFile The line with information in regards to what value and suit the cards should hold
 * @return Returns the assigned card
 */
Card assignCard(char *lineFromFile);

/**
 * 
 * @param filename The filename of the file that needs to be read
 * @return Returns the FileAssessment of the file read
 */
FileAssessment readFromFile(char *filename);
