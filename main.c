#include "head.h"
#include "handlers.h"

#define PORT_NUMBER 8053
#define QUEUE_NUM 100

void toIPv4(char* str, int* dest);

int main(int argc, char* argv[]) {

    // decode dnsIP
    assert(argc == 3);
    int dnsIP[4];
    toIPv4(argv[1], dnsIP);
    int dnsPort = strtol(argv[2], NULL, 10);
    
    FileDesc sockId = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockId != -1);

    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT_NUMBER);

    int enable = 1;
    if (setsockopt(sockId, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    bind(sockId, (struct sockaddr*)&servAddr, sizeof(servAddr));
    listen(sockId, QUEUE_NUM);

    fd_set connectionSet;
    FD_ZERO(&connectionSet); // initialize the connectionSet for select
    FD_SET(sockId, &connectionSet); // add socket to the connectionSet

    ReadBuffs *readBuffs = newReadBuffs();
    QueAnsPairs *qaPairs = newQueAnsPairs();

    while (TRUE) {
        fd_set readyConnections = connectionSet;
        select(FD_SETSIZE, &readyConnections, NULL, NULL, NULL);

        int i;
        for (i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &readyConnections)) {
                if (i == sockId) {
                    // this is a new connection
                    FileDesc connection = accept(i, NULL, NULL);
                    handleNew(connection, &connectionSet);
                } else {
                    // this is a old connection
                    ReadBuff *result = handleExisting(i, readBuffs);
                    if (result) {
                        handleResult(result, &connectionSet, &qaPairs);
                    }
                }
            }
        }
    }

    return 0;
}

void toIPv4(char* str, int* dest) {
    int i, j = 0;
    for (i = 0; i < 4; i++) {
        while (str[j] != '.') {
            dest[i] *= 10;
            dest[i] += str[j] - '0';
            j++;
        }
        j++;
    }
}