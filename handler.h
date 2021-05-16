#ifndef HANDLER
#define HANDLER

#include "head.h"
#include "dataStructure.h"
#include "parser.h"

ReadBuff *handleRead(FileDesc connection, ReadBuffs *readBuffs);
void handleResult(ReadBuff *result, QueAnsPairs *qaPairs, fd_set *connectionSet, struct sockaddr_in dnsAddr, Cache *cache, FileDesc logFile);

#endif