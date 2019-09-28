#include <Python.h>
#include <stdio.h>
#include "EgnFunc.h"

static PyObject *
AddTimer(PyObject *self, PyObject *args)
{
    const char *command;
    int sts;

    if (!PyArg_ParseTuple(args, "s", &command))
        return NULL;
    sts = system(command);
    return PyLong_FromLong(sts);
}

static PyMethodDef PyEFuncMethods[] = {
	{"add_timer", (PyCFunction)AddTimer, METH_VARARGS, "pack from python obj to bytes" },
	{NULL}  /* Sentinel */
};

static struct PyModuleDef efunc_module = {
    PyModuleDef_HEAD_INIT,
    "efunc",   /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,       /* size of per-interpreter state of the module,
                 or -1 if the module keeps state in global variables. */
    PyEFuncMethods
};

PyMODINIT_FUNC
PyInit_efunc(void)
{
    printf("on init efunc\n");
    return PyModule_Create(&efunc_module);
}
