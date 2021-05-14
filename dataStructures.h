#ifndef DATASTRUCTURES
#define DATASTRUCTURES

#include "head.h"

typedef struct {
    FileDesc connection;
    int length;
    int readLength;
    char *bytes;
} ReadBuff;
ReadBuff *newReadBuff(FileDesc connection, int length);
void buffRead(ReadBuff *this);
bool isFinished(ReadBuff *this);
void destroyReadBuff(ReadBuff *this);

typedef struct {
    int length;
    int size;
    ReadBuff **buffs;
} ReadBuffs;
ReadBuffs *newReadBuffs();
ReadBuff *findBuff(ReadBuffs *this, FileDesc connection);
void addBuff(ReadBuffs *this, ReadBuff *readBuff);
ReadBuff *popBuff(ReadBuffs *this, FileDesc connection);
void destroyReadBuffs(ReadBuffs *this);

typedef struct {
    FileDesc queryConnection;
    FileDesc answerConnection;
} QueAnsPair;
typedef struct {
    int length;
    int size;
    QueAnsPair *pairs;
} QueAnsPairs;
QueAnsPairs *newQAPairs();
void addQA(QueAnsPairs *this, FileDesc queryConnection, FileDesc answerConnection);
FileDesc findAns(QueAnsPairs *this, FileDesc queryConnection);
void removeQA(QueAnsPairs *this, FileDesc queryConnection);
void destroyQAPairs(QueAnsPairs *this);

#endif