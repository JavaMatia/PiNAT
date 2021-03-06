#include "pyCoreFunctions.hpp"

extern "C"
{

    PyObject* py_initCore(PyObject* self, PyObject* args)
    {
        PyObject* packPoolPointer = 0;
    
        if(!PyArg_ParseTuple(args, "O", &packPoolPointer)) {
            return NULL;
        }
    
        void* packetPointer = PyLong_AsVoidPtr(packPoolPointer);   
        pinat::initCore((pinat::PacketPool*)packetPointer);
        Py_RETURN_NONE;
    }   
    
    PyObject* py_getIPs(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
    
        if(!PyArg_ParseTuple(args, "k", &packetID)) {
            return NULL;
        }

        PyObject* ret = nullptr;
        std::vector<std::string>* vec = nullptr;

        try {
            vec = pinat::getIPs(packetID);
        } catch(std::exception& e) {
            delete vec;
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
        
        if (vec)
        {
            ret = Py_BuildValue("ss", vec->at(0).c_str(), vec->at(1).c_str());
            delete vec;
            return ret;
        }
        else
        {
            Py_RETURN_NONE;
        }
    }
    
    PyObject* py_getPorts(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
    
        if(!PyArg_ParseTuple(args, "k", &packetID)) {
            return NULL;
        }

        PyObject* ret = nullptr;
        std::vector<unsigned int>* vec = nullptr;

        try {
            vec = pinat::getPorts(packetID);
        } catch(std::exception& e) {
            delete vec;
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
        
        if (vec)
        {
            ret = Py_BuildValue("II", vec->at(0), vec->at(1));
            delete vec;
            return ret;
        }
        else
        {
            Py_RETURN_NONE;
        }
    }
    
    PyObject* py_getMACs(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
    
        if(!PyArg_ParseTuple(args, "k", &packetID)) {
            return NULL;
        }

        PyObject* ret = nullptr;
        std::vector<std::string>* vec = nullptr;

        try {
            vec = pinat::getMACs(packetID);
        } catch(std::exception& e) {
            delete vec;
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
        
        if (vec)
        {
            ret = Py_BuildValue("ss", vec->at(0).c_str(), vec->at(1).c_str());
            delete vec;
            return ret;
        }
        else
        {
            Py_RETURN_NONE;
        }
    }
    
    PyObject* py_checkType(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
        char* type;
    
        if(!PyArg_ParseTuple(args, "ks", &packetID, &type)) {
            return NULL;
        }
    
        try {
            if(pinat::checkType(packetID, type))
            {
                Py_RETURN_TRUE;
            }
            else
            {
                Py_RETURN_FALSE;
            }
        } catch(std::exception& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
    }
    
   PyObject* py_dropPacket(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
    
        if(!PyArg_ParseTuple(args, "k", &packetID)) {
            return NULL;
        }
    
        try {
            pinat::dropPacket(packetID);
        } catch(std::exception& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
    
        Py_RETURN_NONE;
    }

    PyObject* py_getArpInfo(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
    
        if(!PyArg_ParseTuple(args, "k", &packetID)) {
            return NULL;
        }

        PyObject* ret = nullptr;
        std::vector<std::string>* vec = nullptr;

        try {
            vec = pinat::getArpInfo(packetID);
        } catch(std::exception& e) {
            delete vec;
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
        
        if (vec)
        {
            ret = Py_BuildValue("ssss", vec->at(0).c_str(), vec->at(1).c_str(), vec->at(2).c_str(), vec->at(3).c_str());
            delete vec;
            return ret;
        }
        else
        {
            Py_RETURN_NONE;
        }
    }
    
    PyObject* py_getDNSInfo(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
        PyObject* addresses, *info;
        std::string dname;
        std::vector<std::string>* addressVector;
        std::map<std::string, std::vector<std::string>*>* dnsInfo = nullptr;
        
        if(!PyArg_ParseTuple(args, "k", &packetID)) {
            return NULL;
        }

        try {
            dnsInfo = pinat::getDNSInfo(packetID);
        } catch(std::exception& e) {
            delete dnsInfo;
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }


        if(dnsInfo)
        {
            //create the dictionary
            info = PyDict_New();

            //Iterate over the map:
            for (std::map<std::string, std::vector<std::string>*>::iterator it = dnsInfo->begin(); it != dnsInfo->end(); it++)
            {
                dname = it->first;
                addressVector = it->second;

                //create new list:
                addresses = PyList_New(addressVector->size());
                //Now, fill the pylist with the values inside tempAddesses
                for (unsigned int i=0; i<addressVector->size(); i++)
                    PyList_SetItem(addresses, i, PyUnicode_FromString(addressVector->at(i).c_str()));

                //FREE the allocated space for the addressVector
                delete addressVector;
                //insert vector + dname into the dictionary
                PyDict_SetItem(info, PyUnicode_FromString(dname.c_str()), addresses);
            }

            delete dnsInfo;
            return info;
        }
        else
        {
            Py_RETURN_NONE;
        }
    }

    PyObject* py_openDB(PyObject* self, PyObject* args)
    {
        char* path;
    
        if(!PyArg_ParseTuple(args, "s", &path)) {
            return NULL;
        }

        PyObject* ret = nullptr;
    
        try {
            ret = PyLong_FromVoidPtr((void*)pinat::openDB(path));
        } catch(std::exception& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }

        return ret;
    }

    PyObject* py_closeDB(PyObject* self, PyObject* args)
    {
        PyObject* pyPointer = nullptr;
    
        if(!PyArg_ParseTuple(args, "O", &pyPointer)) {
            return NULL;
        }
    
        void* dbPointer = PyLong_AsVoidPtr(pyPointer);

        try {
            pinat::closeDB((sqlite3*)dbPointer);
        } catch(std::exception& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
        
        Py_RETURN_NONE;
    }

    PyObject* py_execDB(PyObject* self, PyObject* args)
    {
        PyObject* pyPointer = nullptr;
        char* command;
    
        if(!PyArg_ParseTuple(args, "Os", &pyPointer, &command)) {
            return NULL;
        }
    
        void* dbPointer = PyLong_AsVoidPtr(pyPointer);

        try {
            pinat::execDB((sqlite3*)dbPointer, command);
        } catch(std::exception& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
        
        Py_RETURN_NONE;
    }

    PyObject* py_selectDB(PyObject* self, PyObject* args)
    {
        PyObject* pyPointer = nullptr;
        char* command;
    
        if(!PyArg_ParseTuple(args, "Os", &pyPointer, &command)) {
            return NULL;
        }
    
        void* dbPointer = PyLong_AsVoidPtr(pyPointer);
        std::vector<std::vector<std::string>*>* table = nullptr;
        std::vector<std::string>* row = nullptr;
        PyObject *pyTable, *pyRow;

        try {
            table = pinat::selectDB((sqlite3*)dbPointer, command);
        } catch(std::exception& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }

        if(table)
        {
            pyTable = PyList_New(table->size());
            for(unsigned int i=0; i<table->size(); i++)
            {
                row = table->at(i);
                pyRow = PyList_New(row->size());
                for(unsigned int j=0; j<row->size(); j++)
                    PyList_SetItem(pyRow, j, PyUnicode_FromString(row->at(j).c_str()));
                PyList_SetItem(pyTable, i, pyRow);
                
                delete row;
            }
            delete table;

            return pyTable;
        }
        else
        {
            Py_RETURN_NONE;
        }
    }

    PyObject* py_censorWords(PyObject* self, PyObject* args)
    {
        unsigned long packetID = 0;
        PyObject* listObj = nullptr, *tmp = nullptr;
        std::vector<std::string> blacklist;
        char* wordToReplace;

        if(!PyArg_ParseTuple(args, "ksO", &packetID, &wordToReplace, &listObj)) {
            return NULL;
        }

        for(unsigned int i = 0; i < PyList_Size(listObj); i++)
        {
            tmp = PyList_GetItem(listObj, i);
            std::wstring ws(PyUnicode_AS_UNICODE(tmp));
            blacklist.push_back(std::string(ws.begin(), ws.end()));
        }

        try {
            pinat::censorWords(packetID, wordToReplace, &blacklist);
        } catch(std::exception& e) {
            PyErr_SetString(PyExc_RuntimeError, e.what());
            return NULL;
        }
        
        Py_RETURN_NONE;
    }
}
