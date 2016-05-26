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

/*
** This file contains all the c extension module functions or module methods
*/ 

static PyObject *
attr_test_cattr(PyObject *module, PyObject *args, PyObject *kwds)
{
  struct attrl *attribs = (struct attrl *)NULL;
  static char *kwlist[] = {"attribs",NULL};
  PyObject *py_list = (PyObject *)NULL;
  PyObject *ret_v = (PyObject *)NULL;
  int status = SUCCESS;

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "O:test_c_attr",
                                   kwlist,
                                   &py_list
                                   )
      ) {
    return NULL;
  }
  status = attr_to_c_attrl(py_list,&attribs);
  if (status == SUCCESS) {
    struct attrl *tmp = attribs;
    while(tmp) {
      printf("name: %s,res:%s,val:%s,op:%d\n",
             tmp->name,
             tmp->resource,
             tmp->value,
             tmp->op);
      tmp = tmp->next;
    }
    /* return again */
    ret_v = attrl_to_py_list(attribs);
    _free_attrl(attribs);
  } else {
    printf("failed to convert\n");
    return NULL;
  }
  return ret_v;
}


/*
**  ~~~~ Module Method Definition Obect ~~~~
*/

static PyMethodDef pbsapi_methods[] = {
  {"test_cattr",(PyCFunction)attr_test_cattr, 
   METH_KEYWORDS," test building c attrl"},
  {NULL,          NULL}                                 /* sentinel */
};
