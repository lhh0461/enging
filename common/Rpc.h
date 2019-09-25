#ifndef __RPC__
#define __RPC__

#include <Python.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <tinyxml2.h>

#include "Package.h"

using namespace tinyxml2;

namespace XEngine
{

typedef uint16_t RPC_PID;

enum eRpcType
{
    RPC_SERVER = 1,
    RPC_CLIENT = 2,
    RPC_HOST = 3,
};

enum eRpcFieldType
{
    RPC_INT = 1,
    RPC_UINT = 2,
    RPC_STRING = 3,
    RPC_BOOL = 4,
    RPC_PB = 5,
    RPC_FLOAT = 6,
};

enum eRpcDeamonType
{
    RPC_GAMDE = 1,
    RPC_GATED = 2,
    RPC_DBD = 3,
};

struct stRpcFunction
{
    eRpcType type;
    RPC_PID pid;
    std::string name;
    std::string module;
    eRpcDeamonType deamon;
    std::list<eRpcFieldType> args;
};

class CRpc
{
public:
    CRpc();
    ~CRpc();
    int Init(const std::string & cCfgPath);
    RPC_PID GetPidByName(const std::string & cFuncName);
    int Pack(RPC_PID pid, PyObject *obj, CPackage *package);
    PyObject *UnPack(RPC_PID pid, CPackage *package);
    int Dispatch(CPackage *package);
    int RpcCall(const CPackage *package);
private:
    stRpcFunction *GetFunctionById(RPC_PID pid);
    int PackField(eRpcFieldType field, PyObject *item, CPackage *package);
    PyObject *UnPackField(eRpcFieldType field, CPackage *package);
private:
    stRpcFunction *ParseFunc(XMLElement *elem, eRpcType type);
    void ParseSection(XMLElement *root, eRpcType type, const char *name, std::list<stRpcFunction *> &lRpcList);
    void ParseCfg(const std::string &cfg);
    eRpcFieldType GetArgTypeByName(const std::string & name);
    int CheckFieldType(int field_type, PyObject *item);
    int GetRpcCfgFiles(const char *basePath, std::vector<std::string> & filelist);
private:
    std::unordered_map<std::string, RPC_PID> m_Name2Pid;
    std::unordered_map<RPC_PID, stRpcFunction *> m_RpcTable;
};

}

#endif //__RPC__

