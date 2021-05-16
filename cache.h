#ifndef CACHEH
#define CACHEH

#include "head.h"
#include "parser.h"

#define CACHE_LEN 5

typedef struct {
    Query query;
    Answer answer;
    int ittl;
    int responseLen;
    uint8_t *response;
    time_t expiry;
} CacheEntry;
typedef struct {
    CacheEntry entries[CACHE_LEN];
    // non existent cache will be marked invalid by a expiry in the past
    // and response of NULL
} Cache;
Cache *newCache();
void addEntry(Cache *this, Info *info, FileDesc logFile);
CacheEntry *findEntry(Cache *this, Query query); // if not found or expired return NULL
void destroyCache(Cache *this);

#endif