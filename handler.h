#ifndef HANDLER
#define HANDLER

#include "head.h"
#include "dataStructure.h"
#include "parser.h"
#include "cache.h"

#define TIME_LEN 25

// 2 domain name + a time stamp and " relpacing  by "
#define MAX_LOG_LEN (TIME_LEN + DOMAIN_MAX_LEN * 2 + 15)

// write the current time into buff, buff should have a length of at least TIME_LEN
void getTimeStamp(char *buff);
// when a connection is ready to read
// if finished reading within this function, pop and return the ReadBuff that contain what's been read
// if not finished reading return NULL
ReadBuff *handleRead(FileDesc connection, ReadBuffs *readBuffs);
// handle a ReadBuff that's finished reading, do all the neccesary parsing and logging and requesting,
// might modify connection, cache, qaPairs or close connections
void handleResult(ReadBuff *result, QueAnsPairs *qaPairs, fd_set *connectionSet, struct sockaddr_in dnsAddr, Cache *cache, FileDesc logFile);

#endif