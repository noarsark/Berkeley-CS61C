#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/**
 * Deletes all comments of a C source file.
 * TODO: To handle special case for "\", eg,
 *
 * [code]  //this line is a comment \
 *         but this line is also a comment [\code]
 */
void comment_deleter(char *input) {
    FILE *in = fopen(input, "r");
    FILE *out = fopen("output", "w");
    if (in == NULL) {
        fprintf(stderr, "File %s not found!\n", input);
        exit(1);
    }
    if (out == NULL) {
        fprintf(stderr, "File output cannot be opened!\n");
        exit(1);
    }
    char c;
    while ((c = fgetc(in)) != EOF) {
        if (c == '\'' || c == '"') {
            /* Quotes, no comment inside. */
            char match = fgetc(in);
            while (1) {
                if (match == c) {
                    break;
                }
                fputc(match, out); // Here, match cannot be EOF for a valid C source file.
                match = fgetc(in);
            }
        } else if (c == '/') {
            /* May be a comment, checks the next character */
            char next_c = fgetc(in);
            /* Stops checking if end of file. */
            if (next_c == EOF) {
                break;
            }

            if (next_c == '/') {
                /* One-line comment detected, here assuming that
                 * the line isn't too long. */
                char line[256];
                fgets(line, sizeof line, in);
                continue;
            }

            if (next_c == '*') {
                /* Multi-line comment detected */
                while (1) {
                    char match_star = getc(in);
                    if (match_star == '*') {
                        char match_back_slash = getc(in);
                        if (match_back_slash == '/') {
                            break;
                        }
                    }
                }
                continue;
            }
        } else {
            /* If not comment, just prints it to the output. */
            fputc(c, out);
        }
    }
    fclose(in);
    fclose(out);
}

int main() {
    char input[256];
    printf("Choose a C source file which you want to delete comments of: ");
    scanf("%s", input);
    printf("Running comment deleter\n");
    comment_deleter(input);
    printf("Comments deleted.\n");
    return 0;
}