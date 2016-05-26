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

#include "../include/PBSLogType.h"

/* PBS defines/extern */
char   *msg_daemonname = PBS_LOG_NAME; /* must be defined */

extern char log_directory[];
extern char log_buffer[];

#define LOG_OK                     0
static char err_log_buf[PBSPY_STR_BUFFER];

/* PBSLog_Type */
static char id_pbs_log[] = "$Id: PBSLogType.c 13 2007-11-30 02:24:22Z kmohraj $";

/* ######################   TYPE MAGIC METHOD ############################# */


/* __new__ */

static PyObject *
log__new(PyTypeObject *type,PyObject *args,PyObject *kwds)
{

  PBSLogObject *self = (PBSLogObject *)NULL;

  self = (PBSLogObject *)type->tp_alloc(type, 0);
  if (self) {
    self->daemon_name = PBS_LOG_NAME;
    Py_INCREF(PBS_LOG_NONE);
    self->py_daemon_name = PBS_LOG_NONE;
    self->mask = PBSEVENT_MASK;
    self->free_daemon_name = 0;
    self->filename = (char *)NULL;
    self->directory = (char *)NULL;
  }
  return (PyObject *)self;
}

/* __del__ */
static void
log__dealloc(PBSLogObject *self)
{
  Py_CLEAR(self->py_daemon_name);
  if (self->free_daemon_name) {
    PyMem_Free(self->daemon_name);
    self->free_daemon_name = 0;
  }
  if (self->filename)
    PyMem_Free(self->filename);
  if (self->directory)
    PyMem_Free(self->directory);
  self->ob_type->tp_free((PyObject*)self);
}

/* __init__ */
static int
log__init(PBSLogObject *self,PyObject *args, PyObject *kwds)
{
  char *fname = (char *)NULL;
  char *dname = (char *)NULL;
  long mask = PBSEVENT_MASK;
  char *directory = (char *)NULL;

  static char *kwlist[] = {"name", "mask",
                           "filename","directory",NULL};
  if (!PyArg_ParseTupleAndKeywords(args, kwds, 
                                   "|zO!zz:PBSLog.__init__", 
                                   kwlist,
                                   &dname,
                                   &PyLong_Type,&mask,
                                   &fname,
                                   &directory
                                   )
      ) {
    return -1;
  }
  /* XXX: should check if valid event mask */
  if (mask != PBSEVENT_MASK) {
    self->mask = mask;
  }
  *log_event_mask = mask;

  if ((fname)) {
    if (((*fname != '\0') && (*fname != '/'))) {
      PyErr_Format(PyExc_TypeError,"An absolute filename is needed");
      return -1;
    }
    
    if ( !(self->filename = PyMem_New(char,strlen(fname)+1)) ) {
      return -1;
    }
    strcpy(self->filename,fname);
  }

  if ((directory)) {
    if ( !(self->directory = PyMem_New(char,strlen(directory)+1)) ) {
      return -1;
    }
    strcpy(self->directory,directory);
  } /* if directory */
  if (dname) {
    if ( !(self->daemon_name = PyMem_New(char,strlen(dname)+1)) ) {
      return -1;
    }
    strcpy(self->daemon_name,dname);
    self->free_daemon_name++;
    msg_daemonname = self->daemon_name;
  }
  return 0;
}

/* __repr__ and __str__ */
static PyObject *
log__repr(PBSLogObject *self)
{
  static char repr_buf[PBSPY_STR_BUFFER];
  snprintf(repr_buf,PBSPY_STR_BUFFER,"Name:%s,Filename:'%s',mask:%#lx",
                             self->daemon_name,
                             self->filename ? self->filename : "",
                             self->mask);
  return PyString_FromString(repr_buf);
}

/* ########################## GET_SETS #################################### */

static PyObject *
log_get_mask(PBSLogObject *self,void *closure)
{
  /* return None so that we can use in comparisions */
  return PyLong_FromLong(self->mask);
}

static int
log_set_mask(PBSLogObject *self,PyObject *value,void *closure)
{

  /* __delete__ */
  if (value == NULL) { /* can't delete op */
    PyErr_Format(PyExc_AttributeError,"cannot delete attribute 'mask'");
    return -1;
  }
  /* __set__ */
  if (!(PyLong_Check(value))) {
    PyErr_Format(PyExc_TypeError,
                 "event mask must be a long type");
    return -1;
  }
  self->mask = PyLong_AsLong(value);
  *log_event_mask = self->mask;

  return 0;
}

