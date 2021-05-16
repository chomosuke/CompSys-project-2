#ifndef PARSER
#define PARSER

#include "head.h"
#include "dataStructure.h"

#define DOMAIN_MAX_LEN 256 // 255 + \0

// for readability, so that the intented use of variable can be indicated by the type
typedef bool bit; // this should only be 1 or 0
typedef uint8_t bits; // could contain several bits, but not a full bytes

// represent an query parsed from packet
typedef struct {
    char qname[DOMAIN_MAX_LEN];
    uint16_t qtype;
    uint16_t qclass;
} Query;

// represent an answer parsed from packet
typedef struct {
    int iqname; // index of the question with the url
    uint16_t qtype;
    uint16_t qclass;
    uint32_t ttl;
    uint16_t rdlength;
    uint8_t *rdata;
} Answer;

// represend all information extracted from packet
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

    // data that could be cached
    // if it is to be cached, must be cached immediately to ensure expiry being correct
    int originalLen;
    int ittl; // offset of first ttl in original
    uint8_t *original;
} Info;
// parse data into a Info struct, data should be a ReadBuff that's finished reading.
// new Info will hold no pointer anything in data, so data can be safely destroyed.
Info *newInfo(ReadBuff *data);
// free all memories malloced to Info
void destroyInfo(Info *this);

#endif