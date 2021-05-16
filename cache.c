#include "cache.h"
#include "handler.h"

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
    CacheEntry *entry = findEntry(this, info->querys[0]); // if this query already exist and is being replaced
    time_t currentTime = time(NULL);
    if (entry == NULL) {
        // find a expired entry
        int i;
        for (i = 0; i < CACHE_LEN; i++) {
            if (this->entries[i].response != NULL && this->entries[i].expiry <= currentTime) {
                entry = &this->entries[i];
                break;
            }
        }
    }
    if (entry == NULL) {
        // evict entry with shortest ttl
        entry = &this->entries[0];
        int i;
        for (i = 1; i < CACHE_LEN; i++) {
            if (this->entries[i].expiry < entry->expiry) {
                entry = &this->entries[i];
            }
        }
    }

    if (entry->response != NULL) {
        // log
        char timeStamp[TIME_LEN];
        getTimeStamp(timeStamp);
        char logBuff[MAX_LOG_LEN];
        sprintf(logBuff, "%s replacing %s by %s\n", timeStamp, entry->query.qname, info->querys[0].qname);

        // free previous
        free(entry->response);
        free(entry->answer.rdata);

    }

    // entry is the entry to be overwritten
    entry->query = info->querys[0];
    entry->answer = info->answers[0];
    entry->answer.rdata = malloc(entry->answer.rdlength * sizeof(uint8_t));
    assert (entry->answer.rdata != NULL);
    memcpy(entry->answer.rdata, info->answers[0].rdata, entry->answer.rdlength * sizeof(uint8_t));
    entry->ittl = info->ittl;
    entry->responseLen = info->originalLen;
    entry->response = malloc(entry->responseLen * sizeof(uint8_t));
    assert (entry->response != NULL);
    memcpy(entry->response, info->original, entry->responseLen * sizeof(uint8_t));

    // calculate expiry
    entry->expiry = time(NULL) + entry->answer.ttl;
}

CacheEntry *findEntry(Cache *this, Query query) {
    int i;
    for (i = 0; i < CACHE_LEN; i++) {
        Query *thisQ = &this->entries[i].query;
        if (
            query.qclass == thisQ->qclass &&
            strcmp(query.qname, thisQ->qname) == 0 &&
            query.qtype == thisQ->qtype
        ) {
            return &this->entries[i];
        }
    }
    return NULL;
}

void destroyCache(Cache *this) {
    int i;
    for (i = 0; i < CACHE_LEN; i++) {
        if (this->entries[i].response != NULL) {
            free(this->entries[i].response);
            free(this->entries[i].answer.rdata);
        }
    }
    free(this);
}