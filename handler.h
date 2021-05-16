#ifndef HANDLER
#define HANDLER

#include "head.h"
#include "dataStructure.h"
#include "parser.h"
#include "cache.h"

#define TIME_LEN 25

// 2 domain name + a time stamp and " relpacing  by "
#define MAX_LOG_LEN (TIME_LEN + DOMAIN_MAX_LEN * 2 + 15)

void getTimeStamp(char *buff);
ReadBuff *handleRead(FileDesc connection, ReadBuffs *readBuffs);
void handleResult(ReadBuff *result, QueAnsPairs *qaPairs, fd_set *connectionSet, struct sockaddr_in dnsAddr, Cache *cache, FileDesc logFile);

#endif