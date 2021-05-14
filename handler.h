#ifndef HANDLER
#define HANDLER

#include "head.h"

void handleNew(FileDesc connection, fd_set *connectionSet);
ReadBuff *handleExisting(FileDesc connection, ReadBuffs *readBuffs);
void *handleResult(FileDesc connection, ReadBuff *result, QueAnsPairs *qaPairs, fd_set *connectionSet);

typedef struct {
    FileDesc sourceConnection;
    int length;
    int readLength;
    char *bytes;
} ReadBuff;
ReadBuff *newReadBuff(FileDesc sourceConnection, int length);
void read(ReadBuff *readBuff);
bool isFinished();
void destroyReadBuff(ReadBuff *readResult);

typedef struct {
    int length;
    int size;
    ReadBuff **buffs;
} ReadBuffs;
ReadBuffs *newReadBuffs();
ReadBuff *findBuff(ReadBuffs *readBuffs, FileDesc connection);
void addBuff(ReadBuffs *readBuffs, ReadBuff *readBuff);
void destroyReadBuffs(ReadBuffs *readBuffs);

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
void addQA(FileDesc queryConnection, FileDesc answerConnection);
FileDesc findAns(FileDesc queryConnection);
void removeQA(FileDesc queryConnection);
void destroyQAPairs(QueAnsPairs *qaPairs);

#endif