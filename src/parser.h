#include "token.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct Parser {
    Token *tokens;
    int current;
} Parser;

extern void parse_ast(Token *tokens);

#endif