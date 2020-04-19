#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

#include <tinyxml2.h>

#include "CRpc.h"
#include "CPackage.h"
#include "Script.h"
#include "CLog.h"

using namespace std;
using namespace tinyxml2;

namespace XEngine
{

CRpc::CRpc()
{
}

CRpc::~CRpc()
{
    m_Name2Pid.clear();
    for (auto it = m_RpcTable.begin(); it != m_RpcTable.end(); it++) {
        if (it->second) {
            delete it->second;
        }
    }
    m_RpcTable.clear();
}

int CRpc::Init(const string & cCfgPath)
{
    ParseCfg(cCfgPath);
}

eRpcFieldType CRpc::GetArgTypeByName(const string & name)
{
    if (name == "RPC_INT") {
        return RPC_INT;
    } else if (name == "RPC_UINT") {
        return RPC_UINT;
    } else if (name == "RPC_STRING") {
        return RPC_STRING;
    } else if (name == "RPC_PB") {
        return RPC_PB;
    } else if (name == "RPC_FLOAT") {
        return RPC_FLOAT;
    } else if (name == "RPC_BOOL") {
        return RPC_BOOL;
    }
}

stRpcFunction * CRpc::ParseFunc(XMLElement *elem, eRpcType type)
{
    stRpcFunction *pFunction = new stRpcFunction();
    pFunction->type = type;

    XMLElement *field_elem;

    field_elem = elem->FirstChildElement("function");
    pFunction->name = string(field_elem->GetText());

    field_elem = elem->FirstChildElement("args");
    XMLElement *args_elem = field_elem->FirstChildElement("arg");
    pFunction->args.push_back(GetArgTypeByName(args_elem->GetText()));

    while ((args_elem = args_elem->NextSiblingElement("arg")) != NULL) {
        pFunction->args.push_back(GetArgTypeByName(args_elem->GetText()));
    }

    return pFunction;
}

void CRpc::ParseSection(XMLElement *root, eRpcType type, const char * name, std::list<stRpcFunction *> &rpc_list)
{
    XMLElement *elem = root->FirstChildElement(name);
    if (elem) {
        rpc_list.push_back(ParseFunc(elem, type));
        while ((elem = elem->NextSiblingElement(name)) != NULL) {
            rpc_list.push_back(ParseFunc(elem, type));
        }
    } else {
       //TODO 
    }
}

static bool _CompareFunc(const stRpcFunction *first, const stRpcFunction *second)
{
    return first->module < second->module && first->name < second->name;
}

int CRpc::GetRpcCfgFiles(const char *basePath, vector<string> & vecFilelist)
{
    std::vector<std::string> result;
    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr=readdir(dir)) != NULL)
    {
        if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
            continue;
        else if(ptr->d_type == DT_REG)    ///file
        {
            string filename = ptr->d_name;
            if (filename.rfind(".xml")==(filename.length()-strlen(".xml"))) {
                vecFilelist.push_back(std::string(basePath)+std::string(ptr->d_name));
            }
        }
    }
    closedir(dir);
}

void CRpc::ParseCfg(const string &cCfgPath)
{
    XMLDocument doc;
    std::list<stRpcFunction *> lRpcList;
    vector<string> vecCfgList;
    GetRpcCfgFiles(cCfgPath.c_str(), vecCfgList);

    for (auto it = vecCfgList.begin(); it != vecCfgList.end(); it++)
    {
        doc.LoadFile(it->c_str());
        XMLElement *root = doc.FirstChildElement("root");
        if (root) {
            ParseSection(root, RPC_SERVER, "server", lRpcList);
            ParseSection(root, RPC_CLIENT, "client", lRpcList);
            ParseSection(root, RPC_HOST, "host", lRpcList);
        } else {
            assert(0);
        }
    }

    lRpcList.sort(_CompareFunc);
    RPC_PID pid = 1;
    for (auto it : lRpcList) {
        m_Name2Pid.insert(make_pair(it->name, pid));
        it->pid = pid;
        m_RpcTable[pid] = it;
        pid++;
    }
}

