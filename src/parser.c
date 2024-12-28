#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static Expr *create_expr(Parser *parser, ExprType type) {
    Expr *expr = malloc(sizeof(Expr));
    expr->type = type;

    return expr;
}

static Expr *parse_primary(Parser *parser) {
    Token token = parser->tokens[parser->current];

    if (token.type == NUMBER || token.type == STRING) {
        Expr *expr = malloc(sizeof(Expr));
        expr->type = EXPR_LITERAL;
        expr->as.literalExpr.value = token;
        return expr;
    } 
    else {
        printf("Unknown primary token %s", token.lexeme);
        exit(EXIT_FAILURE);
    }
}


static Expr *parse_variable_declaration(Parser *parser, TokenType type) {
    Expr *expr = malloc(sizeof(Expr));
    expr->type = EXPR_VAR_DECL;

    advance(parser); // let, var, etc

    Token identifier = parser->tokens[parser->current];
    expr->as.varDeclExpr.name = strdup(identifier.lexeme);
    advance(parser); // x

    advance(parser); // =

    expr->as.varDeclExpr.expr = parse_primary(parser);
    advance(parser); // ;

    return expr;
}

static Expr *parse_token(Parser *parser) {
    TokenType type = parser->tokens[parser->current].type;

    if (type == LET || type == VAR || type == CONST) {
        return parse_variable_declaration(parser, type);
    } else {
        return parse_primary(parser);
    }
}

static void print_expr(Expr *expr) {
    switch (expr->type) {
        case EXPR_LITERAL:
            printf("Literal: %s\n", expr->as.literalExpr.value.lexeme);
            break;
        case EXPR_VAR_DECL:
            printf("Variable Declaration: %s", expr->as.varDeclExpr.name);
            if (expr->as.varDeclExpr.expr) {
                printf(" = ");
                print_expr(expr->as.varDeclExpr.expr);
            }
            printf("\n");
            break;
        default:
            printf("Unknown expression type\n");
            break;
    }
}

Ast *parse_ast(Token *tokens) {
    Parser *parser = init_parser(tokens);

    Ast *ast = (Ast *)malloc(sizeof(Ast));
    ast->body = malloc(sizeof(Expr) * 2);

    int expr_count = 0;
    while (parser->tokens[parser->current].type != EOF_TOKEN) {
        Expr *expr = parse_token(parser);

        ast->body[expr_count++] = *expr;
        advance(parser);
    }

    printf("Parsed Expressions:\n");
    for (int i = 0; i < expr_count; i++) {
        print_expr(&ast->body[i]);
    }

    return ast;
}
