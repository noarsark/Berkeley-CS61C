/*
 * Include the provided hashtable library
 */
#include "hashtable.h"

/*
 * Include the header file
 */
#include "philspel.h"

/*
 * Standard IO and file routines
 */
#include <stdio.h>

/*
 * General utility routines (including malloc())
 */
#include <stdlib.h>

/*
 * Character utility routines.
 */
#include <ctype.h>

/*
 * String utility routines
 */
#include <string.h>

/*
 * this hashtable stores the dictionary
 */
HashTable *dictionary;

/*
 * the MAIN routine.  You can safely print debugging information
 * to standard error (stderr) and it will be ignored in the grading
 * process, in the same way which this does.
 */

void test_process_input();

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Specify a dictionary\n");
        return 0;
    }
    /*
     * Allocate a hash table to store the dictionary
     */
    fprintf(stderr, "Creating hashtable\n");
    dictionary = createHashTable(2255, &stringHash, &stringEquals);

    fprintf(stderr, "Loading dictionary %s\n", argv[1]);
    readDictionary(argv[1]);
    fprintf(stderr, "Dictionary loaded\n");

    fprintf(stderr, "Processing stdin\n");
    processInput();
    fprintf(stderr, "stdin processed\n");

//    fprintf(stderr, "Processing sampleInput\n");
//    test_process_input();
//    fprintf(stderr, "sampleInput processed\n");
    /* main in C should always return 0 as a way of telling
       whatever program invoked this that everything went OK
       */
    return 0;
}

/*
 * You need to define this function. void *s can be safely casted
 * to a char * (null terminated string) which is done for you here for
 * convenience. (My note: nope, it is actually the C++ case.
 * Should not cast void * in C.)
 */
unsigned int stringHash(void *s) {
    char *ch = s;
    unsigned int hash = 23333;
    while (*ch != '\0') {
        hash = 23 * hash + (unsigned int) *ch;
        ++ch;
    }
    return hash;
}

/*
 * You need to define this function.  It should return a nonzero
 * value if the two strings are identical (case sensitive comparison)
 * and 0 otherwise.
 */
int stringEquals(void *s1, void *s2) {
    char *string1 = s1;
    char *string2 = s2;
    if (strcmp(string1, string2) == 0) {
        return 1;
    }
    return 0;
}

/*
 * this function should read in every word in the dictionary and
 * store it in the dictionary.  You should first open the file specified,
 * then read the words one at a time and insert them into the dictionary.
 * Once the file is read in completely, exit.  You will need to allocate
 * (using malloc()) space for each word.  As described in the specs, you
 * can initially assume that no word is longer than 60 characters.  However,
 * for the final 20% of your grade, you cannot assumed that words have a bounded
 * length. You can NOT assume that the specified file exists.  If the file does
 * NOT exist, you should print some message to standard error and call exit(0)
 * to cleanly exit the program. Since the format is one word at a time, with
 * returns in between, you can
 * safely use fscanf() to read in the strings.
 */
void readDictionary(char *filename) {
    FILE *fp = fopen(filename, "r");
    /* Check for empty file. */
    if (fp == NULL) {
        fprintf(stderr, "File does not exist");
        exit(0);
    }

    /* Set default buffer size for current word, which can enlarge later. */
    const int DEFAULT_BUFFER_SIZE = 61;
    int buffer_size = DEFAULT_BUFFER_SIZE;
    char *word = malloc(buffer_size * sizeof(char));

    char c;           // current character
    int total_ch = 0; // total number of characters in current word

    /* Keep getting character until the end the file. */
    while ((c = fgetc(fp)) != EOF) {
        if (isalpha(c)) {
            /* current character is letter, middle of a word. */
            word[total_ch] = c;
            ++total_ch;

            /* Reallocate larger memory if total characters of current word
             * are larger than buffer size. */
            if (total_ch == buffer_size) {
                buffer_size *= 2;
                word = realloc(word, buffer_size * sizeof(char));
            }
        } else {
            /* current character is not letter, ie, reach the end of the word. */
            word[total_ch] = '\0';
            /* Add the word to dictionary. */
            insertData(dictionary, word, word);
            /* reset for getting new word. */
            buffer_size = DEFAULT_BUFFER_SIZE;
            total_ch = 0;
            word = realloc(word, buffer_size * sizeof(char));
        }
    }
    free(word);
    fclose(fp);
}

/* If word length is zero, then it shouldn't be a word. */
int is_word(char *word) {
    return strlen(word);
}

/* Check if the given word satisfies one of the three given cases,
 * then return 1; else return 0. Assuming the word is valid, ie,
 * length > 0. */
