#ifndef HEAD
#define HEAD

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#define AAAA 28

#define CACHE
#define NONBLOCKING

#define TRUE 1 == 1
#define FALSE 1 != 1

// for readability
typedef int FileDesc;
typedef int bool;

/** 
 * About ownership for every struct:
 * 
 * Each struct should own what pointer in it points to and free them in deconstructor.
 * 
 * If need to copying a struct that's referenced in one struct to another struct, do a
 * deep copy.
 */

#endif