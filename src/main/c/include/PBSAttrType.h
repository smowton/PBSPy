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
#ifndef PBSPY_PBSATTRTYPE__H
#define PBSPY_PBSATTRTYPE__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* for offsetof */
#include <Python.h>
 
/* 
** PBSAttr_Type
**    For efficiency we store them as PyObject * instead of char *, so that
**    we can just use Py_INCREF when retrieving attrbibutes.
*/

typedef struct {
  PyObject_HEAD
  PyObject *name;      /* attribute name */
  PyObject *value;     /* value          */
  PyObject *rscn;      /* resource name  */
  PyObject *bop;       /* A BatchOp_Type instance */
} PBSAttrObject;

staticforward PyTypeObject PBSAttr_Type;

#define PBSAttrType_Check(op)        PyObject_TypeCheck(op, &PBSAttr_Type)

#ifdef __cplusplus
}
#endif

#endif /* !PBSPY_PBSATTRTYPE__H */
