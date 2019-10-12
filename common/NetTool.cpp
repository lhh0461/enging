#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <string>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "NetTool.h"

namespace XEngine
{


int Listen(const char *ip, int port, int backlog)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);
    if (0 < bind(fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) {
        return -1;
    }

    if (listen(fd, backlog) < 0) {
        return -1;
    }
    
    return fd;
}

int Connect(const char *ip, int port, int block)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);

    SetCloseOnExec(fd);
    if (0 > connect(fd, (struct sockaddr*)&addr, sizeof(addr))) {
        if (!block && errno == EINPROGRESS) {
            return fd; 
        }   
        perror("fs net connect");
        close(fd);
        return -1; 
    }   

    return fd;
}

int Accept(int listen_fd, std::string &ip, int &port)
{
    struct sockaddr_in addr;
    socklen_t addrlen;

    int conn_fd = accept(listen_fd, (struct sockaddr *) &addr, &addrlen);
    if (conn_fd < 0) {
        perror("fs net connect");
        return -1;
    }

    ip = inet_ntoa(addr.sin_addr);
    port = addr.sin_port;

    return conn_fd;
}

int SetCloseOnExec(int fd)
{
    int flag = fcntl(fd, F_GETFD, 0);
    int result = fcntl(fd, F_SETFD, flag | FD_CLOEXEC);
    return result;
}

int SetBlock(int fd, int block, int *oldflag)
{
    int flag = fcntl(fd, F_GETFL, 0); 
    if (oldflag != NULL) {
        *oldflag = flag;
    }   
    return fcntl(fd, F_SETFL, block == 0 ? flag | O_NONBLOCK : flag & (~O_NONBLOCK));
}

}
