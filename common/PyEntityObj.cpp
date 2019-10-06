#include <Python.h>
#include <structmember.h>

typedef struct {
    PyObject_HEAD
    int entity_id;
    int entity_type;
    PyDictObject *save_data;
    PyDictObject *temp_data;
} PyEntityObject;

static PyTypeObject PyEntity_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "entity.Entity",           /* tp_name */
    sizeof(PyEntityObject),    /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)Entity_dealloc,/* tp_dealloc */
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
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    Entity_methods,            /* tp_methods */
    Entity_members,            /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)Entity_init,      /* tp_init */
    0,                         /* tp_alloc */
    Entity_new,                 /* tp_new */
};

static PyModuleDef entity_module = {
    PyModuleDef_HEAD_INIT,
    "entity",
    "Example module that creates an extension type.",
    -1,
    NULL, NULL, NULL, NULL, NULL
};

static PyObject * Entity_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    char char *s;

    if (!PyArg_ParseTuple(args, "s", &s)) {
        return NULL;
    }

    PyEntityObj *self;
    self = (Noddy *)type->tp_alloc(type, 0);
    if (self != NULL) {
        self->save_data = PyDict_New();
        if (self->save_data == NULL) {
            Py_DECREF(self);
            return NULL;
        }

        self->temp_data = PyDict_New();
        if (self->temp_data == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }

    return (PyObject *)self;
}

static void Entity_dealloc(PyEntityObject* self)
{
    Py_DECREF(self->save_data);
    Py_DECREF(self->temp_data);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject * GetEntityId(PyObject *self, PyObject *unused)
{
    return PyLong_FromLong(self->entity_id);
}

static PyObject *GetEntityType(PyEntityObj *self, PyObject *unused)
{
    return PyLong_FromLong(self->entity_type);
}

static PyObject *GetEntitySave(PyEntityObj *self, PyObject *unused)
{
    return self->save_data;
}

static PyObject *GetEntityTemp(PyEntityObj *self, PyObject *unused)
{
    return self->temp_data;
}

static PyObject *Write2Db(PyObject *self, PyObject *args)
{
    PyObject *obj = self->save_data;
    //TODO 获取dbd的fd
    //TODO 序列化到buf
    SerializeEntity2String();
    
    //TODO 序列化
    return Py_RETURN_NONE;
}

static PyMethodDef Entity_methods[] = {
    {"get_entity_id", (PyCFunction)GetEntityId, METH_NOARGS,"Return the name, combining the first and last name"},
    {"get_entity_type", (PyCFunction)GetEntityType, METH_NOARGS,"Return the name, combining the first and last name"},
    {"get_entity_save", (PyCFunction)GetEntityType, METH_NOARGS,"Return the name, combining the first and last name"},
    {"get_entity_temp", (PyCFunction)GetEntityType, METH_NOARGS,"Return the name, combining the first and last name"},
    {"write2db", (PyCFunction)Write2Db, METH_NOARGS,"Return the name, combining the first and last name"},
    {NULL}  /* Sentinel */
};

static PyMemberDef Entity_members[] = {
    {NULL}  /* Sentinel */
};

PyMODINIT_FUNC
PyInit_entity(void)
{
    PyObject* m;

    if (PyType_Ready(&PyEntity_Type) < 0)
        return NULL;

    m = PyModule_Create(&entity_module);
    if (m == NULL)
        return NULL;

    Py_INCREF(&PyEntity_Type);
    PyModule_AddObject(m, "Entity", (PyObject *)&PyEntity_Type);
    return m;
}
