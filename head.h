#ifndef HEAD
#define HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define CACHE
#define NONBLOCKING

#define TRUE 1 == 1
#define FALSE 1 != 1

// for readability
typedef int FileDesc;
typedef int bool;

#endif