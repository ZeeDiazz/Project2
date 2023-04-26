#pragma once
#include "card.h"

typedef enum StatusCode {
    SUCCESS = 1,
    DUPLICATE,
    WRONGCARDFORMAT,
    FILENOTFOUND,
    MISSINGCARDS
} StatusCode;


typedef struct FileAssessment {
    StatusCode statusCode;
    char *errorMessage;
    Deck deck;
} FileAssessment;


Card assignCard(char *lineFromFile);

FileAssessment readFromFile(char *filename);