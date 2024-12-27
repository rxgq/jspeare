#ifndef TOKEN_H
#define TOKEN_H

typedef enum TokenType {
    LET,
    
    SINGLE_EQUALS,
    SEMI_COLON,

    IDENTIFIER,
    NUMBER
} TokenType;

typedef struct Token {
    TokenType type;
    char* lexeme;
} Token;

#endif
