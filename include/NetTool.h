#include <string>

namespace XEngine
{

int Listen(const char *ip, int port, int backlog);
int Connect(const char *ip, int port, int block = 1);
int SetCloseOnExec(int fd);
int SetBlock(int fd, int block, int *oldflag);
int Accept(int listen_fd, std::string &ip, int &port);
void Daemonize(void);

}
