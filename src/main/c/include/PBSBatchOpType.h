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
#ifndef PBSPY_BATCHOPTYPE___H
#define PBSPY_BATCHOPTYPE___H

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h> /* for offsetof */
#include <Python.h>
#include <pbs_ifl.h>


/* BatchOp_Type */
typedef struct {
  PyObject_HEAD
  int _idx;         /* index in the BATCHOP_ARRAY */
} BatchOpObject;

staticforward PyTypeObject BatchOp_Type;

#define BatchOpType_Check(op)        PyObject_TypeCheck(op, &BatchOp_Type)

#ifdef __cplusplus
}
#endif

#endif /* !PBSPY_BATCHOPTYPE___H */
 