int is_currectly_spelled(char *word) {
    /* Case one, exact same word in dictionary. */
    if (findData(dictionary, word)) {
        return 1;
    }
    /* Case two, all but the first letters to lowercase. */
    for (int i = 1; i < strlen(word); ++i) {
        word[i] = tolower(word[i]);
    }
    if (findData(dictionary, word)) {
        return 1;
    }
    /* Case three, all to lower case. . */
    word[0] = tolower(word[0]);
    if (findData(dictionary, word)) {
        return 1;
    }

    return 0;
}

/*
 * This should process standard input and copy it to standard output
 * as specified in specs.  EG, if a standard dictionary was used
 * and the string "this is a taest of  this-proGram" was given to
 * standard input, the output to standard output (stdout) should be
 * "this is a teast [sic] of  this-proGram".
 *
 * (Note: I think "taest" or
 * "teast" here, one of which may be a typo in comment specs ...)
 *
 * All words should be checked against the dictionary as they are input,
 * again with all but the first letter converted to lowercase,
 * and finally with all letters converted to lowercase.
 * Only if all 3 cases are not in the dictionary should it
 * be reported as not being found, by appending " [sic]" after the error.
 *
 * Since we care about preserving whitespace, and pass on all non alphabet
 * characters untouched, and with all non alphabet characters acting as
 * word breaks, scanf() is probably insufficient (since it only considers
 * whitespace as breaking strings), so you will probably have
 * to get characters from standard input one at a time.
 *
 * As stated in the specs, you can initially assume that no word is longer than
 * 60 characters, but you may have strings of non-alphabetic characters (eg,
 * numbers, punctuation) which are longer than 60 characters. For the final 20%
 * of your grade, you can no longer assume words have a bounded length.
 */
void processInput() {
    /* Set default buffer size for current word, which can enlarge later. */
    const int DEFAULT_BUFFER_SIZE = 61;
    int buffer_size = DEFAULT_BUFFER_SIZE;
    char *word = malloc(buffer_size * sizeof(char));

    char c;           // current character
    int total_ch = 0; // total number of characters in current word

    /* Keep getting character, one by one, until the end the stdin. */
    while ((c = getchar()) != EOF) {
        if (isalpha(c)) {
            /* current character is letter, still middle of word. */
            word[total_ch] = c;
            ++total_ch;

            /* Reallocate larger memory if total characters of current word
             * are larger than buffer size. */
            if (total_ch == buffer_size) {
                buffer_size *= 2;
                word = realloc(word, buffer_size * sizeof(char));
            }
        } else {
            /* current character is not letter, ie, reach the end of the word. */
            word[total_ch] = '\0';
            printf("%s", word);

            /* Check if the word is valid and correctly spelled. */
            if (is_word(word) && !is_currectly_spelled(word)) {
                printf(" [sic]");
            }

            printf("%c" ,c);
            /* reset for getting new word. */
            buffer_size = DEFAULT_BUFFER_SIZE;
            total_ch = 0;
            word = realloc(word, buffer_size * sizeof(char));
        }
    }
    free(word);
}

/* Same as the processInput() routine but use file input instead of stdin. */
//void test_process_input() {
//    /* Set default buffer size for current word, which can enlarge later. */
//    const int DEFAULT_BUFFER_SIZE = 61;
//    int buffer_size = DEFAULT_BUFFER_SIZE;
//    char *word = malloc(buffer_size * sizeof(char));
//
//    char c;           // current character
//    int total_ch = 0; // total number of characters in current word
//
//    FILE *fr = fopen("sampleInput", "r");
//    FILE *fw = fopen("actualOutput", "w");
//    /* Keep getting character, one by one, until the end the stdin. */
//    while ((c = fgetc(fr)) != EOF) {
//        if (isalpha(c)) {
//            /* current character is letter, still middle of word. */
//            word[total_ch] = c;
//            ++total_ch;
//
//            /* Reallocate larger memory if total characters of current word
//             * are larger than buffer size. */
//            if (total_ch == buffer_size) {
//                buffer_size *= 2;
//                word = realloc(word, buffer_size * sizeof(char));
//            }
//        } else {
//            /* current character is not letter, ie, reach the end of the word. */
//            word[total_ch] = '\0';
//            fprintf(fw, "%s", word);
//
//            /* Check if the word is valid and correctly spelled. */
//            if (is_word(word) && !is_currectly_spelled(word)) {
//                fprintf(fw, " [sic]");
//            }
//
//            fprintf(fw, "%c" ,c);
//            /* reset for getting new word. */
//            buffer_size = DEFAULT_BUFFER_SIZE;
//            total_ch = 0;
//            word = realloc(word, buffer_size * sizeof(char));
//        }
//    }
//    free(word);
//    fclose(fr);
//    fclose(fw);
//}
