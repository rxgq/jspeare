#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "token.h"
#include "lexer.h"

#define INITIAL_TOKEN_CAPACITY 2

static Lexer *lexer_init(char *source) {
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->source = source;
    lexer->tokens = malloc(sizeof(Token) * INITIAL_TOKEN_CAPACITY);
    
    lexer->token_count = 0;
    lexer->token_capacity = INITIAL_TOKEN_CAPACITY;
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
        if (!lexer->tokens) {
            fprintf(stderr, "Memory allocation for tokens failed\n");
            exit(EXIT_FAILURE);
        }
    }

    lexer->tokens[lexer->token_count - 1] = *token;
}

static void create_token(Lexer *lexer, TokenType type, const char *lexeme) {
    Token token;
    token.type = type;

    token.lexeme = strdup(lexeme);
    if (!token.lexeme) {
        fprintf(stderr, "Memory allocation for lexeme failed\n");
        exit(EXIT_FAILURE);
    }

    add_token(lexer, &token);
}

static TokenMap triple_token_map[] = {
    {"...", TRIPLE_DOT},
    {"===", TRIPLE_EQUALS},
    {"!==", EXCLAMATION_DOUBLE_EQUALS},
    {">>>", UNSIGNED_RIGHT},
};

static TokenMap double_token_map[] = {
    {"++", DOUBLE_PLUS},
    {"--", DOUBLE_MINUS},
    {"**", DOUBLE_STAR},
    {"||", DOUBLE_PIPE},
    {"&&", DOUBLE_AMPERSAND},
    {"??", DOUBLE_QUESTION},
    {"==", DOUBLE_EQUALS},
    {"!=", EXCLAMATION_EQUALS},
    {">=", GREATER_THAN_EQUAL},
    {"<=", LESS_THAN_EQUAL},
    {"+=", PLUS_EQUALS},
    {"-=", MINUS_EQUALS},
    {"*=", STAR_EQUALS},
    {"/=", SLASH_EQUALS},
    {"%%=", MODULO_EQUALS},
    {"<<", LEFT_SHIFT},
    {">>", RIGHT_SHIFT},
};

static TokenMap single_token_map[] = {
    {"+", PLUS},
    {"-", MINUS},
    {"*", STAR},
    {"/", SLASH},
    {"%%", MODULO},
    {"!", EXCLAMATION},
    {".", DOT},
    {",", COMMA},
    {";", SEMI_COLON},
    {">", GREATER_THAN},
    {"<", LESS_THAN},
    {":", COLON},
    {"=", SINGLE_EQUALS},
    {"(", LEFT_PAREN},
    {")", RIGHT_PAREN},
    {"{", LEFT_BRACE},
    {"}", RIGHT_BRACE},
    {"[", LEFT_SQUARE_BRACKET},
    {"]", RIGHT_SQUARE_BRACKET},
    {"&", SINGLE_AMPERSAND},
    {"|", SINGLE_PIPE},
    {"~", TILDE},
    {"^", CARET},
};

static TokenMap keyword_token_map[] = {
    {"let", LET},
    {"var", VAR},
    {"const", CONST},
    {"typeof", TYPEOF},
    {"null", NULL_},
    {"true", TRUE},
    {"false", FALSE},
    {"NaN", NAN},
    {"new", NEW},
    {"delete", DELETE},
    {"in", IN},
    {"this", THIS},
    {"instanceof", INSTANCEOF},
    {"break", BREAK},
    {"debugger", DEBUGGER},
    {"export", EXPORT},
    {"finally", FINALLY},
    {"super", SUPER},
    {"throw", THROW},
    {"try", TRY},
    {"class", CLASS},
    {"default", DEFAULT},
    {"else", ELSE},
    {"extends", EXTENDS},
    {"if", IF},
    {"package", PACKAGE},
    {"return", RETURN},
    {"switch", SWITCH},
    {"while", WHILE},
    {"await", AWAIT},
    {"case", CASE},
    {"enum", ENUM},
    {"for", FOR},
    {"with", WITH},
    {"catch", CATCH},
    {"do", DO},
    {"eval", EVAL},
    {"yield", YIELD},
    {"void", VOID},
    {"function", FUNCTION},
};

