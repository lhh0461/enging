#include <Python.h>
#include <stdio.h>

#include "XEngine.h"
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
    return PyEntityObj_New(entity_name);
}

static PyObject *ICreateEntityAnyWhere(PyObject *self, PyObject *args)
{
    const char *entity_name;

    if (!PyArg_ParseTuple(args, "s", &entity_name))
        return NULL;
    
    new CPackage()
    Rpc->RpcCall()
    return Py_RETURN_NONE;
}

static PyObject *ICreateEntityRemote(PyObject *self, PyObject *args)
{
    const char *entity_name;

    if (!PyArg_ParseTuple(args, "s", &entity_name))
        return NULL;

    Rpc->RpcCall()
    return Py_RETURN_NONE;
}

static PyObject *IDestroyEntity(PyObject *self, PyObject *args)
{
    const char *entity_name;

    if (!PyArg_ParseTuple(args, "s", &entity_name))
        return NULL;
    return PyEntityObj_New(entity_name);
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
    Py_RETURN_NONE;
    */
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

static PyMethodDef PyXEngineMethods[] = {
    {"add_timer", (PyCFunction)IAddTimer, METH_VARARGS, "pack from python obj to bytes" },
    {"del_timer", (PyCFunction)IDelTimer, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity", (PyCFunction)ICreateEntity, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity_anywhere", (PyCFunction)ICreateEntityAnyWhere, METH_VARARGS, "pack from python obj to bytes" },
    {"create_entity_remote", (PyCFunction)ICreateEntityRemote, METH_VARARGS, "pack from python obj to bytes" },
    {"register_global_service", (PyCFunction)IRegisterGlobalService, METH_VARARGS, "pack from python obj to bytes" },
    {"destroy_entity", (PyCFunction)IDestroyEntity, METH_VARARGS, "pack from python obj to bytes" },
    {"connect_to_server", (PyCFunction)IConnectToServer, METH_VARARGS, "pack from python obj to bytes" },
    {"rpc_call", (PyCFunction)IDestroyEntity, METH_VARARGS, "pack from python obj to bytes" },
    {NULL}  /* Sentinel */
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
    printf("on init efunc\n");
    return PyModule_Create(&XEngine_module);
}
