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
#include "../include/PBSBatchOpType.h"

/* ~~~~~~~~~~~~~~~~~~~~~~   BatchOp_Type ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static char id_pbs_op[] = "$Id: PBSBatchOpType.c 13 2007-11-30 02:24:22Z kmohraj $";

/* @@@@@ ALL DEFINES IN THIS FILE TO BE UNDEFINED AT THE END @@@@@ */

#define EQ_STR(s,t)      !strcasecmp(s,t)



/* ####################### INTERNAL DATATYPES ############################# */

typedef struct _BatchOp {
  char *name;
  char *print;
  enum batch_op op;
  PyObject *py_str;      /* A string representation of the BatchOp */
} BatchOp;



static BatchOp bops[] = {
  {"SET"   ,"+"  ,SET    ,NULL},
  {"UNSET" ,"-"  ,UNSET  ,NULL},
  {"INCR"  ,"++" ,INCR   ,NULL},
  {"DECR"  ,"--" ,DECR   ,NULL},
  {"EQ"    ,"="  ,EQ     ,NULL},
  {"NE"    ,"!=" ,NE     ,NULL},
  {"GE"    ,">=" ,GE     ,NULL},
  {"GT"    ,">"  ,GT     ,NULL},
  {"LE"    ,"<=" ,LE     ,NULL},
  {"LT"    ,"<"  ,LT     ,NULL},
  {"DFLT"  ,"?"  ,DFLT   ,NULL},
  {NULL,NULL}
};

#define   BATCHOP_ARRAY         bops
#define   BATCHOP_ARRAY_END     ( (sizeof bops/sizeof bops[0]) - 1 )
#define   BATCHOP_NKEYS         BATCHOP_ARRAY_END 
#define   BATCHOP_DFLT_IDX      (BATCHOP_NKEYS - 1)
static int
init_bop_array(void)
{
  int i = 0;
  int status = 0;
  PyObject *tmp_str = (PyObject *)NULL;
  for (i = 0; i < BATCHOP_ARRAY_END; i++) {
    if (!(tmp_str = PyString_FromString(BATCHOP_ARRAY[i].name))) {
      status += -1;
    } else {
      BATCHOP_ARRAY[i].py_str = tmp_str; /* it is new value */
    }
  } /* for (i = 0 ... */
  /* now do the last one */
  if (!(tmp_str = PyString_FromString("None"))) {
    status += -1;
  } else {
    BATCHOP_ARRAY[BATCHOP_ARRAY_END].py_str = tmp_str;
  }
  return status;
}

static int
batchop_index(const char *op_str)
{
  int ret_v = -1;
  char *cn = (char *)NULL;
  char *cp = (char *)NULL;
  int i = 0;

  /* return default if None */
  if (op_str == NULL)
    return BATCHOP_ARRAY_END;

  for(i = 0; i < BATCHOP_NKEYS; i++) {
    cn = BATCHOP_ARRAY[i].name; cp = BATCHOP_ARRAY[i].print;
    if ( EQ_STR(cn,op_str) || EQ_STR(cp,op_str) ) {
      ret_v = i;
      break;
    }
  }
  return ret_v;
}

/* ######################   TYPE MAGIC METHOD ############################# */

/* __new__ */
static PyObject *
batchop__new(PyTypeObject *type,PyObject *args,PyObject *kwds)
{

  BatchOpObject *self = (BatchOpObject *)NULL;

  self = (BatchOpObject *)type->tp_alloc(type, 0);
  if (self) {
    self->_idx = BATCHOP_ARRAY_END;
  }
  return (PyObject *)self;
}

/* __del__ */
static void
batchop__dealloc(BatchOpObject *self)
{
  self->ob_type->tp_free((PyObject*)self);
}

/* __init__ */
static int
batchop__init(BatchOpObject *self, PyObject *args, PyObject *kwds)
{
  char *op_str = NULL;
  static char *kwlist[] = {"op", NULL};
  int idx = -1;

  if (!PyArg_ParseTupleAndKeywords(args, kwds, 
                                   "|z:__init__", kwlist,
                                   &op_str
                                   )
      ) {
    return -1;
  }

  if (op_str) {
    idx = batchop_index(op_str);
    if (idx == -1) {
      PyErr_Format(PyExc_TypeError,"Illegal batch op value \"%s\"",op_str);
      return -1;
    }
    self->_idx = idx;
  }

  return 0;
}

/* __repr__ and __str__ */
static PyObject *
batchop__repr(BatchOpObject *self)
{
  PyObject *ret_str = BATCHOP_ARRAY[self->_idx].py_str;
  Py_INCREF(ret_str);
  return ret_str;
}

/* ########################## GET_SETS #################################### */

static PyObject *
bop_get_op(BatchOpObject *self,void *closure)
{
  /* return None so that we can use in comparisions */
  if (self->_idx == BATCHOP_ARRAY_END) {
    Py_RETURN_NONE;
  } else {
    return batchop__repr(self);
  }
}

