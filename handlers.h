#ifndef HANDLER
#define HANDLER

#include "head.h"
#include "dataStructures.h"

void handleNew(FileDesc connection, fd_set *connectionSet);

// return NULL if not finished reading, otherwise return the read data
ReadBuff *handleExisting(FileDesc connection, ReadBuffs *readBuffs);

void handleResult(ReadBuff *result, QueAnsPairs *qaPairs, fd_set *connectionSet);

#endif