#include "NetTool.h"
#include ".h"

int main()
{
    int fd = Connect("127.0.0.1", 8888);
    CPackage *package = new CPackage();
    int num = send(fd, package->) 
}