static int
bop_set_op(BatchOpObject *self,PyObject *value,void *closure)
{
  char *val = (char *)NULL;
  int idx = -1;
  /* __delete__ */
  if (value == NULL) { /* can't delete op */
    PyErr_Format(PyExc_AttributeError,"cannot delete attribute 'op'");
    return -1;
  }
  /* __set__ */

  if ( !PyString_Check(value) ) {
    
    PyErr_Format(PyExc_TypeError,
                 "Attribute value must be a valid BatchOp string");
    return -1;
  }

  val = PyString_AsString(value);
  idx = batchop_index(val);
  if (idx == -1) {
    PyErr_Format(PyExc_TypeError,"Illegal batch op value \"%s\"",val);
    return -1;
  }
  self->_idx = idx;

  return 0;
}

static PyGetSetDef batchop__getsets [] = {
  {"op" , (getter)bop_get_op, (setter)bop_set_op,
   "get or set the batch op value",
   NULL},
  {NULL}
};

/* ######################   TYPE METHODS      ############################# */

static PyMethodDef batchop__methods[] = {
  {NULL}  /* Sentinel */
};

/* ######################   TYPE DEFINITION   ############################# */

static char batchop__doc[] = 
"op: valid values are of type string,please see <pbs_ifl.h> for more info \n\
\tThis should be set to a python string the correspondence from pbs:\n\
\t'SET'   or '+'   [SET]\n\
\t'UNSET' or '-'   [UNSET]\n\
\t'INCR'  or '++'  [INCREMENT]\n\
\t'DECR'  or '--'  [DECREMENT]\n\
\t'EQ'    or '='   [EQUAL]\n\
\t'NE'    or '!='  [NOT EQUALS]\n\
\t'GE'    or '>='  [GREATER THAN AND EQUALS]\n\
\t'GT'    or '>'   [GREATER THAN]\n\
\t'LE'    or '<='  [LESS THAN AND EQUAL TO]\n\
\t'LT'    or '<'   [LESS THAN]\n\
\t'DFLT'  or '?'   [DEFAULT]\n\
";

/* Krb5 Context Type definition */
static PyTypeObject BatchOp_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                              /*ob_size*/
    TYPEOPBJECT_MODULE_PREFIX "BatchOp",            /*tp_name*/
    sizeof(BatchOpObject),                          /*tp_basicsize*/
    0,                                              /*tp_itemsize*/
    (destructor)batchop__dealloc,                   /*tp_dealloc*/
    0,                                              /*tp_print*/
    0,                                              /*tp_getattr*/
    0,                                              /*tp_setattr*/
    0,                                              /*tp_compare*/
    (reprfunc)batchop__repr,                        /*tp_repr*/
    0,                                              /*tp_as_number*/
    0,                                              /*tp_as_sequence*/
    0,                                              /*tp_as_mapping*/
    0,                                              /*tp_hash */
    0,                                              /*tp_call*/
    0,                                              /*tp_str*/
    (getattrofunc)0,                                /*tp_getattro*/
    (setattrofunc)0,                                /*tp_setattro*/
    0,                                              /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,                             /*tp_flags*/
    batchop__doc,                                   /* tp_doc */
    0,		                                        /* tp_traverse */
    0,		                                        /* tp_clear */
    0,		                                        /* tp_richcompare */
    0,		                                        /* tp_weaklistoffset */
    (getiterfunc)0,	             	                /* tp_iter */
    0,		                                        /* tp_iternext */
    batchop__methods,                               /* tp_methods */
    0,                                              /* tp_members */
    batchop__getsets,                               /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    (initproc)batchop__init,                        /* tp_init */
    0,                                              /* tp_alloc */
    (newfunc)batchop__new,                          /* tp_new */
};
    

/* ######################## UTILITY FUNCTIONS ############################## */

PyObject *
BatchOp_New(const char *op_str)
{
  BatchOpObject *bop = (BatchOpObject *)batchop__new(&BatchOp_Type,NULL,NULL);
  int idx = -1;

  if (bop == NULL)
    goto done;
  /* if op_str is NULL then we already have a default from ..__new(...) */
  if (op_str == NULL)
    goto done;
  /* well, we have to change the index */
  if ((idx = batchop_index(op_str)) == -1) {
    PyErr_Format(PyExc_TypeError,"Invalid batch op string \"%s\"",op_str);
    Py_DECREF(bop);
    return NULL;
  }
  
  bop->_idx = idx;
 done:
  return (PyObject *)bop;
}

/* converter for keyword args or String or BatchOp_Type */
static int
convert_to_batchop(PyObject *arg,void *store)
{
  PyObject *new_val = (PyObject *)NULL;
  char *op_str = (char *)NULL;
  int create_new = 1;

  if (arg == Py_None) {
    create_new = 1; /* default op_str == NULL */
  } else if (PyString_Check(arg)) {
    op_str = PyString_AsString(arg);
  } else if (BatchOpType_Check(arg)) {
    create_new = 0;
    Py_INCREF(arg);
    new_val = arg;
  } else {
    PyErr_Format(PyExc_TypeError,
                 "Illegal value for 'op' must be None,Valid batch op string"
                 " or a valid BatchOp type object");
    return 0;
  }

  if (create_new) {
    if ( !(new_val = BatchOp_New(op_str)) ) {
      return 0;
    }
  }

  /* ok now copy it into the store which must be of type PyObject ** */
  Py_CLEAR(*(PyObject **)store);
  *(PyObject **)store = new_val;
  return 1;
}

/* @@@@@ UNDEFINE ALL THE DEFINES IN THE BEGINNING                 @@@@@ */

#undef EQ_STR
