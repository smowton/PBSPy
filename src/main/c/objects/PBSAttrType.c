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

#include "../include/PBSAttrType.h"

/* ~~~~~~~~~~~~~~~~~~~~~~   PBSAttr_Type ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static char id_pbs_attr[] = "$Id: PBSAttrType.c 13 2007-11-30 02:24:22Z kmohraj $";



/* internal defines */
typedef struct _Closure {
  size_t offset;
  char *doc;
} Closure;


/* @@@@@ ALL DEFINES IN THIS FILE TO BE UNDEFINED AT THE END @@@@@ */

#define SET_ATTR_OR_RETURN(_a,_v)                                      \
  do {                                                                 \
    Py_CLEAR(_a);                                                      \
    if (_v) {                                                          \
      _a = PyString_FromString(_v);                                    \
    } else {                                                           \
      _a = PyString_FromString("");                                    \
    }                                                                  \
    if (!_a) {                                                         \
      return -1;                                                       \
    }                                                                  \
  } while (0)

#define EXTRACT_C_STR(o)           \
  (o ? ((o == Py_None) ? "None" : PyString_AsString(o)) : "")

#define EXTRACT_BATCHOP_STR(idx) \
  ( idx == BATCHOP_ARRAY_END  ? "None" :  BATCHOP_ARRAY[idx].name )

/* ######################   TYPE MAGIC METHOD ############################# */

/* __new__ */
static PyObject *
attr__new(PyTypeObject *type,PyObject *args,PyObject *kwds)
{

  PBSAttrObject *self = (PBSAttrObject *)NULL;

  self = (PBSAttrObject *)type->tp_alloc(type, 0);
  if (self) {
    if (!(self->bop = BatchOp_New(NULL))) {
      Py_DECREF(self);
      return (PyObject *)NULL;
    }
    Py_INCREF(Py_None);self->name  = Py_None;
    Py_INCREF(Py_None);self->value = Py_None;
    Py_INCREF(Py_None);self->rscn  = Py_None;
  }
  return (PyObject *)self;
}

