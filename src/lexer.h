#include "token.h"

#ifndef LEXER_H
#define LEXER_H

typedef struct Lexer {
    char *source;
    Token *tokens;
    int token_count;
    int token_capacity;
    int current;
} Lexer;

extern Token *tokenize(char *source);

#endif