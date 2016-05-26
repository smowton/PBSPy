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

#include "../include/pbsmodule.h"

#include "../include/utils.h"
#include "../include/OptionType.h"
#include "../include/PBSBatchOpType.h"
#include "../include/PBSAttrType.h"
#include "../include/PBSDataType.h"
#include "../include/PBSServerType.h"

#ifdef PBSPY_ENABLE_MOM
#include "../include/PBSMomType.h"
#endif

#ifdef PBSPY_ENABLE_LOG
#include "../include/PBSLogType.h"
#endif

/* EXCEPTION HELPER */
static char unk_err[] = "Weird, I should not be displaying this";
static void
SET_PBS_EXCEPTION(int connect,int err_no,char *add_str)
{
  PyObject *exc = (PyObject *)NULL;
  static char msg[PBSPY_STR_BUFFER+1];
  char *cp; /* pointer so we don;t copy unneccessarily */
  char *pbs_errmsg = (char *)NULL;

  if (connect != -1) { /* server error */
    pbs_errmsg = pbs_geterrmsg(connect);
    if ( !(pbs_errmsg) ) {
      pbs_errmsg = "No suitable message in the database";
    }
    if (add_str) {
      snprintf(msg,PBSPY_STR_BUFFER,"%s: %s",pbs_errmsg,add_str);
    } else {
      snprintf(msg,PBSPY_STR_BUFFER,"%s",pbs_errmsg);
    }
    cp = &msg[0];
  } else { /* ordinary error */
    if (add_str) {
      cp = add_str;
    } else {
      cp = unk_err;
    }
  } /* no connect */
  exc = Py_BuildValue("(i,s)",err_no,cp);
  if (exc) {
    PyErr_SetObject(PBSPyExceptionObject,exc);
    Py_DECREF(exc);
  }
}



/* --------------- INCLUDE UTILITY FILES ------------------- */

#include "../common/utils.c"

/* --------------- BEGIN INCLUDE ALL TYPES ------------------- */

#include "../objects/OptionType.c"
#include "../objects/PBSBatchOpType.c"
#include "../objects/PBSAttrType.c"
#include "../objects/PBSDataType.c"
#include "../objects/PBSServerType.c"

#ifdef PBSPY_ENABLE_MOM
#include "../objects/PBSMomType.c"
#endif

#ifdef PBSPY_ENABLE_LOG
#include "../objects/PBSLogType.c"
#endif

#include "../objects/PBSConstants.c"

/* --------------- END INCLUDE ALL TYPES ------------------- */

/* --------------- INCLUDE MODULE FUNCTIONS/METHODS FILE ------------------- */

static char _pbs_module_doc[]=
"Module providing a Python Interface to the C OpenPBS API\n\
\tIt will work with Torque and should work without much effort with PBSPro\n\
";

#include "moduleFunctions.c"


PyMODINIT_FUNC
MODULE_FUNCTION_NAME (void)
{
    PyObject *m;
    PyObject *d;


    m = Py_InitModule3(PBSPY_EXT_MODULE_NAME, 
                       pbsapi_methods,
                       _pbs_module_doc);

    if (m == NULL)
      return;

#ifndef Py_True
    /* initialize the PBS_NULLSTR */
    if ( !(Py_True = PyInt_FromLong(1L)) )
      return;
#endif
#ifndef Py_False
    /* initialize the PBS_NULLSTR */
    if ( !(Py_False = PyInt_FromLong(0L)) )
      return;
#endif

    /* Initialize the bop Array */
    if (init_bop_array() < 0)
      return;

    if (PyType_Ready(&Option_Type) < 0)
        return;

    if (PyType_Ready(&BatchOp_Type) < 0)
        return;

    if (PyType_Ready(&PBSAttr_Type) < 0)
        return;

    if (PyType_Ready(&PBSData_Type) < 0)
        return;

    if (PyType_Ready(&PBSServer_Type) < 0)
        return;

#ifdef PBSPY_ENABLE_MOM
    if (PyType_Ready(&PBSMom_Type) < 0)
        return;
#endif

#ifdef PBSPY_ENABLE_LOG
    if (PyType_Ready(&PBSLog_Type) < 0)
        return;
#endif

    /* initialize the PBS_NULLSTR */
    if ( !(PBS_NULLSTR = PyString_FromString(NULLSTR)) )
      return;
    /* Initialize Unknown host */
    if ( !(PBS_UNKNOWN_HOST = PyString_FromString("UNKNOWN")) )
      return;
    /* Initialize Unknown host */
    if ( !(PBS_MOM_REPR_UNK = PyString_FromFormat("not connected"
                                                  )) )
      return;

    /* initialize the PBS_LOG_NONE */
    if ( !(PBS_LOG_NONE = PyString_FromString(PBS_LOG_NAME)) )
      return;

    PBSPyExceptionObject = PyErr_NewException(PBSPY_EXT_MODULE_NAME  ".error",
                                            PyExc_EnvironmentError, 
                                            NULL);
    Py_INCREF(PBSPyExceptionObject); /* Below *steals* a reference */
    PyModule_AddObject(m, "PBSException", PBSPyExceptionObject);
    
	/* Add some symbolic constants to the module */
    /* this functions never fails!! */
	d   = PyModule_GetDict(m);
/*
    if ( (_insert_str_const(d,"__version__",PACKAGE_VERSION) < 0) ) 
      return;
*/

    /* Include the BatchOp_Type */
    Py_INCREF(&BatchOp_Type);
    PyModule_AddObject(m, "BatchOp", (PyObject *)&BatchOp_Type);

    /* Include the PBSAttr_Type */
    Py_INCREF(&PBSAttr_Type);
    PyModule_AddObject(m, "Attr", (PyObject *)&PBSAttr_Type);

    /* Include the PBSData_Type */
    Py_INCREF(&PBSData_Type);
    PyModule_AddObject(m, "Data", (PyObject *)&PBSData_Type);

    /* Include the PBSServer_Type */
    Py_INCREF(&PBSServer_Type);
    PyModule_AddObject(m, "Server", (PyObject *)&PBSServer_Type);

#ifdef PBSPY_ENABLE_MOM
    Py_INCREF(&PBSMom_Type);
    PyModule_AddObject(m, "Mom", (PyObject *)&PBSMom_Type);
#endif

#ifdef PBSPY_ENABLE_LOG
    Py_INCREF(&PBSLog_Type);
    PyModule_AddObject(m, "Log", (PyObject *)&PBSLog_Type);
#endif

    /* from PBSConstants.c (modobj.attr) */
    if ((insert_attr_flags(d) < 0))
      return;

    /* from PBSConstants.c (modobj.values) */
    if ((insert_value_flags(d) < 0))
      return;

    return;
}

