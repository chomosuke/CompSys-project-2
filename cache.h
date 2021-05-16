#ifndef CACHEH
#define CACHEH

#include "head.h"
#include "parser.h"

#define CACHE_LEN 5

// represent a single cache entry
typedef struct {
    Query query;
    Answer answer;
    int ittl;
    int responseLen;
    uint8_t *response;
    time_t expiry;
} CacheEntry;
// an array of cache entries
typedef struct {
    CacheEntry entries[CACHE_LEN];
    // non existent cache will be marked invalid by a expiry in the past
    // and response of NULL
} Cache;
// construct a new cache and initialize all the entries
Cache *newCache();
// add an entry to Cache, evict and log if needed
void addEntry(Cache *this, Info *info, FileDesc logFile);
// find a cache entry with the same query if not found return NULL
CacheEntry *findEntry(Cache *this, Query query);
// free all memory that thas been malloced to cache
void destroyCache(Cache *this);

#endif