inline stRpcFunction *CRpc::GetFunctionById(RPC_PID function_id)
{
    auto iter = m_RpcTable.find(function_id);
    if (iter != m_RpcTable.end()) {
        return iter->second;
    }
    return NULL;
}

RPC_PID CRpc::GetPidByName(const string & func_name)
{
    auto iter = m_Name2Pid.find(func_name);
    if (iter != m_Name2Pid.end()) {
        return iter->second;
    }
    return 0;
}

int CRpc::CheckFieldType(int field_type, PyObject *item)
{
    switch(field_type) {
        case RPC_INT:
            if (!PyLong_CheckExact(item)) return -1;
            break;
        case RPC_STRING:
            if (!PyUnicode_CheckExact(item)) return -1;
            break;
        case RPC_FLOAT:
            if (!PyFloat_CheckExact(item)) return -1;
            break;
        case RPC_PB:
            if (!PyBytes_CheckExact(item)) return -1;
            break;
        case RPC_BOOL:
            if (!PyBool_Check(item) && !PyLong_CheckExact(item)) return -1;
            break;
        default:
            fprintf(stderr, "check field unknown field type=%d\n", field_type);
            return -1;
    }
    return 0;
}

int CRpc::PackField(eRpcFieldType field, PyObject *item, CPackage *package)
{
    if (CheckFieldType(field, item) == -1) {
        fprintf(stderr, "pack field type error. expected_type=%d,type_name=%s\n", field, Py_TYPE(item)->tp_name);
        return -1;
    }

    switch(field) {
        case RPC_INT:
            package->PackInt64(PyLong_AsLong(item));
            break;
        case RPC_STRING: 
            {
                Py_ssize_t size; 
                const char *str = PyUnicode_AsUTF8AndSize(item, &size);
                package->PackString(str, size);
                break;
            }
        case RPC_FLOAT:
            package->PackFloat(PyFloat_AS_DOUBLE(item));
            break;
        case RPC_PB:
            {
                const char *str = PyBytes_AsString(item);
                Py_ssize_t size = PyBytes_Size(item);
                package->PackString(str, size);
                break;
            }
        case RPC_BOOL:
            if (PyBool_Check(item)) {
                package->PackBool(item == Py_True ? true : false);
            } else if (PyLong_CheckExact(item)) {
                package->PackBool(PyLong_AsLong(item) > 0 ? true : false);
            }
            break;
        default:
            fprintf(stderr, "pack field unknown field type=%d\n", field);
            return -1;
    }
    return 0;
}

//pack obj data to buf
int CRpc::Pack(RPC_PID pid, PyObject *obj, CPackage *package)
{
    if (!PyTuple_CheckExact(obj)) {
        fprintf(stderr, "expected pack obj is tuple\n");
        return -1; 
    }

    stRpcFunction * pFunction = this->GetFunctionById(pid);
    if (pFunction == NULL) {
        fprintf(stderr, "can't find pid function. pid=%d\n", pid);
        return -1;
    }

    if ((int)pFunction->args.size() != PyTuple_Size(obj)) {
        fprintf(stderr, "tuple obj size is not equal arg cnt. expected=%ld, size=%ld\n", pFunction->args.size(), PyTuple_Size(obj));
        return -1;
    }
    
    package->PackInt16(pid);
    int i = 0;
    for (auto iter : pFunction->args) {
        if (this->PackField(iter, PyTuple_GetItem(obj, i++), package) != 0) return -1;
    }

    return 0;
}
/*
namespace type {
    enum object_type {
        NIL                 = MSGPACK_OBJECT_NIL,
        BOOLEAN             = MSGPACK_OBJECT_BOOLEAN,
        POSITIVE_INTEGER    = MSGPACK_OBJECT_POSITIVE_INTEGER,
        NEGATIVE_INTEGER    = MSGPACK_OBJECT_NEGATIVE_INTEGER,
        FLOAT32             = MSGPACK_OBJECT_FLOAT32,
        FLOAT64             = MSGPACK_OBJECT_FLOAT64,
        FLOAT               = MSGPACK_OBJECT_FLOAT,
#if defined(MSGPACK_USE_LEGACY_NAME_AS_FLOAT)
        DOUBLE              = MSGPACK_DEPRECATED("please use FLOAT64 instead") MSGPACK_OBJECT_DOUBLE, // obsolete
#endif // MSGPACK_USE_LEGACY_NAME_AS_FLOAT
        STR                 = MSGPACK_OBJECT_STR,
        BIN                 = MSGPACK_OBJECT_BIN,
        ARRAY               = MSGPACK_OBJECT_ARRAY,
        MAP                 = MSGPACK_OBJECT_MAP,
        EXT                 = MSGPACK_OBJECT_EXT
    };
}
*/

