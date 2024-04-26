#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  Pair * pair = createPair(key,value);
  long posicion = hash(key,map->capacity);

  while(map->buckets[posicion % map->capacity] != NULL)
    {
      posicion++;
    }
  map->buckets[posicion % map->capacity] = pair;
  map->size++;
  map->current  = posicion % map->capacity;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  Pair ** anterior_buckets = map->buckets;
  long anterior_capacity = map->capacity;
  map->capacity = map->capacity * 2;
  map->buckets = (Pair **)calloc(map->capacity,sizeof(Pair *));
  map->size = 0;
  
  for(long i = 0; i < map->capacity ; i++)
    {
      map->buckets[i] = NULL;
    }

  for(long i = 0; i < anterior_capacity ; i++)
    {
      if(anterior_buckets[i] != NULL)
      {
        insertMap(map,anterior_buckets[i]->key,anterior_buckets[i]->value);
      }
    }
}


HashMap * createMap(long capacity) {
  HashMap * map = (HashMap *)malloc(sizeof(HashMap));
  map->buckets = (Pair **)calloc(capacity,sizeof(Pair *));
  map->capacity = capacity;
  map->size = 0;
  map->current = -1;
  for(long i = 0 ; i < capacity ; i++)
    {
      map->buckets[i] = NULL;
    }
  return map;
}

void eraseMap(HashMap * map,  char * key) {
  Pair * pair = searchMap(map, key);
  if(pair == NULL) return;
  pair->key = NULL;
  pair->value = NULL;
  map->size--;
}

Pair * searchMap(HashMap * map,  char * key) {
  long posicion = hash(key,map->capacity);
  Pair * pair = map->buckets[posicion % map->capacity];

  while(strcmp(pair->key , key))
    {
      pair = map->buckets[posicion % map->capacity];
      if(pair == NULL) return NULL;
      posicion++;
    }
  map->current = posicion % map->capacity;
  return pair;
}

Pair * firstMap(HashMap * map) {
  for(long i = 0; i < map->capacity ; i++)
    {
      Pair * pair = map->buckets[i];
      if(pair != NULL && pair->key != NULL)
      {
        map->current  = i;
        return pair;
      }
    }
  return NULL;
}

Pair * nextMap(HashMap * map) {
  for(long i = map->current + 1; i < map->capacity ; i++)
    {
      Pair * pair = map->buckets[i];
      if(pair != NULL && pair->key != NULL)
      {
        map->current  = i;
        return pair;
      }
    }
  return NULL;
}
