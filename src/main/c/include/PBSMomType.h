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
#ifndef PBSPY_MOMTYPE___H
#define PBSPY_MOMTYPE___H

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h> /* for offsetof */
#include <Python.h>
#include <pbs_ifl.h>
#include <rm.h>

/* 
** PBSRM_Type
*/
typedef struct {
  PyObject_HEAD
  int   con_hdl;                /* connection handle */
  char *c_host;                 /* connection data */
  int  port;                    /* port */
  int fullresp;                 /* whether we need full response */
} PBSMomObject;

staticforward PyTypeObject PBSMom_Type;

#define PBSMomType_Check(op)        PyObject_TypeCheck(op, &PBSMom_Type)



#ifdef __cplusplus
}
#endif

#endif /* !PBSPY_MOMTYPE___H */
 