//return obj 正常
//return null 异常
PyObject *CRpc::UnPackField(eRpcFieldType field, CPackage *package)
{
    switch(field) {
        case RPC_INT:
            {
                int32_t val;
                package->UnPackInt32(val);
                if (package->GetErrCode() > 0) {
                    return NULL;
                }
                return PyLong_FromLong(val);
            }
        case RPC_UINT:
            {
                uint32_t val;
                package->UnPackUInt32(val);
                if (package->GetErrCode() > 0) {
                    return NULL;
                }
                return PyLong_FromLong(val);
            }
        case RPC_STRING: 
            {
                std::string val;
                package->UnPackString(val);
                if (package->GetErrCode() > 0) {
                    return NULL;
                }
                return PyUnicode_FromStringAndSize(val.c_str(), val.size());
            }
        case RPC_PB:
            {
                std::string val;
                package->UnPackString(val);
                if (package->GetErrCode() > 0) {
                    return NULL;
                }
                return PyBytes_FromStringAndSize(val.c_str(), val.size());
            }
        case RPC_FLOAT:
            {
                float val;
                package->UnPackFloat(val);
                if (package->GetErrCode() > 0) {
                    return NULL;
                }
                return PyFloat_FromDouble(val);
            }
        case RPC_BOOL:
            {
                bool val;
                package->UnPackBool(val);
                if (package->GetErrCode() > 0) {
                    return NULL;
                }
                return PyBool_FromLong(val);
            }
        default:
            assert(0);
    }
}

//unpack buf to obj data
PyObject * CRpc::UnPack(RPC_PID pid, CPackage *package)
{
    PyObject *obj;
    PyObject *field;

    fprintf(stderr, "Unpack. pid=%d\n", pid);

    const stRpcFunction *pFunction = GetFunctionById(pid);
    obj = PyTuple_New(pFunction->args.size());
    if (obj == NULL) {
        fprintf(stderr, "new unpack tuple fail. pid=%d\n", pid);
        return NULL;
    }
    fprintf(stderr, "new unpack tuple size=%d. pid=%d\n",pFunction->args.size(), pid);

    int i = 0;
    for (auto iter : pFunction->args) {
        if ((field = UnPackField(iter, package)) == NULL) {
            Py_DECREF(obj);
            return NULL;
        }
        PyTuple_SetItem(obj, i++, field);
    }
    return obj;
}

//除了包意外事件还有其他数据
int CRpc::Dispatch(CConnState *pConnState, CPackage *pPackage)
{
    PyObject *obj;

    RPC_PID pid;
    pPackage->UnPackUInt16(pid);
    if (pPackage->GetErrCode() > 0) {
        return -1;
    }

    LOG_DEBUG("Dispatch. pid=%d\n", pid);
    const stRpcFunction *pFunction = GetFunctionById(pid);
    if (pFunction == NULL) {
        fprintf(stderr, "CRpc::Dispatch can't find pid function. pid=%d\n", pid);
        return -1;
    }

    obj = UnPack(pid, pPackage);
    if (obj == NULL) {
        fprintf(stderr, "CRpc::Dispatch unpack fail. pid=%d\n", pid);
        return -1;
    }

    if (CallScriptFunction(pFunction->module.c_str(), pFunction->name.c_str(), obj) < 0) {
        fprintf(stderr, "CRpc::Dispatch call script function fail. module=%s,function=%s\n", pFunction->module.c_str(), pFunction->name.c_str());
        Py_DECREF(obj);
        return -1;
    }

    Py_DECREF(obj);
    return 0;
}

}
