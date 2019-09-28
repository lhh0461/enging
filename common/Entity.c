#include <Python.h>
#include "Entity.h"

typedef struct {
    PyObject_HEAD
    CEntity *entity_data;
} PyEntityObject;

static PyTypeObject PyEntity_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "entity.Entity",           /* tp_name */
    sizeof(PyEntityObject), /* tp_basicsize */
    0,                         /* tp_itemsize */
    0,                         /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_reserved */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash  */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "Entity objects",           /* tp_doc */
};

static PyModuleDef entity_module = {
    PyModuleDef_HEAD_INIT,
    "entity",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC
PyInit_entity(void)
{
    PyObject* m;

    PyEntity_Type.tp_new = PyType_GenericNew;
    if (PyType_Ready(&PyEntity_Type) < 0)
        return NULL;

    m = PyModule_Create(&entity_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyEntity_Type);
    PyModule_AddObject(m, "Entity", (PyObject *)&PyEntity_Type);
    return m;
}
