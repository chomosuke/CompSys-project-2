#include "handler.h"
#include "parser.h"
#include "cache.h"

#define TIME_LEN 25

// 2 domain name + a time stamp and " relpacing  by "
#define MAX_LOG_LEN (TIME_LEN + DOMAIN_MAX_LEN * 2 + 15)

void buffWrite(ReadBuff *buff, FileDesc connection);
void getTimeStamp(char *buff);

// return the read data if finished reading, otherwise return NULL
ReadBuff *handleRead(FileDesc connection, ReadBuffs *readBuffs) {
    ReadBuff *buff = findBuff(readBuffs, connection);
    if (buff == NULL) {
        // a new connection
        uint8_t lengthBuff[2];
        int readCount = read(connection, lengthBuff, 2);
        if (readCount == 1) {
            // just in case
            readCount += read(connection, lengthBuff + 1, 1);
        }
        assert(readCount == 2);
        uint16_t length;
        memcpy(&length, lengthBuff, sizeof(length));
        length = ntohs(length);
        addBuff(readBuffs, newReadBuff(connection, length));
    } else {
        buffRead(buff);
        if (isFinished(buff)) {
            return popBuff(readBuffs, connection);
        }
    }
    return NULL;
}

// this will parse the read data stream and do something about it.
// if it's a query, forward to the server and add entry to qaPairs
// if it's an answer, forward to client and remove entry to qaPairs
void handleResult(ReadBuff *result, QueAnsPairs *qaPairs, fd_set *connectionSet, struct sockaddr_in dnsAddr, Cache *cache, FileDesc logFile) {
    
    FileDesc connection = result->connection;
    
    Info *info = newInfo(result);

    char timeStamp[TIME_LEN];
    getTimeStamp(timeStamp);
    char logBuff[MAX_LOG_LEN];

    if (info->qr == 1) {
        // response

        addEntry(cache, info, logFile);

        if (info->ancount > 0 && info->answers[0].qtype == AAAA) {
            // log
            char addr[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, info->answers[0].rdata, addr, INET6_ADDRSTRLEN);
            sprintf(logBuff, "%s %s is at %s\n", timeStamp, info->querys[0].qname, addr);
            write(logFile, logBuff, strlen(logBuff));
        }

        FileDesc ansConn = findAns(qaPairs, connection);

        buffWrite(result, ansConn);

        // close and remove from connectionSet
        FD_CLR(connection, connectionSet);
        close(connection);
        
        FD_CLR(ansConn, connectionSet);
        close(ansConn);

        removeQA(qaPairs, connection);
    } else {
        // query

        // log
        sprintf(logBuff, "%s requested %s\n", timeStamp, info->querys[0].qname);
        write(logFile, logBuff, strlen(logBuff));

        CacheEntry *entry = findEntry(cache, info->querys[0]);
        if (entry != NULL) {
            // respond with cache

            if (entry->answer.qtype == AAAA) {
                // log
                char addr[INET6_ADDRSTRLEN];
                inet_ntop(AF_INET6, entry->answer.rdata, addr, INET6_ADDRSTRLEN);
                sprintf(logBuff, "%s %s is at %s\n", timeStamp, info->querys[0].qname, addr);
                write(logFile, logBuff, strlen(logBuff));
            }

            // replace the id:
            uint16_t id = htons(info->id);
            memcpy(entry->response, &id, sizeof(id));

            // overwrite the ttl:
            uint32_t ttl = entry->expiry - time(NULL);
            ttl = htonl(ttl);
            memcpy(&(entry->response[entry->ittl]), &ttl, sizeof(ttl));

            // write it to connection
            uint16_t len = htons(entry->responseLen);
            write(connection, &len, sizeof(len));
            write(connection, entry->response, entry->responseLen * sizeof(uint8_t));

            // close the connection
            FD_CLR(connection, connectionSet);
            close(connection);

        } else {
            // process
            if (info->querys[0].qtype == AAAA) {
                FileDesc dnsConn = socket(AF_INET, SOCK_STREAM, 0);
                int c = connect(dnsConn, (struct sockaddr*)&dnsAddr, sizeof(dnsAddr));
                if (c < 0) {
                    printf("%d\n", errno);
                    exit(1);
                }

                buffWrite(result, dnsConn);

                // add to connection set
                FD_SET(dnsConn, connectionSet);

                // add to qaPairs
                addQA(qaPairs, dnsConn, connection);
            } else {
                // log
                sprintf(logBuff, "%s unimplemented request\n", timeStamp);
                write(logFile, logBuff, strlen(logBuff));

                // cut off everything not in header
                result->length = 12;
                result->readLength = 12;

                // change rcode to 4
                result->bytes[3] &= 0xf0;
                result->bytes[3] |= 0x4;

                // set ra to 1
                result->bytes[3] |= 0x80;

                // change qr to respones
                result->bytes[2] |= 0x80;

                // set qd and an numbers etc to 0
                int i;
                for (i = 4; i < 12; i++) {
                    result->bytes[i] = 0;
                }

                buffWrite(result, connection);
                FD_CLR(connection, connectionSet);
                close(connection);
            }
        }
    }
    destroyReadBuff(result);
    destroyInfo(info);
}

void buffWrite(ReadBuff *buff, FileDesc connection) {
    // write the len first
    uint16_t len = htons(buff->length);
    write(connection, &len, sizeof(len));

    // write the rest of the message
    write(connection, buff->bytes, buff->length * sizeof(uint8_t));
}

void getTimeStamp(char *buff) {
    time_t t = time(NULL);
    struct tm *tInfo = gmtime(&t);
    strftime(buff, TIME_LEN, "%FT%T%z", tInfo);
}