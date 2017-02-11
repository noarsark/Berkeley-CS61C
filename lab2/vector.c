#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

struct vector_t {
    size_t size;
    int *data;
};

/* Utility function to handle allocation failures. In this
   case we print a message and exit. */
static void allocation_failed() {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
}

/* Create a new vector */
vector_t *vector_new() {
    vector_t *retval;

    retval = malloc(sizeof(vector_t));

    if (retval == NULL) {
        allocation_failed();
    }

    retval->size = 1;
    retval->data = malloc(retval->size * sizeof(int));

    if (retval->data == NULL) {
        free(retval);
        allocation_failed();
    }

    retval->data[0] = 0;

    return retval;
}

/* Free up the memory allocated for the passed vector */
void vector_delete(vector_t *v) {
    if (v != NULL) {
        free(v->data);
        free(v);
    }
}

/* Return the value in the vector */
int vector_get(vector_t *v, size_t loc) {
    if (v == NULL) {
        fprintf(stderr, "vector_get: passed a NULL vector.\n");
        abort();
    }

    /* If the requested location is higher than we have allocated, return 0.
     * Otherwise, return what is in the passed location.
     */
    if (loc < v->size) {
        return v->data[loc];
    } else {
        return 0;
    }
}

/* Set a value in the vector. If the extra memory allocation fails, call
   allocation_failed(). */
void vector_set(vector_t *v, size_t loc, int value) {
    if (v == NULL) {
        fprintf(stderr, "vector_get: passed a NULL vector.\n");
        abort();
    }
    if (loc < v->size) {
        v->data[loc] = value;
        return;
    }
    /* Try to insert the value to the back of the vector. */
    v->size = loc + 1;
    int *tmp = realloc(v->data, v->size * sizeof v->data);
    if (!tmp) {
        vector_delete(v);
        allocation_failed();
    } else {
        v->data = tmp;
        for (size_t i = v->size; i < loc; ++i) {
            v->data[i] = 0;
        }
        v->data[loc] = value;
    }
}








