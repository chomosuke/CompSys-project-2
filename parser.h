#ifndef PARSER
#define PARSER

#include "head.h"
#include "dataStructure.h"

#define DOMAIN_MAX_LEN 256 // 255 + \0

typedef bool bit;
typedef uint8_t bits;

typedef struct {
    char qname[DOMAIN_MAX_LEN];
    uint16_t qtype;
    uint16_t qclass;
} Query;

typedef struct {
    char iqname; // index of the question with the url
    uint16_t qtype;
    uint16_t qclass;
    uint32_t ttl;
    uint16_t rdlength;
    uint8_t *rdata;
} Answer;

typedef struct {
    uint16_t id;
    bit qr;
    bits opcode;
    bit aa;
    bit tc;
    bit rd;
    bit ra;
    bits z;
    bits rcode;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
    Query *querys;
    Answer *answers;
} Info;
Info *newInfo(ReadBuff *data);
void destroyInfo(Info *this);


#endif