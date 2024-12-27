#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "lexer.h"

static Lexer *lexer_init(char *source) {
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->tokens = malloc(sizeof(Token) * 2);
    lexer->token_count = 0;
    lexer->token_capacity = 2;
    lexer->current = 0;

    return lexer;
}

static inline void advance(Lexer *lexer) {
    lexer->current++;
}

static int is_end(Lexer *lexer) {
    if (lexer->source[lexer->current] == '\0') {
        return 1;
    }

    return 0;
}

static void add_token(Lexer *lexer, Token *token) {
    lexer->token_count++;

    if (lexer->token_count >= lexer->token_capacity) {
        lexer->token_capacity *= 2;
        lexer->tokens = (Token *)realloc(lexer->tokens, lexer->token_capacity * sizeof(Token));
    }

    lexer->tokens[lexer->token_count - 1] = *token;
}

static void create_token(Lexer *lexer, TokenType type, char *lexeme) {
    Token *token = (Token *)malloc(sizeof(Token));

    int len = strlen(lexeme);
    token->lexeme = (char *)malloc(len + 1);
    strncpy(token->lexeme, lexeme, len);
    token->lexeme[len] = '\0';

    token->type = type;

    add_token(lexer, token);
}

static inline void parse_token(Lexer *lexer) {
    int start = lexer->current;

    while (!is_end(lexer) && lexer->source[lexer->current] != ' ') {
        advance(lexer);
    }

    int len = (lexer->current - start);

    char *lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, lexer->source + start, len);
    
    lexeme[len] = '\0';

    create_token(lexer, LET, lexeme);
}

Token *tokenize(char *source) {
    Lexer *lexer = lexer_init(source);

    while (lexer->source[lexer->current] != '\0') {

        parse_token(lexer);
        if (!is_end(lexer)) advance(lexer);
    }

    for (int i = 0; i < lexer->token_count; i++) {
        printf("\nToken: %s | %d", lexer->tokens[i].lexeme, lexer->tokens[i].type);
    }

    return lexer->tokens;
}