namespace XEngine
{

int Listen(const char *ip, int port, int backlog);
int Connect(const char *ip, int port);
int SetCloseOnExec(int fd);
int SetBlock(int fd, int block, int *oldflag);

}   // namespace XEngine