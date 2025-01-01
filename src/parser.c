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

static inline Token current(Parser *parser) {
    return parser->tokens[parser->current];
}

static inline void advance(Parser *parser) {
    parser->current++;
}

static Expr *create_expr(ExprType type) {
    Expr *expr = malloc(sizeof(Expr));
    expr->type = type;

    return expr;
}

static Expr *parse_primary(Parser *parser) {
    Token token = current(parser);

    if (token.type == NUMBER || token.type == STRING) {
        Expr *expr = create_expr(EXPR_LITERAL);
        expr->as.literalExpr.value = token;
        return expr;
    } 
    else {
        printf("Unknown primary token %s", token.lexeme);
        exit(EXIT_FAILURE);
    }
}


static Expr *parse_variable_declaration(Parser *parser) {
    Expr *expr = create_expr(EXPR_VAR_DECL);

    Token declarator = current(parser);
    expr->as.varDeclExpr.declarator = strdup(declarator.lexeme);
    advance(parser);

    Token identifier = current(parser);
    expr->as.varDeclExpr.name = strdup(identifier.lexeme);
    advance(parser);

    advance(parser); // =

    expr->as.varDeclExpr.expr = parse_primary(parser);
    advance(parser); // ;

    return expr;
}

static Expr *parse_token(Parser *parser) {
    TokenType type = current(parser).type;

    if (type == LET || type == VAR || type == CONST) {
        return parse_variable_declaration(parser);
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
            printf("Variable Declaration: %s %s", expr->as.varDeclExpr.declarator, expr->as.varDeclExpr.name);
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

Ast *init_ast() {
    Ast *ast = (Ast *)malloc(sizeof(Ast));
    ast->body = malloc(sizeof(Expr) * 2);
    ast->expr_count = 0;
    ast->expr_capacity = 2;

    return ast;
}

Ast *parse_ast(Token *tokens) {
    Parser *parser = init_parser(tokens);
    Ast *ast = init_ast();

    while (current(parser).type != EOF_TOKEN) {
        Expr *expr = parse_token(parser);

        if (ast->expr_count >= ast->expr_capacity) {
            ast->expr_capacity *= 2;
            ast->body = realloc(ast->body, sizeof(Expr) * ast->expr_capacity);
        }

        ast->body[ast->expr_count++] = *expr;
        advance(parser);
    }

    printf("Parsed Expressions:\n");
    for (int i = 0; i < ast->expr_count; i++) {
        print_expr(&ast->body[i]);
    }

    return ast;
}