#include "cache.h"

Cache *newCache() {
    
    Cache *this = malloc(sizeof(Cache));
    assert (this != NULL);

    // set expiries to 0 (corrispond to Jan 1st 1970)
    // and response to NULL
    int i;
    for (i = 0; i < CACHE_LEN; i++) {
        this->entries[i].expiry = 0;
        this->entries[i].response = NULL;
    }

    return this;
}

void addEntry(Cache *this, Info *info, FileDesc logFile) {

    if (info->ancount == 0) {
        return; // no answer, no cache
    }

    // find the entry with the smallest expiry.
    CacheEntry *entry = &this->entries[0];
    int i;
    for (i = 1; i < CACHE_LEN; i++) {
        if (this->entries[i].expiry < entry->expiry) {
            entry = &this->entries[i];
        }
    }

    
}