/* __init__ */
static int
attr__init(PBSAttrObject *self, PyObject *args, PyObject *kwds)
{

  static char *kwlist[] = {"name", "value","resource","bop",NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwds, 
                                   "O&O&|O&O&:PBSAttr.__init__", 
                                   kwlist,
                                   convert_to_string,&self->name,
                                   convert_to_string,&self->value,
                                   convert_to_string,&self->rscn,
                                   convert_to_batchop,&self->bop
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
attr__dealloc(PBSAttrObject *self)
{
  Py_CLEAR(self->name);
  Py_CLEAR(self->rscn);
  Py_CLEAR(self->value);
  Py_CLEAR(self->bop);
  self->ob_type->tp_free((PyObject*)self);
}

/* __repr__ and __str__ */
static PyObject *
attr__repr(PBSAttrObject *self)
{
  PyObject *ret_v = (PyObject *)NULL;
  static char fmt_buf[PBSPY_STR_BUFFER+1];
  int idx =  ((BatchOpObject *)self->bop)->_idx;

  strcpy(fmt_buf,"Attr(name=");
  (self->name == Py_None) ? strcat(fmt_buf,"%s") : strcat(fmt_buf,"'%s'");
  strcat(fmt_buf,",value=");
  (self->value == Py_None) ? strcat(fmt_buf,"%s") : strcat(fmt_buf,"'%s'");
  strcat(fmt_buf,",resource=");
  (self->rscn == Py_None) ? strcat(fmt_buf,"%s") : strcat(fmt_buf,"'%s'");
  strcat(fmt_buf,",op=");
  (idx == BATCHOP_ARRAY_END) ? strcat(fmt_buf,"%s") : strcat(fmt_buf,"'%s'");
  strcat(fmt_buf,")");

  ret_v =  PyString_FromFormat(fmt_buf,
                               EXTRACT_C_STR(self->name),
                               EXTRACT_C_STR(self->value),
                               EXTRACT_C_STR(self->rscn),
                               EXTRACT_BATCHOP_STR(idx)
                               );

  return ret_v;
}

/* ########################## GET_SETS #################################### */

static PyObject *
attr_get_str(PBSAttrObject *self,void *closure)
{
  PyObject *ret_v = (PyObject *) NULL;
  size_t offset = ((Closure *)closure)->offset;
  ret_v = *(PyObject **)((char *)&(*self) +  offset);
  Py_INCREF(ret_v);
  return ret_v;
}

static int
attr_set_str(PBSAttrObject *self,PyObject *value,void *closure)
{
  PyObject **member= (PyObject **) NULL;
  size_t offset = ((Closure *)closure)->offset;

  /* __delete__ */
  if (value == NULL) { /* can't delete op */
    PyErr_Format(PyExc_AttributeError,"cannot delete attribute '%s'",
                 ((Closure *)closure)->doc);
    return -1;
  }
  /* __set__ */

  member = (PyObject **)((char *)&(*self) +  offset);

  if ( !(convert_to_string(value,(void *)member)) ) {
    return -1;
  }

  return 0;
}

static PyObject *
attr_get_bop(PBSAttrObject *self,void *closure)
{
  Py_INCREF(self->bop);
  return self->bop;
}

static int
attr_set_bop(PBSAttrObject *self,PyObject *value,void *closure)
{

  /* __delete__ */
  if (value == NULL) { /* can't delete op */
    PyErr_Format(PyExc_AttributeError,"cannot delete attribute 'bop'");
    return -1;
  }
  /* __set__ */

  if ( !(convert_to_batchop(value,(void *)&self->bop)) ) {
    return -1;
  }

  return 0;
}

static Closure get_str_closures[] = {
  {offsetof(PBSAttrObject,name)    ,"name"},                     /* name */
  {offsetof(PBSAttrObject,value)   ,"value"},                    /* value */
  {offsetof(PBSAttrObject,rscn)    ,"resource"},                 /* resource */
  {0,NULL}
};

static PyGetSetDef attr__getsets [] = {
  {"name" , (getter)attr_get_str, (setter)attr_set_str,
   "get or set 'name' attribute", (void *)&get_str_closures[0]},
  {"value" , (getter)attr_get_str, (setter)attr_set_str,
   "get or set 'value' attribute", (void *)&get_str_closures[1]},
  {"resource" , (getter)attr_get_str, (setter)attr_set_str,
   "get or set 'resource' attribute", (void *)&get_str_closures[2]},
  {"bop" , (getter)attr_get_bop, (setter)attr_set_bop,
   "get or set BatchOp", (void *)NULL},
  {NULL}
};

/* ######################   TYPE METHODS      ############################# */

static PyMethodDef attr__methods[] = {
  {NULL}  /* Sentinel */
};

/* ######################   TYPE DEFINITION   ############################# */

static char attr__doc[] =\
"attr type: Attr() creates an instance of PBS attr object.\n\
This is basically a wrapper for the C 'struct attrl' in the PBS API\n\
The public attributes are:\n\
\t'name'    :  corresponds to the name of the resource\n\
\t'resource':  corresponds to the resource name \n\
\t'value'   :  is the value for the name/resource \n\
\t'bop'     :  is the value for the batch operation (see BatchOp Type) \n\
\tThe constructor for Attr has the following signature:\n\
\tAttr(name='val',resource='val',value='val',bop='BatchOp type')\n\
\tObviously, python keyword arguments are supported.\n\
\tThe default constructor initializes to all attributres to an empty string\n\
\tA LIST of Attr() objects is generally required by a few PBS API\n\
\tThis object enables one to create such an object.\n\
";

/* PBS struct attrl  */
static PyTypeObject PBSAttr_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                              /*ob_size*/
    TYPEOPBJECT_MODULE_PREFIX "Attr",               /*tp_name*/
    sizeof(PBSAttrObject),                          /*tp_basicsize*/
    0,                                              /*tp_itemsize*/
    (destructor)attr__dealloc,                      /*tp_dealloc*/
    0,                                              /*tp_print*/
    0,                                              /*tp_getattr*/
    0,                                              /*tp_setattr*/
    0,                                              /*tp_compare*/
    (reprfunc)attr__repr,                           /*tp_repr*/
    0,                                              /*tp_as_number*/
    0,                                              /*tp_as_sequence*/
    0,                                              /*tp_as_mapping*/
    0,                                              /*tp_hash */
    0,                                              /*tp_call*/
    0,                                              /*tp_str*/
    (getattrofunc)0,                                /*tp_getattro*/
    (setattrofunc)0,                                /*tp_setattro*/
    0,                                              /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,       /*tp_flags*/
    attr__doc,                                      /* tp_doc */
    0,		                                        /* tp_traverse */
    0,		                                        /* tp_clear */
    0,		                                        /* tp_richcompare */
    0,		                                        /* tp_weaklistoffset */
    (getiterfunc)0,		                            /* tp_iter */
    0,		                                        /* tp_iternext */
    attr__methods,                                  /* tp_methods */
    0,                                              /* tp_members */
    attr__getsets,                                  /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    (initproc)attr__init,                           /* tp_init */
    0,                                              /* tp_alloc */
    (newfunc)attr__new,                             /* tp_new */
};

/* ######################## UTILITY FUNCTIONS ############################## */

/* construct a new PBSAttrObject from struct attrl */

static PyObject *
PBSAttr_New(struct attrl *attr)
{
  PBSAttrObject *self = (PBSAttrObject *)attr__new(&PBSAttr_Type,NULL,NULL);
  PyObject *kwds = PyDict_New();
  PyObject *args = PyTuple_New(0);

  if ( !(self) || !(kwds) || !(args) ) 
    goto error_exit;

  if ( (set_dict_string(kwds,"name",attr->name) == -1) ) {
    printf("failed setting name\n");
    goto error_exit;
  }
  if ( (set_dict_string(kwds,"value",attr->value) == -1) ) {
    printf("failed setting value\n");
    goto error_exit;
  }
  if ( (set_dict_string(kwds,"resource",attr->resource) == -1) ) {
    printf("failed setting resource\n");
    goto error_exit;
  }
  if ( (attr__init(self,args,kwds) == -1) ) {
    printf("failed __init__\n");
    goto error_exit;
  }

  Py_CLEAR(kwds);
  Py_CLEAR(args);
  return (PyObject *)self;

 error_exit:
  Py_CLEAR(kwds);
  Py_CLEAR(args);
  Py_CLEAR(self); /* this should call dealloc */
  return NULL;
}


/*
** MACRO For Python list to C linked list
** 0: conversion was successful, 1: failed.
*/

#define NULL_OR_C_STR(o)                                            \
  (o ? ( (o == Py_None) ?  NULLSTR : PyString_AsString(o) ) : NULLSTR)


#define FUNC_SET_C_ATTR(FNAME,c_type)                                   \
  static void                                                           \
  FNAME(c_type *carg,PBSAttrObject *arg)                                \
  {                                                                     \
    int idx = BATCHOP_DFLT_IDX;                                         \
    carg->name = NULL_OR_C_STR(arg->name);                              \
    carg->resource = NULL_OR_C_STR(arg->rscn);                          \
    carg->value =  NULL_OR_C_STR(arg->value);                           \
    if ((BatchOpType_Check(arg->bop))) {                                \
      idx = ((BatchOpObject *)arg->bop)->_idx;                          \
      if (idx == BATCHOP_ARRAY_END) {                                   \
        idx = BATCHOP_DFLT_IDX; /* set it as DFLT */                    \
      }                                                                 \
    }                                                                   \
    /* set the op */                                                    \
    carg->op = BATCHOP_ARRAY[idx].op;                                   \
  }

FUNC_SET_C_ATTR(_set_attrl,struct attrl);
FUNC_SET_C_ATTR(_set_attropl, struct attropl);

/* do the free functions */
#define FUNC_FREE_ATTRL(FNAME,c_type)                                   \
  static void                                                           \
  FNAME(c_type *carg)                                                   \
  {                                                                     \
    c_type *tmp = (c_type *)NULL;                                       \
    c_type *tmp_traverse = (c_type *)carg;                              \
    while ((tmp = tmp_traverse)) {                                      \
      tmp_traverse = tmp_traverse->next;                                \
      PyMem_Free(tmp);                                                  \
    }                                                                   \
  }                                                                     

FUNC_FREE_ATTRL(_free_attrl  ,struct attrl);
FUNC_FREE_ATTRL(_free_attropl,struct attropl);

/*
** Caller *MUST* free the clist if succeeded.
**   Raises:
**        TypeError,MemoryError
**
*/

#define  PBSATTR_TO_C_ATTR_LIST(FUNCNAME,c_type,SET_FUNC,FREE_FUNC)     \
  static int                                                            \
  FUNCNAME(PyObject *arg,c_type **clist)                                \
  {                                                                     \
    PyObject *iter = (PyObject *)NULL;                                  \
    PyObject *item = (PyObject *)NULL;                                  \
    int rc = SUCCESS;                                                   \
    c_type *tmp = (c_type *)NULL;                                       \
    c_type *tmp_traverse = (c_type *)NULL;                              \
    int idx = 0;                                                        \
    /* see if we are None or NULL then it is empty   */                 \
    if ( (arg == Py_None) || (arg == NULL) ) {                          \
      *clist = (c_type *)NULL;                                          \
      return SUCCESS;                                                   \
    }                                                                   \
    /* make sure we have a valid iterator */                            \
    if ( !(iter = PyObject_GetIter(arg)) ) {                            \
      return FAILED;                                                    \
    }                                                                   \
    idx = 0;/* mainly to indicate the item index for exception */       \
    while ( (item = PyIter_Next(iter)) != NULL) {                       \
      /* check if we are PBSAttr_Type */                                \
      if ( !(PBSAttrType_Check(item)) ) {                               \
        rc = FAILED;                                                    \
        PyErr_Format(PyExc_TypeError,                                   \
                     "list item number <%d> not of PBSAttr Type",idx);  \
        break;                                                          \
      }                                                                 \
      if ( !(tmp = PyMem_New(c_type,1)) ) {                             \
        rc = FAILED;                                                    \
        break;                                                          \
      } else {                                                          \
        /* make sure tail is null */                                    \
        tmp->next = (c_type *)NULL;                                     \
      }                                                                 \
      /* ok we got the tmp, check and see if the 'head' is there */     \
      if (*clist) {                                                     \
        tmp_traverse = *clist;                                          \
        while ((tmp_traverse->next)) tmp_traverse = tmp_traverse->next; \
        tmp_traverse->next = tmp;                                       \
      } else {                                                          \
        *clist = tmp;                                                   \
      }                                                                 \
      SET_FUNC(tmp,(PBSAttrObject *)item);                              \
        /* release reference when done */                               \
      Py_DECREF(item);                                                  \
      idx++;                                                            \
    }  /* while (item...)                 */                            \
    Py_DECREF(iter);                                                    \
    /* OK, there might be error in retrieving item */                   \
    if ( (PyErr_Occurred()) ) {                                         \
      rc = FAILED;                                                      \
    }                                                                   \
    if ( rc == FAILED ) {                                               \
      Py_DECREF(item); /* make sure item is decref'ed, */               \
      FREE_FUNC(*clist);                                                \
    }                                                                   \
    return rc;                                                          \
  }

PBSATTR_TO_C_ATTR_LIST(attr_to_c_attrl  ,struct attrl,_set_attrl,_free_attrl);
PBSATTR_TO_C_ATTR_LIST(attr_to_c_attropl,struct attropl,
                       _set_attropl,_free_attropl);


/* FROM utils.h */
C_LINKED_LIST_TO_PY_LIST(attrl_to_py_list,struct attrl,PBSAttr_New);

/* @@@@@ UNDEFINE ALL THE DEFINES IN THE BEGINNING                 @@@@@ */

#undef  SET_ATTR_OR_RETURN
#undef  EXTRACT_C_STR
#undef  EXTRACT_BATCHOP_STR
#undef  NULL_OR_C_STR