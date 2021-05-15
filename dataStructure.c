#include "dataStructure.h"

ReadBuff *newReadBuff(FileDesc connection, int length) {
    ReadBuff *this = malloc(sizeof(ReadBuff));
    this->length = length;
    this->readLength = 0;
    this->bytes = malloc(this->length * sizeof(uint8_t));
    assert(this->bytes != NULL);
    this->connection = connection;
    return this;
}

void buffRead(ReadBuff *this) {
    assert(this->length > this->readLength);
    int readCount = read(this->connection, 
        this->bytes + this->readLength, 
        this->length - this->readLength);
    this->readLength += readCount;
}

bool isFinished(ReadBuff *this) {
    return this->length == this->readLength;
}

void destroyReadBuff(ReadBuff *this) {
    free(this->bytes);
    free(this);
}

ReadBuffs *newReadBuffs() {
    ReadBuffs *this = malloc(sizeof(ReadBuffs));
    assert(this != NULL);
    this->length = 0;
    this->size = 10; // initial size of 10
    this->buffs = malloc(this->size * sizeof(ReadBuff*));
    assert(this->buffs != NULL);
    return this;
}

ReadBuff *findBuff(ReadBuffs *this, FileDesc connection) {
    int i = 0;
    while (i < this->length) {
        if (this->buffs[i]->connection == connection) {
            return this->buffs[i];
        }
        i++;
    }
    return NULL;
}

void addBuff(ReadBuffs *this, ReadBuff *readBuff) {
    if (this->length == this->size) {
        // don't have space to add more, double the size
        this->size *= 2;
        this->buffs = realloc(this->buffs, this->size * sizeof(ReadBuff*));
        assert(this->buffs != NULL);
    }
    this->buffs[this->length] = readBuff;
    this->length += 1;
}

ReadBuff *popBuff(ReadBuffs *this, FileDesc connection) {
    ReadBuff *buff = NULL;
    int i = 0;
    while (i < this->length) {
        if (this->buffs[i]->connection == connection) {
            buff = this->buffs[i];
            break;
        }
        i++;
    }
    int j;
    for (j = i + 1; j < this->length; j++, i++) {
        this->buffs[i] = this->buffs[j];
    }
    if (buff != NULL) {
        this->length -= 1;
    }
    return buff;
}

void destoryReadBuffs(ReadBuffs *this) {
    int i;
    for (i = 0; i < this->length; i++) {
        free(this->buffs[i]);
    }
    free(this->buffs);
    free(this);
}

QueAnsPairs *newQAPairs() {
    QueAnsPairs *this = malloc(sizeof(QueAnsPairs));
    assert(this != NULL);
    this->length = 0;
    this->size = 10; // initial size of 10
    this->pairs = malloc(this->size * sizeof(QueAnsPair));
    assert(this->pairs != NULL);
    return this;
}

void addQA(QueAnsPairs *this, FileDesc queryConnection, FileDesc answerConnection) {
    if (this->length == this->size) {
        // don't have space to add more, double the size
        this->size *= 2;
        this->pairs = realloc(this->pairs, this->size * sizeof(QueAnsPair));
        assert(this->pairs != NULL);
    }
    this->pairs[this->length].queryConnection = queryConnection;
    this->pairs[this->length].answerConnection = answerConnection;
    this->length += 1;
}

FileDesc findAns(QueAnsPairs *this, FileDesc queryConnection) {
    int i;
    for (i = 0; i < this->length; i++) {
        if (this->pairs[i].queryConnection == queryConnection) {
            return this->pairs[i].answerConnection;
        }
    }
    // not found, something's wrong
    assert(FALSE);
}

void removeQA(QueAnsPairs *this, FileDesc queryConnection) {
    int i;
    for (i = 0; i < this->length; i++) {
        if (this->pairs[i].queryConnection == queryConnection) {
            break;
        }
    }
    if (i == this->length) {
        // not found, something's wrong
        assert(FALSE);
    }
    int j;
    for (j = i + 1; j < this->length; i++, j++) {
        this->pairs[i] = this->pairs[j];
    }
    this->length -= 1;
}

void destroyQAPairs(QueAnsPairs *this) {
    free(this->pairs);
    free(this);
}