static TokenType get_token_type(const char *str) {
    for (size_t i = 0; i <sizeof(keyword_token_map) / sizeof(keyword_token_map[0]); i++) {
        if (strcmp(str, keyword_token_map[i].lexeme) == 0) {
            return keyword_token_map[i].type;
        }
    }

    for (size_t i = 0; i <sizeof(single_token_map) / sizeof(single_token_map[0]); i++) {
        if (strcmp(str, single_token_map[i].lexeme) == 0) {
            return single_token_map[i].type;
        }
    }

    return UNKNOWN;
}

static void parse_identifier(Lexer *lexer) {
    int start = lexer->current;

    while (!is_end(lexer) && isalnum(lexer->source[lexer->current])) {
        advance(lexer);
    }

    int len = (lexer->current - start);

    char *lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, lexer->source + start, len);
    
    lexeme[len] = '\0';

    TokenType type = get_token_type(lexeme);
    if (type != UNKNOWN) {
        create_token(lexer, type, lexeme);
    } else {
        create_token(lexer, IDENTIFIER, lexeme);
    }

    lexer->current--;
}

static void parse_string(Lexer *lexer) {
    int start = lexer->current;

    advance(lexer);
    while (!is_end(lexer) && lexer->source[lexer->current] != '\"') {
        advance(lexer);
    }
    advance(lexer);

    int len = (lexer->current - start);

    char *lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, lexer->source + start, len);
    
    lexeme[len] = '\0';

    create_token(lexer, STRING, lexeme);
    lexer->current--;
}

static void parse_numeric(Lexer *lexer) {
    int start = lexer->current;

    while (!is_end(lexer) && isdigit(lexer->source[lexer->current])) {
        advance(lexer);
    }

    int len = (lexer->current - start);

    char *lexeme = (char *)malloc(len + 1);
    strncpy(lexeme, lexer->source + start, len);
    
    lexeme[len] = '\0';

    create_token(lexer, NUMBER, lexeme);
    lexer->current--;
}

static inline void parse_token(Lexer *lexer) {
    for (size_t i = 0; i < sizeof(triple_token_map) / sizeof(triple_token_map[0]); i++) {
        if (strncmp(lexer->source + lexer->current, triple_token_map[i].lexeme, 3) == 0) {
            create_token(lexer, triple_token_map[i].type, triple_token_map[i].lexeme);
            lexer->current += 3;
            return;
        }
    }

    for (size_t i = 0; i < sizeof(double_token_map) / sizeof(double_token_map[0]); i++) {
        if (strncmp(lexer->source + lexer->current, double_token_map[i].lexeme, 2) == 0) {
            create_token(lexer, double_token_map[i].type, double_token_map[i].lexeme);
            lexer->current += 2;
            return;
        }
    }

    char token[] = { lexer->source[lexer->current], '\0' };

    TokenType type = get_token_type(token);
    if (type != UNKNOWN) {
        create_token(lexer, type, token);
    }
    else if (isdigit(token[0])) {
        parse_numeric(lexer);
    }
    else if (token[0] == '\"') {
        parse_string(lexer);
    }
    else {
        parse_identifier(lexer);
    }
}


Token *tokenize(char *source) {
    Lexer *lexer = lexer_init(source);
 
    while (lexer->source[lexer->current] != '\0') {
        while (lexer->source[lexer->current] == ' ' || lexer->source[lexer->current] == '\n')
            advance(lexer);

        parse_token(lexer);
        if (!is_end(lexer)) advance(lexer);
    }

    for (int i = 0; i < lexer->token_count; i++) {
        printf("\nToken: %s | %d", lexer->tokens[i].lexeme, lexer->tokens[i].type);
    }

    return lexer->tokens;
}