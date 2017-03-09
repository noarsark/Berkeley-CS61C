#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Deletes all comments of a C source file and prints the rest to
 * the file OUTPUT.
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
            /* String literals, no comment inside. */
            char match = fgetc(in);
            while (1) {
                if (match == c) {
                    break;
                }
                fputc(match, out);  // Here, match cannot be EOF for a valid C source file.
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
                /* One-line comment detected. */
                char line[256];
                do {
                    if (fgets(line, sizeof line, in) == NULL){
                        break;
                    }
                } while (line[strlen(line) - 2] == '\\');
                continue;
            }

            if (next_c == '*') {
                /* Multi-line comment detected */
                while (1) {
                    char match_star = getc(in);
                    if (match_star == '*') {
                        char match_back_slash = getc(in);
                        if (match_back_slash == '/') {
                            /* Comment closed. */
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
    printf("Choose the C source file you want to delete comments of: ");
    scanf("%s", input);
    printf("Running comment deleter...\n");
    comment_deleter(input);
    printf("Comments deleted.\n");
    return 0;
}