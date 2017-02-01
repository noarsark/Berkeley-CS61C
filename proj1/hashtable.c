
#include "hashtable.h"
#include <stdlib.h>


/*
 * creates a hashtable
 */
HashTable *createHashTable(int size, unsigned int (*hashFunction) (void *),
			   int (*equalFunction)(void *, void *)){
  int i = 0;

  /*
   * create the hashtable
   */
  HashTable *newTable = malloc(sizeof(HashTable));
  newTable->size = size;

  /*
   * Allocate the array of pointers which points to the hash buckets
   * and initialize all of them to NULL;
   */
  newTable->data = malloc(sizeof(struct HashBucket *) * size);
  for(i = 0; i < size; ++i){
    newTable->data[i] = NULL;
  }

  /*
   * Assign the function pointers and return the hashtable
   */
  newTable->hashFunction = hashFunction;
  newTable->equalFunction = equalFunction;
  return newTable;
}

/*
 * inserts the data
 */
void insertData(HashTable *table, void *key, void *data){
  /*
   * compute the location for the data
   */
  unsigned int location = ((table->hashFunction)(key)) % table->size;

  /*
   * Allocate a new bucket
   */
  struct HashBucket *newBucket = (struct HashBucket *) 
    malloc(sizeof(struct HashBucket));

  /*
   * Insert it into the table
   */
  newBucket->next = table->data[location];
  newBucket->data = data;
  newBucket->key = key;
  table->data[location] = newBucket;
}

void * findData(HashTable *table, void *key){
  /*
   * compute the hash function
   */
  unsigned int location = ((table->hashFunction)(key)) % table->size;
  struct HashBucket *lookAt =
    table->data[location];

  /*
   * Look up the data, if equal return it
   */
  while(lookAt != NULL){
    if((table->equalFunction)(key, lookAt->key) != 0){
      return lookAt->data;
    }
    lookAt = lookAt->next;
  }

  /*
   * otherwise return null
   */
  return NULL;
}
