#include <Python.h>
#include <string>
#include "pySniffer.hpp"
#include "pyCoreFunctions.hpp"


PyMethodDef core_methods[] = {
    {"init_core", (PyCFunction)py_initCore, METH_VARARGS, "Initializes the core function system"},
    {"get_src_ip", (PyCFunction)py_getSrcIp, METH_VARARGS, "Gets the src IP of the specified packet"},
    {"get_dst_ip", (PyCFunction)py_getDstIp, METH_VARARGS, "Gets the dst IP of the specified packet"},
    {"get_src_port", (PyCFunction)py_getSport, METH_VARARGS, "Gets the src port of the specified packet"},
    {"get_dst_port", (PyCFunction)py_getDport, METH_VARARGS, "Gets the dst port of the specified packet"},
    {"get_src_mac", (PyCFunction)py_getSrcMac, METH_VARARGS, "Gets the src Mac of the specified packet"},
    {"get_dst_mac", (PyCFunction)py_getDstMac, METH_VARARGS, "Gets the dst Mac of the specified packet"},
    {"check_type", (PyCFunction)py_getDstMac, METH_VARARGS, "Checks the type of the specified packet"},
    {"drop_packet", (PyCFunction)py_dropPacket, METH_VARARGS, "Drops the specified packet"},
    {NULL}  /* Sentinel */
};


/*
module information.
*/
static struct PyModuleDef pynatModule = {
    PyModuleDef_HEAD_INIT,
    "pynat",
    "Python PiNAT library",
    -1,
    core_methods, 
    NULL, NULL, NULL, NULL
};

/*
python module init function.
returns the new module.
*/
PyMODINIT_FUNC PyInit_pynat(void) {
    PyObject* module = PyModule_Create(&pynatModule);
    if(module == NULL)
        return NULL;

    if(Sniffer_InitType(module) != 0) //initializing our created object
        return NULL;

    return module;
}