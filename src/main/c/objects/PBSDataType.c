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

#include "../include/PBSDataType.h"

/* PBSData_Type a wrapper for the batch_status data type */

static char id_pbs_batch_status[] = "$Id: PBSDataType.c 13 2007-11-30 02:24:22Z kmohraj $";

/* ######################   TYPE MAGIC METHOD ############################# */

/* __new__ */
static PyObject *
bs__new(PyTypeObject *type,PyObject *args,PyObject *kwds)
{

  PBSDataObject *self = (PBSDataObject *)NULL;

  self = (PBSDataObject *)type->tp_alloc(type, 0);
  if (self) {
    self->name     = (PyObject *)NULL;
    self->text     = (PyObject *)NULL;
    self->attribs  = (PyObject *)NULL;
  }
  return (PyObject *)self;
}

/* __init__ */
static int
bs__init(PBSDataObject *self, PyObject *args, PyObject *kwds)
{

  static char *kwlist[] = {"name", "text","attribs",NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwds, 
                                   "O!|O!O!:PBSData.__init__", 
                                   kwlist,
                                   &PyString_Type,&self->name,
                                   &PyString_Type,&self->text,
                                   &PyList_Type,&self->attribs
                                   )
      ) {
    return -1;
  }

  return 0;
}

/*
** __del__
*/
static void
bs__dealloc(PBSDataObject *self)
{
  Py_CLEAR(self->name);
  Py_CLEAR(self->text);
  Py_CLEAR(self->attribs);
  self->ob_type->tp_free((PyObject*)self);
}

/* __repr__ and __str__ */
static PyObject *
bs__repr(PBSDataObject *self)
{
  Py_INCREF(self->name);
  return self->name;
}

/* ########################## GET_SETS #################################### */
static PyObject *
bs_get_attribs(PBSDataObject *self,void *closure)
{
  if (self->attribs) {
    Py_INCREF(self->attribs);
    return self->attribs;
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject *
bs_get_name(PBSDataObject *self,void *closure)
{
  if (self->name) {
    Py_INCREF(self->name);
    return self->name;
  } else {
    Py_RETURN_NONE;
  }
}

static PyObject *
bs_get_text(PBSDataObject *self,void *closure)
{
  if (self->text) {
    Py_INCREF(self->text);
    return self->text;
  } else {
    Py_RETURN_NONE;
  }
}

static PyGetSetDef bs__getsets [] = {
  {"name" , (getter)bs_get_name, (setter) NULL,
   "get 'name'", (void *)NULL},
  {"text" , (getter)bs_get_text, (setter) NULL,
   "get 'text'", (void *)NULL},
  {"attribs" , (getter)bs_get_attribs, (setter) NULL,
   "get 'attribs'", (void *)NULL},
  {NULL}
};

/* ######################   TYPE METHODS      ############################# */

static PyMethodDef bs__methods[] = {
  {NULL}  /* Sentinel */
};

/* ######################   TYPE DEFINITION   ############################# */

static char bs__doc[] =\
"Data()\n\
";

/* PBS struct batch_status  */
static PyTypeObject PBSData_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                              /* ob_size*/
    TYPEOPBJECT_MODULE_PREFIX "Data",               /* tp_name*/
    sizeof(PBSDataObject),                          /* tp_basicsize*/
    0,                                              /* tp_itemsize*/
    (destructor)bs__dealloc,                        /* tp_dealloc*/
    0,                                              /* tp_print*/
    0,                                              /* tp_getattr*/
    0,                                              /* tp_setattr*/
    0,                                              /* tp_compare*/
    (reprfunc)bs__repr,                             /* tp_repr*/
    0,                                              /* tp_as_number*/
    0,                                              /* tp_as_sequence*/
    0,                                              /* tp_as_mapping*/
    0,                                              /* tp_hash */
    0,                                              /* tp_call*/
    0,                                              /* tp_str*/
    (getattrofunc)0,                                /* tp_getattro*/
    (setattrofunc)0,                                /* tp_setattro*/
    0,                                              /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,       /* tp_flags*/
    bs__doc,                                        /* tp_doc */
    0,		                                        /* tp_traverse */
    0,		                                        /* tp_clear */
    0,		                                        /* tp_richcompare */
    0,		                                        /* tp_weaklistoffset */
    (getiterfunc)0,		                            /* tp_iter */
    0,		                                        /* tp_iternext */
    bs__methods,                                    /* tp_methods */
    0,                                              /* tp_members */
    bs__getsets,                                    /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    (initproc)bs__init,                             /* tp_init */
    0,                                              /* tp_alloc */
    (newfunc)bs__new,                               /* tp_new */
};

/* ######################## UTILITY FUNCTIONS ############################## */

/* construct a new PBSDataObject from struct batch_status */

static PyObject *
PBSData_New(struct batch_status *bs)
{
  PBSDataObject *self = (PBSDataObject *)bs__new(&PBSData_Type,NULL,NULL);

  if (!self)
    return NULL;
  /* set the individual ones */
  if (bs->name) {
    if ( !(self->name = PyString_FromString(bs->name)) ) 
      goto error_exit;
  } else {
    Py_INCREF(PBS_NULLSTR);
    self->name = PBS_NULLSTR;
  }

  if (bs->text) {
    if ( !(self->text = PyString_FromString(bs->text)) ) 
      goto error_exit;
  } else {
    Py_INCREF(PBS_NULLSTR);
    self->text = PBS_NULLSTR;
  }

  /* the list ... */
  if ( !(self->attribs = attrl_to_py_list(bs->attribs)) ) {
    goto error_exit;
  }
  return (PyObject *)self;

 error_exit:
  Py_CLEAR(self); /* this should call the dealloc */
  return NULL;
}


/* FROM utils.h */
C_LINKED_LIST_TO_PY_LIST(bs_to_py_list,struct batch_status,PBSData_New);
