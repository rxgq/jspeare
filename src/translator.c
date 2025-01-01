#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "translator.h"


void from_primary(FILE *out, Expr *expr) {
    fprintf(out, "%s", expr->as.literalExpr.value.lexeme);
}

void from_expression(FILE *out, Expr *expr) {
    from_primary(out, expr);
}

void from_var_decl(FILE *out, Expr *expr) {
    char *var_name = expr->as.varDeclExpr.name;
    printf("%s", var_name);

    if (strcmp(expr->as.varDeclExpr.declarator, "const") == 0) {
        fprintf(out, "Hark! Let it be proclaimed that the immutable variable %s shall be bound to ", var_name);
    }

    if (strcmp(expr->as.varDeclExpr.declarator, "let") == 0) {
        fprintf(out, "Lo! Be it known that the mutable variable %s shall be assigned to ", var_name);
    }

    if (strcmp(expr->as.varDeclExpr.declarator, "var") == 0) {
        fprintf(out, "Verily, let the variable %s be declared henceforth and assigned to ", var_name);
    }

    from_expression(out, expr->as.varDeclExpr.expr);
    fprintf(out, "\n");
}

void to_shakespearean(Ast *ast) {
    FILE *out = fopen("out/out.txt", "w");
    if (!out) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ast->expr_count; i++) {
        Expr *expr = &ast->body[i];
        if (expr->type == EXPR_VAR_DECL) {
            from_var_decl(out, expr);
        }
        else {
            break;
        }
    }

    fclose(out);
}
