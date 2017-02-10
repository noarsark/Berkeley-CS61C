
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"
#include "tables.h"

const int SYMTBL_NON_UNIQUE = 0;
const int SYMTBL_UNIQUE_NAME = 1;

#define INITIAL_SIZE 5
#define SCALING_FACTOR 2

/*******************************
 * Helper Functions
 *******************************/

void allocation_failed() {
    write_to_log("Error: allocation failed\n");
    exit(1);
}

void addr_alignment_incorrect() {
    write_to_log("Error: address is not a multiple of 4.\n");
}

void name_already_exists(const char *name) {
    write_to_log("Error: name '%s' already exists in table.\n", name);
}

void write_symbol(FILE *output, uint32_t addr, const char *name) {
    fprintf(output, "%u\t%s\n", addr, name);
}

void null_table_error() {
    write_to_log("Error: the table name is not found.\n");
    exit(1);
}

/*******************************
 * Symbol Table Functions
 *******************************/

/* Creates a new SymbolTable containg 0 elements and returns a pointer to that
   table. Multiple SymbolTables may exist at the same time.
   If memory allocation fails, you should call allocation_failed().
   Mode will be either SYMTBL_NON_UNIQUE or SYMTBL_UNIQUE_NAME. You will need
   to store this value for use during add_to_table().
 */
SymbolTable *create_table(int mode) {
    SymbolTable *table = malloc(sizeof(SymbolTable));
    if (!table) {
        allocation_failed();
    }
    table->len = 0;
    table->cap = INITIAL_SIZE;
    table->tbl = malloc(table->cap * sizeof(Symbol));
    if (!table->tbl) {
        allocation_failed();
    }
    table->mode = mode;
    return table;
}

/* Frees the given SymbolTable and all associated memory. */
void free_table(SymbolTable *table) {
    /* YOUR CODE HERE */
    if (!table) {
        return;
    }
    if (table->tbl) {
        free(table->tbl);
    }
    free(table);
}

/* A suggested helper function for copying the contents of a string. */
static char *create_copy_of_str(const char *str) {
    size_t len = strlen(str) + 1;
    char *buf = malloc(len);
    if (!buf) {
        allocation_failed();
    }
    strncpy(buf, str, len);
    return buf;
}

/* Adds a new symbol and its address to the SymbolTable pointed to by TABLE.
   ADDR is given as the byte offset from the first instruction. The SymbolTable
   must be able to resize itself as more elements are added.

   Note that NAME may point to a temporary array, so it is not safe to simply
   store the NAME pointer. You must store a copy of the given string.

   If ADDR is not word-aligned, you should call addr_alignment_incorrect() and
   return -1. If the table's mode is SYMTBL_UNIQUE_NAME and NAME already exists
   in the table, you should call name_already_exists() and return -1. If memory
   allocation fails, you should call allocation_failed().

   Otherwise, you should store the symbol name and address and return 0.
 */
int add_to_table(SymbolTable *table, const char *name, uint32_t addr) {
    /* YOUR CODE HERE */
    /* Checks if the table is null */
    if (!table) {
        null_table_error();
    }
    /* Checks if ADDR is word-aligned. */
    if (addr % 4) {
        addr_alignment_incorrect();
        return -1;
    }
    /* Checks if table's mode is mode is SYMTBL_UNIQUE_NAME and
     * NAME already exists. */
    if (table->mode == SYMTBL_UNIQUE_NAME && get_addr_for_symbol(table, name) != -1) {
        name_already_exists(name);
        return -1;
    }
    /* Resizes the table if full. */
    if (table->len == table->cap) {
        table->cap *= 2;
        SymbolTable *tmp = realloc(table, 2 * sizeof(SymbolTable));
        /* exit if reallocation fails. */
        if (!tmp) {
            free(table->tbl);
            free(table);
            allocation_failed();
        }
        table = tmp;
        tmp = NULL;
    }
    table->tbl[table->len].name = create_copy_of_str(name);
    table->tbl[table->len].addr = addr;
    return 0;
}

/* Returns the address (byte offset) of the given symbol. If a symbol with name
   NAME is not present in TABLE, return -1.
 */
int64_t get_addr_for_symbol(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->len; i++) {
        if (strcmp(name, (table->tbl)[i].name) == 0) {
            return (table->tbl)[i].addr;
        }
    }
    return -1;
}

/* Writes the SymbolTable TABLE to OUTPUT. You should use write_symbol() to
   perform the write. Do not print any additional whitespace or characters.
 */
void write_table(SymbolTable *table, FILE *output) {
    for (int i = 0; i < table->len; i++) {
        write_symbol(output, (table->tbl)[i].addr, (table->tbl)[i].name);
    }
}
