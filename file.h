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
    Card* deck;
} FileAssessment;

/**
 * 
 * @param filename The filename of the file that needs to be read
 * @return Returns the FileAssessment of the file read
 */
FileAssessment readFromFile(char *filename);
