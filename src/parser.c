#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

static Parser *init_parser(Token *tokens) {
    Parser *parser = (Parser *)malloc(sizeof(Parser));
    parser->tokens = tokens;
    parser->current = 0;
    
    return parser;
}

static void advance(Parser *parser) {
    parser->current++;
}

// static int match(Parser *parser, TokenType type) {

// }

static void parse_token(Parser *parser) {
    printf("%s\n", parser->tokens[parser->current].lexeme);
}

void parse_ast(Token *tokens) {
    Parser *parser = init_parser(tokens);

    while (parser->tokens[parser->current].type != EOF_TOKEN) {
        parse_token(parser);
        advance(parser);
    }

}