/* daemon_name */
static PyObject *
log_get_name(PBSLogObject *self,void *closure)
{
  /* return None so that we can use in comparisions */
  Py_INCREF(self->py_daemon_name);
  return self->py_daemon_name;
}

static int
log_set_name(PBSLogObject *self,PyObject *value,void *closure)
{
  char *c_val = (char *)NULL;

  /* __delete__ */
  if (value == NULL) { /* can't delete op */
    PyErr_Format(PyExc_AttributeError,"cannot delete attribute 'name'");
    return -1;
  }
  /* __set__ */
  if ( !(PyString_Check(value)) ) {
    PyErr_Format(PyExc_TypeError,
                 "'name' daemon name must be a string type");
    return -1;
  }
  c_val = PyString_AsString(value);
  if (self->free_daemon_name) {
    PyMem_Free(self->daemon_name);
  }

  if ( !(self->daemon_name = PyMem_New(char,strlen(c_val)+1)) ) {
    return -1;
  }
  strcpy(self->daemon_name,c_val);
  self->free_daemon_name++;
  msg_daemonname = self->daemon_name;

  return 0;
}

static PyGetSetDef log__getsets [] = {
  {"mask" , (getter)log_get_mask, (setter)log_set_mask,
   "get or set the event mask",NULL},
  {"name" , (getter)log_get_name, (setter)log_set_name,
   "get or set the event mask",NULL},
  {NULL}
};

/* ######################   TYPE METHODS      ############################# */

/* log_open */

static char log_open_doc[] = 
"log_open():\n\
open a log file specified by ``filename`` attribute in directory \n\
\tspecified by the ``directory`` attribute\n\
";

static PyObject *
_log_open(PBSLogObject *self)
{
  int rc = LOG_OK;
  errno = 0;
  if ( (rc = log_open(self->filename,self->directory)) != LOG_OK ) {
    snprintf(err_log_buf,PBSPY_STR_BUFFER,
             "could not open log file '%s' in directory '%s': %s",
             self->filename ? self->filename : "DEFAULT",
             self->directory ? self->directory : "DEFAULT",
             strerror(errno));
    SET_PBS_EXCEPTION(-1,errno,err_log_buf);
    return NULL;
  }
  Py_RETURN_NONE;
}

/* log_close */
static char log_close_doc[] = 
"log_close([logit]):\n\
close the currently opened log.\n\
where:\n\
\t``logit`` : if non zero, record 'log closed' message\n\
";
static PyObject *
_log_close(PBSLogObject *self, PyObject *args, PyObject *kwds)
{
  int logit = 0;
  static char *kwlist[] = {"logit",NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "|i:close",kwlist,
                                   &logit
                                   )) {
    return NULL;
  }
  log_close(logit);
  Py_RETURN_NONE;
}

static char log_err_doc[] = 
"log_err(err,failed_routine,mesg):\n\
Useful to record internal errors.\n\
where:\n\
\terr           : 'errno' else -1 for other than system call error\n\
\tfailed_routine: name of the function where failure occured\n\
\tmesg          : text of message to record\n\
";
static PyObject *
_log_err(PBSLogObject *self, PyObject *args, PyObject *kwds)
{
  int  err_no;
  char *routine = NULLSTR;
  char *text = NULLSTR;
  static char *kwlist[] = {"err_no","routine","text",NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "iss:err",kwlist,
                                   &err_no,
                                   &routine,
                                   &text
                                   )) {
    return NULL;
  }
  log_err(err_no,routine,text);
  Py_RETURN_NONE;
}

static char log_event_doc[] = 
"log_event(type,class,severity,name,mesg):\n\
record events regardless given the type and set_event_mask()\n\
where:\n\
\ttype    : Type of event (used, to determine if event should be recorded)\n\
\tclass   : Class of the object (PBS_EVENTCLASS_SERVER..etc)\n\
\tname    : name of the affected object ('mom','server' etc)\n\
\tmesg    : text of message to record\n\
\tNOTES:\n\
\t\tif set_event_mask() is called then only those events in the mask are \n\
\t\trecorded. Unless PBSEVENT_FORCE is specified as the type then it is\n\
\t\talways recorded.\n\
";

