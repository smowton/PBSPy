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
#ifndef PBSPY_OPTIONTYPE___H
#define PBSPY_OPTIONTYPE___H

#ifdef __cplusplus
extern "C" {
#endif
#include <stddef.h> /* for offsetof */
#include <Python.h>

/*
** A generic option object
*/

typedef struct {
  PyObject_HEAD
  PyObject *_d;
  int settable_once; /* if attribute are settable only the first time */
} OptionObject;


staticforward PyTypeObject Option_Type;

#define  OptionType_Check(op)   PyObject_TypeCheck(op, &Option_Type)

staticforward int _setattr_int_const(PyObject *o,char *a, int code);
staticforward int _setattr_str_const(PyObject *o,char *a, char *s);

#ifdef __cplusplus
}
#endif

#endif /* !PBSPY_OPTIONTYPE___H */
 
