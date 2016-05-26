#ifndef PBSPYMODULE___H
#define PBSPYMODULE___H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* for offsetof */
#include <Python.h>

#include <pbs_error.h>
#include <pbs_ifl.h>

#ifndef Py_RETURN_NONE
#define Py_RETURN_NONE return Py_INCREF(Py_None), Py_None
#endif                                /* Py_RETURN_NONE */

#ifndef Py_CLEAR
#define Py_CLEAR(op)                                            \
        do {                                                    \
                if (op) {                                       \
                        PyObject *tmp = (PyObject *)(op);       \
                        (op) = NULL;                            \
                        Py_DECREF(tmp);                         \
                }                                               \
        } while (0)
#endif                                                          /* Py_CLEAR */

#ifndef Py_True
static PyObject *Py_True;
#endif /* pre 2.3 */
#ifndef Py_False
static PyObject *Py_False;
#endif

/* Macros for returning Py_True or Py_False, respectively */
/*
 Copyright (C) 2007, Mohan Kannapareddy <kmoh.raj@gmail.com>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License. 
 You may obtain a copy of the License at 

  http://www.apache.org/licenses/LICENSE-2.0 

 Unless required by applicable law or agreed to in writing, software 
 distributed under the License is distributed on an "AS IS" BASIS, 
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 See the License for the specific language governing permissions and 
 limitations under the License.
*/

#ifndef Py_RETURN_TRUE
#define Py_RETURN_TRUE return Py_INCREF(Py_True), Py_True
#endif   /* Py_RETURN_TRUE */

#ifndef Py_RETURN_FALSE
#define Py_RETURN_FALSE return Py_INCREF(Py_False), Py_False
#endif   /* Py_RETURN_FALSE */

#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

#define IS_SEQUENCE_TYPE(o)   ( (PyList_Check(o)) || (PyTuple_Check(o)) )

#define STRING_BUFFER     8192

/* should we prefix with module name? */
#ifndef TYPEOPBJECT_MODULE_PREFIX
#define TYPEOPBJECT_MODULE_PREFIX     ""
#endif /* TYPEOPBJECT_MODULE_PREFIX */




#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define SUCCESS    1
#define FAILED     0


#define PBSPY_STR_BUFFER      2048


static PyObject *PBSPyExceptionObject;                        /* exception object */
static char NULLSTR[] = "";           /* NULL (empty) C String */
static char PBS_LOG_NAME[] = "PBS_None"; /* for the PBSLog default name */
static PyObject *PBS_LOG_NONE;         /* a Py string of above */
static PyObject *PBS_NULLSTR;         /* PBS NULLSTR constants */
static PyObject *PBS_UNKNOWN_HOST;    /* "unknown host */
static PyObject *PBS_MOM_REPR_UNK;    /* generic mom connection message */

#ifdef __cplusplus
}
#endif

#endif /* !PBSPYMODULE___H */
