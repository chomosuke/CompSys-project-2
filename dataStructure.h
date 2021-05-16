#ifndef DATASTRUCTURE
#define DATASTRUCTURE

#include "head.h"

// represent a single read buffer
typedef struct {
    FileDesc connection;
    int length;
    int readLength;
    uint8_t *bytes;
} ReadBuff;
// construct a ReadBuff
ReadBuff *newReadBuff(FileDesc connection, int length);
// read from connection into the buffer
void buffRead(ReadBuff *this);
// return true if readLength == length
bool isFinished(ReadBuff *this);
// free all memory that's been malloced to this ReadBuff 
void destroyReadBuff(ReadBuff *this);

// represent an array of ReadBuff
typedef struct {
    int length;
    int size;
    ReadBuff **buffs;
} ReadBuffs;
// construct a ReadBuffs
ReadBuffs *newReadBuffs();
// find the ReadBuff with connection, if not found return NULL
ReadBuff *findBuff(ReadBuffs *this, FileDesc connection);
// add a ReadBuff to this ReadBuffs
void addBuff(ReadBuffs *this, ReadBuff *readBuff);
// find the ReadBuff with connection and return and remove it from ReadBuffs
ReadBuff *popBuff(ReadBuffs *this, FileDesc connection);
// free all memory that's been malloced to this ReadBuffs and to ReadBuff's in its array
void destroyReadBuffs(ReadBuffs *this);

// reprensent a pair of queryConnection (to dns server) and answerConnection (to client)
typedef struct {
    FileDesc queryConnection;
    FileDesc answerConnection;
} QueAnsPair;
// represent a list of QueAnsPair
typedef struct {
    int length;
    int size;
    QueAnsPair *pairs;
} QueAnsPairs;
// construct a QueAnsPairs
QueAnsPairs *newQAPairs();
// add an entry to QueAnsPairs
void addQA(QueAnsPairs *this, FileDesc queryConnection, FileDesc answerConnection);
// find answerConnection with queryConnection
FileDesc findAns(QueAnsPairs *this, FileDesc queryConnection);
// remove a QueAnsPair with the corrisponding queryConnection from QueAnsPairs
void removeQA(QueAnsPairs *this, FileDesc queryConnection);
// free all memory that's been malloced to this QueAnsPairs
void destroyQAPairs(QueAnsPairs *this);

#endif