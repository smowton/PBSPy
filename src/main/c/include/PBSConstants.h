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

#ifndef PBSPY_CONSTANTS___H
#define PBSPY_CONSTANTS___H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* for offsetof */
#include <Python.h>

/*
** A Generic Arg structure for boolean arguments
*/
typedef struct {
  int defined;  /* whether argument was passed */
  int value;    /* argument value */
  char *name;   /* root name for any error message during conversion */
} BoolArg;

/*
** A Generic Arg structure for complex string arguments
*/

typedef struct _CStringArg {
  int defined;  /* whether value was None */
  PyObject *arg; /* The actual python object */
  char *value;    /* argument value */
  char *name;   /* root name for any error message during conversion */
} CStringArg;


/*
** functions
*/
staticforward int set_dict_string(PyObject *,char *,const char *);
staticforward PyObject *argv_count_to_sequence(char **argv,int count);
staticforward PyObject *argv_to_sequence(char **argv);
staticforward int sequence_to_argv(PyObject *arglist,int *argc_p,char ***argv_p,int add_null);
staticforward int _insert_str_const(PyObject *d, char *name, char *code);
staticforward int _insert_int_const(PyObject *d, char *name,int code);
staticforward int _insert_long_const(PyObject *d, char *name,long code);
staticforward int convert_to_boolean_int(PyObject *arg, void *valuep);
staticforward int convert_to_string(PyObject *arg,void *store);
staticforward int convert_to_complex_string(PyObject *arg, void *valuep);


/*
** NEW_FUNC should be of the form NEW_FUNC(ctype *)
** LINKED_LIST should have a member 'next'
**
*/

#define  C_LINKED_LIST_TO_PY_LIST(FUNCNAME,c_type,NEW_FUNC)             \
  static PyObject *                                                     \
  FUNCNAME(c_type *clist)                                               \
  {                                                                     \
    PyObject *ret_v = (PyObject *)NULL;                                 \
    c_type *head = clist;                                               \
    int rc = SUCCESS;                                                   \
    int i = 0;                                                          \
    int len = 0;                                                        \
    len = 0;                                                            \
    while(head) { head = head->next; len++;}                            \
    if ( !(ret_v = PyList_New(len)) ) {                                 \
      return NULL;                                                      \
    }                                                                   \
    i = 0;                                                              \
    head = clist;                                                       \
    while((head)) {                                                     \
      PyList_SET_ITEM(ret_v,i,NEW_FUNC(head));                          \
      head = head->next;                                                \
      i++;                                                              \
    } /* while       */                                                 \
    if (rc == FAILED) {                                                 \
      Py_DECREF(ret_v);                                                 \
      return NULL;                                                      \
    }                                                                   \
    return ret_v;                                                       \
  }

#ifdef __cplusplus
}
#endif

#endif /* !PBSPY_CONSTANTS___H */
