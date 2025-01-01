#include "token.h"

#ifndef PARSER_H
#define PARSER_H

typedef struct Expr Expr;

typedef struct BinaryExpr {
    
} BinaryExpr;

typedef struct VarDeclExpr {
    char *name;
    char *declarator;
    Expr *expr;
} VarDeclExpr;

typedef struct LiteralExpr {
    Token value;
} LiteralExpr;

typedef enum ExprType {
    EXPR_BINARY,
    EXPR_VAR_DECL,
    EXPR_LITERAL
} ExprType;

typedef struct Expr {
    ExprType type;

    union {
        VarDeclExpr varDeclExpr;
        BinaryExpr binaryExpr;
        LiteralExpr literalExpr;
    } as;
} Expr;

typedef struct Ast {
    int expr_count;
    int expr_capacity;
    Expr *body;
} Ast;

typedef struct Parser {
    Token *tokens;
    int current;
} Parser;

extern Ast *parse_ast(Token *tokens);

#endif