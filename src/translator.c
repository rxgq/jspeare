#include <stdio.h>
#include <stdlib.h>
#include "translator.h"


void from_primary(FILE *out, Expr *expr) {
    fprintf(out, "%s ", expr->as.literalExpr.value.lexeme);
}

void from_var_decl(FILE *out, Expr *expr) {
    char *var_name = expr->as.varDeclExpr.name;
    printf("%s", var_name);

    fprintf(out, "alloweth t beest known yond the variable %s beest setteth to ", var_name);
    from_primary(out, expr->as.varDeclExpr.expr);
}

void to_shakespearean(Ast *ast) {
    FILE *out = fopen("out/out.txt", "w");
    if (!out) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < ast->expr_count; i++) {
        Expr *expr = &ast->body[i];
        from_var_decl(out, expr);
        break;
    }

    fclose(out);
}
