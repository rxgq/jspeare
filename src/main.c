#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./main.exe <path_to_js>");
        return 1;
    }

    FILE *fptr = fopen(argv[1], "r");
    if (!fptr) {
        perror("Failed to open file");
        fclose(fptr);
        return 1;
    }

    char *file_ext = strrchr(argv[1], '.');
    if (strcmp(file_ext, ".js") != 0) {
        printf("Expected file with a .js extension");
        fclose(fptr);
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    size_t sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc(sizeof(char) * sz + 1);
    if (!buff) {
        perror("Failed to allocate memory for file");
        fclose(fptr);
        return 1;
    }

    size_t read_size = fread(buff, 1, sz, fptr);
    buff[read_size] = '\0';

    if (read_size < sz) {
        buff = (char *)realloc(buff, read_size + 1);
    }

    Token* tokens = tokenize(buff);
    parse_ast(tokens);

    fclose(fptr);
    free(buff);

    return 0;
}