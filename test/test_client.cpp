#include "NetTool.h"
#include "Package.h"

///#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <string>

#include "Cmd.h"

using namespace XEngine;

int test_pack()
{
    CPackage *package = new CPackage();
    std::cout << "len=" << package->GetPkgLen() << std::endl;
    package->PackCmd((uint16_t)MSG_CMD_RPC);
    package->PackInt((uint16_t)1);
    package->PackString(std::string("hello"));

    CPackage *unpack = new CPackage(package->GetPkgBuf() + sizeof(CPackage::PKG_HEADER_TYPE), package->GetPkgDataLen());
    int rpc;
    package->UnPackInt(rpc);
    int arg;
    package->UnPackInt(arg);
    std::string cmd;
    unpack->UnPackString(cmd);
    std::cout << cmd << std::endl;
}

int test_cmd()
{
    int fd = Connect("127.0.0.1", 8888, 0);
    CPackage *package = new CPackage();
    package->PackCmd((uint16_t)MSG_CMD_RPC);
    package->PackInt(1); //pid
    package->PackInt(12); //parm
    package->PackBool(true);
    package->PackString(std::string("hello"));
    package->PackFloat(1.12345);
    int num = ::send(fd, package->GetPkgBuf(), package->GetPkgLen(), 0);
}

int main()
{
    test_cmd();
}
