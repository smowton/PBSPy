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

/*  A Generic Options Type.
 *         This is a object that exposes various constants in the CAPI as a 
 *         read only attribute accessor.
 *
 * A Generic option object.
*/
#include "../include/OptionType.h"

static PyObject *
create_option(PyTypeObject *type,unsigned int flag)
{
  OptionObject *self;

  self = (OptionObject *)type->tp_alloc(type, 0);

  if (self != NULL) {
    self->_d = PyDict_New();
    if (self->_d == NULL) {
      Py_DECREF(self);
      return NULL;
    }
  }
  self->settable_once = flag;

  return (PyObject *)self;
}

/*  __new__ method for options object.
 */
static PyObject *
option__new(PyTypeObject *type,PyObject *args,PyObject *kwds)
{
  unsigned int set_once = 0;
  static char *kwlist[] = {"set_once",NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "|I:option.__new__",kwlist,
                                   &set_once))
    return NULL;
  
  return create_option(type,set_once);
}

/* __del__ */
static void
option__dealloc(OptionObject *self)
{
  Py_XDECREF(self->_d);
  self->ob_type->tp_free((PyObject*)self);
}

#if 0 /* Not needed */
static PyObject *
_option__getattro(OptionObject *self,PyObject *attr)
{
  PyObject *res = (PyObject *)NULL;
  res = PyObject_GenericGetAttr((PyObject *)self,attr);
  return res;
}
#endif /* if 0 */

/* __getattribute__ */
static PyObject *
option__getattro(OptionObject *self,PyObject *attr)
{
  PyObject *res = (PyObject *)NULL;

  /* we always get the attribute from internal dict */
  /* PyDict_GetItem does *not* set an exception if item is not found */
  res = PyDict_GetItem((PyObject *)self->_d,attr);
  if (!res) {
    res = PyObject_GenericGetAttr((PyObject *)self,attr);
  }
  Py_XINCREF(res);
  return res;

}

/* __setattr__ */
static int
option__setattro(OptionObject *self,PyObject *name,PyObject *value)
{
  int res = -1;
  PyObject *dict = self->_d;
  char *name_str = (char *)NULL;

  /* increment the reference count so we have it */
  if (!PyString_Check(name)){
#ifdef Py_USING_UNICODE
    /* The Unicode to string conversion is done here because the
       existing tp_setattro slots expect a string object as name
       and we wouldn't want to break those. */
    if (PyUnicode_Check(name)) {
      name = PyUnicode_AsEncodedString(name, NULL, NULL);
      if (name == NULL)
        return -1;
      name_str = (char *)PyUnicode_AS_DATA(name);
    }
    else
#endif
      {
        PyErr_SetString(PyExc_TypeError,
                        "attribute name must be string");
        return -1;
      }
  }else {
    Py_INCREF(name);
    name_str = PyString_AsString(name);
  }

  if (dict != NULL) {
    if (value == NULL)
      res = PyDict_DelItem(dict, name);
    else {
      if (self->settable_once && (PyDict_GetItem(dict,name) != NULL)) {
        PyErr_Format(PyExc_AttributeError,"attribute <%s> already set once",
                     name_str);
        res = -1;
        goto done;
      }
      res = PyDict_SetItem(dict, name, value);
    }
    goto done;
  }

 done:
  Py_DECREF(name); /* don't need it */
  return res;
}

/* __iter__ */
/** useful for wrappers that want to iterate over the attribute constants 
 */
static PyObject *
option__iter(OptionObject *self)
{
  /* return PyObject_CallMethod(self->_d,"__iter__",NULL); */
  return PyObject_GetIter(self->_d);
}

/* __repr__ */
/** useful for wrappers that want to iterate over the attribute constants 
 */
static PyObject *
option__repr(OptionObject *self)
{
  /* return PyObject_CallMethod(self->_d,"__iter__",NULL); */
  return PyObject_Repr(self->_d);
}

/* A generic Option type  */
/*
** PyObject_HEAD_INIT macro is set to NULL for some platforms, but gets properly initialized
** when PyType_Ready() is called, it *must* be called during module init.
*/ 
static PyTypeObject Option_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                              /* ob_size*/
    TYPEOPBJECT_MODULE_PREFIX "option",             /* tp_name*/
    sizeof(OptionObject),                           /* tp_basicsize*/
    0,                                              /* tp_itemsize*/
    (destructor)option__dealloc,                    /* tp_dealloc*/
    0,                                              /* tp_print*/
    0,                                              /* tp_getattr*/
    0,                                              /* tp_setattr*/
    0,                                              /* tp_compare*/
    (reprfunc)option__repr,                         /* tp_repr*/
    0,                                              /* tp_as_number*/
    0,                                              /* tp_as_sequence*/
    0,                                              /* tp_as_mapping*/
    0,                                              /* tp_hash */
    0,                                              /* tp_call*/
    0,                                              /* tp_str*/
    (getattrofunc)option__getattro,                 /* tp_getattro*/
    (setattrofunc)option__setattro,                 /* tp_setattro*/
    0,                                              /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,                             /* tp_flags*/
    "A Generic Option Object",                      /* tp_doc */
    0,		                                        /* tp_traverse */
    0,		                                        /* tp_clear */
    0,		                                        /* tp_richcompare */
    0,		                                        /* tp_weaklistoffset */
    (getiterfunc)option__iter,		                /* tp_iter */
    0,		                                        /* tp_iternext */
    0,                                              /* tp_methods */
    0,                                              /* tp_members */
    0,                                              /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    0,                                              /* tp_init */
    0,                                              /* tp_alloc */
    (newfunc)option__new,                           /* tp_new */
};


/*
##############################################################################
#                      EXPORTED FUNCTION TO C API                            # 
##############################################################################
*/

/* convenience function to create a Option instance from the C/API */
static PyObject *
OptionType_New(unsigned int settable_once)
{
  return create_option(&Option_Type,settable_once);
}

/* convenience function to set a attribute from C/API */
static int
Option_SetAttr(OptionObject *self,PyObject *attr,PyObject *value)
{
  return option__setattro(self,attr,value);
}

/*
** helper function to set attr for an object from int
*/

static int
_setattr_int_const(PyObject *o,char *a, int code)
{
  PyObject *_a = PyString_FromString(a);
  PyObject *v = PyInt_FromLong((long)code);
  int status = 0;
  if (_a && v) {
    status = Option_SetAttr((OptionObject *)o,_a,v);
  }
  Py_XDECREF(_a);
  Py_XDECREF(v);
  return status;
}
/*
** helper function to set attr for an object from int
*/

static int
_setattr_str_const(PyObject *o,char *a, char *s)
{
  PyObject *_a = PyString_FromString(a);
  PyObject *v =  PyString_FromString(s);
  int status = 0;
  if (_a && v) {
    status = Option_SetAttr((OptionObject *)o,_a,v);
  }
  Py_XDECREF(_a);
  Py_XDECREF(v);
  return status;
}



