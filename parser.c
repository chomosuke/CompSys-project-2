#include "parser.h"

Info *newInfo(ReadBuff *data) {

    Info *this = malloc(sizeof(Info));

    // head
    memcpy(&this->id, data->bytes, sizeof(this->id));
    this->id = ntohs(this->id);
    uint16_t queryParam;
    memcpy(&queryParam, data->bytes + 2, sizeof(queryParam));
    this->qr = queryParam >> 15 & 1;
    this->opcode = queryParam >> 11 & 0xf;
    this->aa = queryParam >> 10 & 1;
    this->tc = queryParam >> 9 & 1;
    this->rd = queryParam >> 8 & 1;
    this->ra = queryParam >> 7 & 1;
    this->z = queryParam >> 4 & 7;
    this->rcode = queryParam & 0xf;
    memcpy(&this->qdcount, data->bytes + 4, sizeof(this->qdcount));
    this->qdcount = ntohs(this->qdcount);
    memcpy(&this->ancount, data->bytes + 6, sizeof(this->ancount));
    this->ancount = ntohs(this->ancount);
    memcpy(&this->nscount, data->bytes + 8, sizeof(this->nscount));
    this->nscount = ntohs(this->nscount);
    memcpy(&this->arcount, data->bytes + 10, sizeof(this->arcount));
    this->arcount = ntohs(this->arcount);

    // querys
    this->querys = malloc(this->qdcount * sizeof(Query));
    assert (this->querys != NULL);

    uint8_t **pQueryStarts = malloc(this->qdcount * sizeof(uint8_t*));
    assert (pQueryStarts != NULL);

    uint8_t *pBytes = data->bytes + 12; // increment every reads

    int iQuery;
    for (iQuery = 0; iQuery < this->qdcount; iQuery++) {
        pQueryStarts[iQuery] = pBytes;
        uint8_t subLen = *(pBytes++);
        char *pQname = this->querys[iQuery].qname; // increment every write
        while (subLen > 0) {
            uint8_t remLen = subLen;
            while (remLen > 0) {
                *(pQname++) = *(pBytes++);
                remLen--;
            }
            *(pQname++) = '.';
            subLen = *(pBytes++);
        }
        *(pQname - 1) = '\0'; // set last . to \0
        memcpy(&this->querys[iQuery].qtype, pBytes, sizeof(this->querys[iQuery].qtype));
        this->querys[iQuery].qtype = ntohs(this->querys[iQuery].qtype);
        pBytes += 2; // you know that 2 bytes are read
        memcpy(&this->querys[iQuery].qclass, pBytes, sizeof(this->querys[iQuery].qclass));
        this->querys[iQuery].qclass = ntohs(this->querys[iQuery].qclass);
        pBytes += 2; // you know that 2 bytes are read
    }

    // questions
    this->answers = malloc(this->ancount * sizeof(Answer));
    assert(this->answers != NULL);
    int iAnswers;
    for (iAnswers = 0; iAnswers < this->ancount; iAnswers++) {
        uint16_t offset;
        memcpy(&offset, pBytes, sizeof(offset));
        offset = ntohs(offset);
        offset = offset & 0x3fff; // get rid of the starting 1s
        uint8_t *pQStart = data->bytes + offset;
        pBytes += 2;

        // find iQname
        int iQname;
        for (iQname = 0; iQname < this->qdcount; iQname++) {
            if (pQStart == pQueryStarts[iQname]) {
                break;
            }
        }
        assert (iQname != this->qdcount);
        this->answers[iAnswers].iqname = iQname;
        
        memcpy(&this->answers[iAnswers].qtype, pBytes, sizeof(this->answers[iAnswers].qtype));
        this->answers[iAnswers].qtype = ntohs(this->answers[iAnswers].qtype);
        pBytes += 2; // you know that 2 bytes are read
        memcpy(&this->answers[iAnswers].qclass, pBytes, sizeof(this->answers[iAnswers].qclass));
        this->answers[iAnswers].qclass = ntohs(this->answers[iAnswers].qclass);
        pBytes += 2; // you know that 2 bytes are read
        memcpy(&this->answers[iAnswers].ttl, pBytes, sizeof(this->answers[iAnswers].ttl));
        this->answers[iAnswers].ttl = ntohl(this->answers[iAnswers].ttl);
        pBytes += 4; // 4 bytes are read
        memcpy(&this->answers[iAnswers].rdlength, pBytes, sizeof(this->answers[iAnswers].rdlength));
        this->answers[iAnswers].rdlength = ntohs(this->answers[iAnswers].rdlength);
        pBytes += 2; // 2 bytes are read

        this->answers[iAnswers].rdata = malloc(this->answers[iAnswers].rdlength * sizeof(uint8_t));
        assert(this->answers[iAnswers].rdata != NULL);
        memcpy(this->answers[iAnswers].rdata, pBytes, this->answers[iAnswers].rdlength * sizeof(uint8_t));
        pBytes += this->answers[iAnswers].rdlength;
    }

    return this;
}

void destroyInfo(Info *this) {
    int i;
    for (i = 0; i < this->qdcount; i++) {
        free(this->answers[i].rdata);
    }
    free(this->answers);
    free(this->querys);
    free(this);
}