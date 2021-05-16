#include "head.h"
#include "handler.h"
#include "dataStructure.h"

#define PORT_NUMBER 8053
#define QUEUE_NUM 100

int main(int argc, char* argv[]) {

    // decode dnsIP
    assert(argc == 3);
    struct sockaddr_in dnsAddr;
    memset(&dnsAddr, 0, sizeof(dnsAddr));
    dnsAddr.sin_family = AF_INET;
    dnsAddr.sin_port = htons(strtol(argv[2], NULL, 10));
    inet_pton(AF_INET, argv[1], &(dnsAddr.sin_addr));

    // initialize listen socket
    FileDesc sockId = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockId >= 0);

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

    assert(bind(sockId, (struct sockaddr*)&servAddr, sizeof(servAddr)) >= 0);
    assert(listen(sockId, QUEUE_NUM) >= 0);

    // init data structures
    fd_set connectionSet;
    FD_ZERO(&connectionSet); // initialize the connectionSet for select
    FD_SET(sockId, &connectionSet); // add socket to the connectionSet

    ReadBuffs *readBuffs = newReadBuffs();
    QueAnsPairs *qaPairs = newQAPairs();

    // open log file
    FileDesc logFile = open("./dns_svr.log", O_WRONLY|O_CREAT|O_TRUNC);

    // start listening and processing
    while (TRUE) {
        fd_set readyConnections = connectionSet;
        select(FD_SETSIZE, &readyConnections, NULL, NULL, NULL);
        int i;
        for (i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &readyConnections)) {
                if (i == sockId) {
                    // this is a new connection
                    FileDesc connection = accept(i, NULL, NULL);
                    FD_SET(connection, &connectionSet);
                } else {
                    // this is a old connection
                    ReadBuff *result = handleRead(i, readBuffs);
                    if (result) {
                        handleResult(result, qaPairs, &connectionSet, dnsAddr, logFile);
                    }
                }
            }
        }
    }

    return 0;
}