#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

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
    if (strcmp(file_ext, ".js") == 1) {
        printf("Expected file with a .js extension");
        fclose(fptr);
        return 1;
    }

    fseek(fptr, 0, SEEK_END);
    size_t sz = ftell(fptr);
    rewind(fptr);

    char *buff = (char *)malloc(sizeof(char) * sz);
    if (!buff) {
        perror("Failed to allocate memory for file");
        fclose(fptr);
        return 1;
    }

    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';
    
    Token* tokens = tokenize(buff);

    fclose(fptr);
    free(buff);

    return 0;
}