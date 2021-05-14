#include "handler.h"

ReadBuff *newReadBuff(int sourceConnection, int length) {
    ReadBuff *buff = malloc(sizeof(ReadBuff));
    buff->length = length;
    buff->readLength = 0;
    buff->bytes = malloc(buff->length * sizeof(char));
    buff->sourceConnection = sourceConnection;
}

ReadBuffs *newReadBuffs() {
    ReadBuffs *readBuffs = malloc(sizeof(ReadBuffs));
    readBuffs->length = 0;
    readBuffs->size = 10; // initial size of 10
    readBuffs->buffs = malloc(readBuffs->size * sizeof(ReadBuff*));
    return readBuffs;
}

ReadBuff *findBuff(ReadBuffs *readBuffs, int connection) {
    int i = 0;
    while (i < readBuffs->size) {
        if (readBuffs->buffs[i]->sourceConnection == connection) {
            return readBuffs->buffs[i];
        }
        i++;
    }
    return NULL;
}

