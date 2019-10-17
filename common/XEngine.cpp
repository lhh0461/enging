#include <Python.h>
#include <stdio.h>
#include "XEngine.h"
#if 0

#include "EntityMgr.h"
#include "Rpc.h"

static PyObject * IAddTimer(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return PyLong_FromLong(sts);
}

static PyObject * IDelTimer(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return PyLong_FromLong(sts);
}

static PyObject *ICreateEntity(PyObject *self, PyObject *args)
{
    const char *entity_name;

    if (!PyArg_ParseTuple(args, "s", &entity_name))
        return NULL;
    ENTITY_ID entity_id = GenEntityId();
    CEntityMgr *entity_mgr = g_Server->GetEntityMgr();
    ENTITY_TYPE entity_type = entity_mgr->GetEntityTypeByName(entity_name);

    PyObject *entity_obj = PyEntityObj_New(entity_type, entity_id);
    if (entity_obj) {
        g_EntityMgr->AddEntity(entity_id, entity_obj);
        return entity_obj;
    }

    Py_RETURN_NONE;
}

static PyObject *ICreateEntityFromDB(PyObject *self, PyObject *args)
{
    const char *entity_name;
    const char *entity_key;

    if (!PyArg_ParseTuple(args, "ss", &entity_name, &entity_key))
        return NULL;

    ENTITY_ID entity_id = GenEntityId();
    CEntityMgr *entity_mgr = g_Server->GetEntityMgr();
    ENTITY_TYPE entity_type = entity_mgr->GetEntityTypeByName(entity_name);

    std::list<CConnState *> conn_list;
    g_Server->GetServerConnByType(SERVER_TYPE_DBD, conn_list);
    if (conn_list.size() > 0) {
        CConnState *conn = g_Server->GetServerConnById(conn_list[0]);
        if (conn) {
            CPackage *pack = new CPackage();
            pack->PackCmd(MSG_CMD_CREATE_ENTIRY_FROM_DB);
            pack->PackInt(entity_type);
            pack->PackString(entity_key);
            conn->PushSendPackList(pack);
        }
    }

    Py_RETURN_NONE;
}

static PyObject *ICreateEntityAnyWhere(PyObject *self, PyObject *args)
{
    const char *entity_name;

    if (!PyArg_ParseTuple(args, "s", &entity_name))
        return NULL;

    CEntityMgr *entity_mgr = g_Server->GetEntityMgr();
    ENTITY_TYPE entity_type = entity_mgr->GetEntityTypeByName(entity_name);

    CConnState *conn = g_Server->GetServerConnById(CENTERD_ID);
    if (conn) {
        CPackage *pack = new CPackage();
        pack->PackCmd(MSG_CMD_CREATE_ENTIRY_ANYWHERE);
        pack->PackInt(entity_type);
        pack->PackString(entity_key);
        conn->PushSendPackList(pack);
    }

    Py_RETURN_NONE;
}

static PyObject *ICreateEntityFromDBAnyWhere(PyObject *self, PyObject *args)
{
    const char *entity_name;
    const char *entity_key;

    if (!PyArg_ParseTuple(args, "ss", &entity_name, &entity_key))
        return NULL;
    CEntityMgr *entity_mgr = g_Server->GetEntityMgr();
    ENTITY_TYPE entity_type = entity_mgr->GetEntityTypeByName(entity_name);

    CConnState *conn = g_Server->GetServerConnById(CENTERD_ID);
    if (conn) {
        CPackage *pack = new CPackage();
        pack->PackCmd(MSG_CMD_CREATE_ENTIRY_FROM_DB_ANYWHERE);
        pack->PackInt(entity_type);
        pack->PackString(entity_key);
        conn->PushSendPackList(pack);
    }

    Py_RETURN_NONE;
}

static PyObject *ICreateEntityRemote(PyObject *self, PyObject *args)
{
    const char *entity_name;

    if (!PyArg_ParseTuple(args, "s", &entity_name))
        return NULL;
    
    //Rpc->RpcCall()
    Py_RETURN_NONE;
}

static PyObject *IDestroyEntity(PyObject *self, PyObject *args)
{
    const char *entity_name;

    if (!PyArg_ParseTuple(args, "s", &entity_name))
        return NULL;
    //return PyEntityObj_New(entity_name);
    Py_RETURN_NONE;
}

static PyObject *IRpcCall(PyObject *self, PyObject *args)
{
/*
    PyObject *obj;

    if (!PyArg_ParseTuple(args, "O", &obj))
        return NULL;

    if (!PyBytes_CheckExact(obj)) {
        PyErr_Format(PyExc_TypeError, "pack obj type error type=%s", Py_TYPE(obj)->tp_name);
        return NULL;
    }   

    const char *str = PyBytes_AsString(obj);
    Py_ssize_t size = PyBytes_Size(obj);

    int res = g_Rpc->Dispatch(str, size);
    if (res < 0) {
        PyErr_SetString(RpcError, "rpc dispatch fail");
        return NULL;
    }   
    */
    Py_RETURN_NONE;
}

static PyObject *IConnectToServer(PyObject *self, PyObject *args)
{
    int server_type;
    const char *ip;
    int port;
    if (!PyArg_ParseTuple(args, "isi", &server_type, &ip, &port))
        return NULL;

    Py_RETURN_NONE;
}
#endif

static PyMethodDef PyXEngineMethods[] = {
    /*
    {"add_timer", (PyCFunction)IAddTimer, METH_VARARGS, "pack from python obj to bytes" },
    {"del_timer", (PyCFunction)IDelTimer, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity", (PyCFunction)ICreateEntity, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity_anywhere", (PyCFunction)ICreateEntityAnyWhere, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity_from_db", (PyCFunction)ICreateEntityFromDB, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity_from_db_anywhere", (PyCFunction)ICreateEntityFromDBAnyWhere, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity_remote", (PyCFunction)ICreateEntityRemote, METH_VARARGS, "pack from python obj to bytes" },
    //{"register_global_service", (PyCFunction)IRegisterGlobalService, METH_VARARGS, "pack from python obj to bytes" },
    {"destroy_entity", (PyCFunction)IDestroyEntity, METH_VARARGS, "pack from python obj to bytes" },
    {"connect_to_server", (PyCFunction)IConnectToServer, METH_VARARGS, "pack from python obj to bytes" },
    {"rpc_call", (PyCFunction)IDestroyEntity, METH_VARARGS, "pack from python obj to bytes" },
    */
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef XEngine_module = {
    PyModuleDef_HEAD_INIT,
    "XEngine",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    PyXEngineMethods
};

PyMODINIT_FUNC
PyInit_XEngine(void)
{
    printf("on init XEngine\n");
    return PyModule_Create(&XEngine_module);
}