static PyObject *
_log_event(PBSLogObject *self, PyObject *args, PyObject *kwds)
{
  int etype;
  int  eclass;
  char *oname = NULLSTR;
  char *text  = NULLSTR;

  static char *kwlist[] = {"etype","eclass","objname","msg",
                           NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "iiss:err",kwlist,
                                   &etype,
                                   &eclass,
                                   &oname,
                                   &text
                                   )) {
    return NULL;
  }
  log_event(etype,eclass,oname,text);
  Py_RETURN_NONE;

}

/* log_record */
static char log_record_doc[] = 
"log_record(type,class,severity,name,mesg):\n\
record events regardless given the type and set_event_mask()\n\
where:\n\
\ttype    : Type of event (used, to determine if event should be recorded)\n\
\tclass   : Class of the object (PBS_EVENTCLASS_SERVER..etc)\n\
\tname    : name of the affected object ('mom','server' etc)\n\
\tmesg    : text of message to record\n\
\tNOTES:\n\
\t\tif set_event_mask() is called then only those events in the mask are \n\
\t\trecorded. Unless PBSEVENT_FORCE is specified as the type then it is\n\
\t\talways recorded.\n\
";

static PyObject *
_log_record(PBSLogObject *self, PyObject *args, PyObject *kwds)
{
  int etype;
  int  eclass;
  char *oname = NULLSTR;
  char *text  = NULLSTR;

  static char *kwlist[] = {"etype","eclass","objname","msg",
                           NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "iiss:err",kwlist,
                                   &etype,
                                   &eclass,
                                   &oname,
                                   &text
                                   )) {
    return NULL;
  }
  log_record(etype,eclass,oname,text);
  Py_RETURN_NONE;
}

static PyMethodDef log__methods[] = {
  {"open",(PyCFunction) _log_open, 
   METH_NOARGS,log_open_doc},                                 /* log_open   */
  {"close",(PyCFunction) _log_close, 
   METH_KEYWORDS,log_close_doc},                              /* log_close  */
  {"error",(PyCFunction) _log_err, 
   METH_KEYWORDS,log_err_doc},                                /* log_err  */
  {"event",(PyCFunction) _log_event, 
   METH_KEYWORDS,log_event_doc},                              /* log_event */
  {"record",(PyCFunction) _log_record, 
   METH_KEYWORDS,log_record_doc},                             /* log_record */
  {NULL}  /* Sentinel */
};

/* ######################   TYPE DEFINITION   ############################# */

static char log__doc[] =
"PBSLog()\n\
";

static PyTypeObject PBSLog_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                              /* ob_size*/
    TYPEOPBJECT_MODULE_PREFIX "PBSLog",             /* tp_name*/
    sizeof(PBSLogObject),                           /* tp_basicsize*/
    0,                                              /* tp_itemsize*/
    (destructor)log__dealloc,                       /* tp_dealloc*/
    0,                                              /* tp_print*/
    0,                                              /* tp_getattr*/
    0,                                              /* tp_setattr*/
    0,                                              /* tp_compare*/
    (reprfunc)log__repr,                            /* tp_repr*/
    0,                                              /* tp_as_number*/
    0,                                              /* tp_as_sequence*/
    0,                                              /* tp_as_mapping*/
    0,                                              /* tp_hash */
    0,                                              /* tp_call*/
    0,                                              /* tp_str*/
    0,                                              /* tp_getattro*/
    0,                                              /* tp_setattro*/
    0,                                              /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,       /* tp_flags*/
    log__doc,                                       /* tp_doc */
    0,		                                        /* tp_traverse */
    0,		                                        /* tp_clear */
    0,		                                        /* tp_richcompare */
    0,		                                        /* tp_weaklistoffset */
    0,		                                        /* tp_iter */
    0,		                                        /* tp_iternext */
    log__methods,                                   /* tp_methods */
    0,                                              /* tp_members */
    log__getsets,                                   /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    (initproc)log__init,                            /* tp_init */
    0,                                              /* tp_alloc */
    (newfunc)log__new,                              /* tp_new */
};


/* @@@@@ UNDEFINE ALL THE DEFINES IN THE BEGINNING                 @@@@@ */

#undef LOG_OK
