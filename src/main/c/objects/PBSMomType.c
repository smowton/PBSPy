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
#include "../include/PBSMomType.h"

/* PBSMom Type */
static char id_pbs_mom[] = "$Id: PBSMomType.c 13 2007-11-30 02:24:22Z kmohraj $";

#define VALID_CONNECT_HDL(h)          ( !(h < 0) )
#define VALID_PORT(p)   (! (p <= 0) )

#define RETURN_INVALID_HDL                                              \
  do {                                                                  \
    if (!VALID_CONNECT_HDL(self->con_hdl)) {                            \
      SET_PBS_EXCEPTION(-1,EINVAL,"connection handle not initialized"); \
      return NULL;                                                      \
    }                                                                   \
  }while (0)

/*
** This is the whole ifl API warpped as a PBSServer object. There are some 
** static (global) variables that are used in the library (pbs_server) etc.
*/


/* ######################   TYPE MAGIC METHOD ############################# */

static void
reset_PBSMomObject(PBSMomObject *self)
{

  if (VALID_CONNECT_HDL(self->con_hdl)) { 
    /* I don't know whether it can be zero */
    (void) closerm(self->con_hdl); /* throw away return */
    self->con_hdl = -1;
  }
  
  /* c_host */
  PyMem_Free(self->c_host);
  self->c_host = (char *)NULL;
  /* port */
  self->port = 0 ; 
  /* fullresp */
  self->fullresp = 0;
}

/* common_init */
static int
rm_common_parse_init(PBSMomObject *self,PyObject *args,
                  PyObject *kwds,char *fmt)
{
  char *host = (char *)NULL;
  int port  = 0;
  PyObject *py_fullresp = (PyObject *)NULL;

  static char *kwlist[] = {"host", "port","fullresp",NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   fmt,kwlist,
                                   &host,
                                   &port,
                                   &py_fullresp
                                   )) {
    return FAILED;
  }


  if (VALID_PORT(port)  && !host) {
    PyErr_Format(PyExc_TypeError,"Need a 'host' parameter along with 'port'");
    return FAILED;
  }

  if (host) {
    PyMem_Free(self->c_host); /* just in case */
    if ( !(self->c_host = PyMem_New(char,strlen(host)+1)) ) {
      return FAILED;
    }
    strcpy(self->c_host,host);
  }
  
  if (VALID_PORT(port)) {
    self->port = port;
  }
  
  if ( !( (py_fullresp == Py_True) || (py_fullresp == Py_False) ||
          (PyInt_Check(py_fullresp)) || (PyLong_Check(py_fullresp)) ) ) {
    
    PyErr_Format(PyExc_TypeError,
                 "fullresp value must be a integer type");
    return FAILED;
  }
  self->fullresp = PyInt_AsLong(py_fullresp) ? 1 : 0;

  return SUCCESS;
}


/* __new__ */
static PyObject *
rm__new(PyTypeObject *type,PyObject *args,PyObject *kwds)
{

  PBSMomObject *self = (PBSMomObject *)NULL;

  self = (PBSMomObject *)type->tp_alloc(type, 0);
  if (self) {
    self->con_hdl = -1;
    self->c_host = (char *)NULL;
    self->port = 0;
  }
  return (PyObject *)self;
}

/* __del__ */
static void
rm__dealloc(PBSMomObject *self)
{
  reset_PBSMomObject(self);
  self->ob_type->tp_free((PyObject*)self);
}

/* __init__ */
static int
rm__init(PBSMomObject *self,PyObject *args, PyObject *kwds)
{
  static char kw_fmt[] = "|zi:__init__";
  if ((rm_common_parse_init(self,args,kwds,kw_fmt) == FAILED)) {
    return -1;
  }
  return 0;
}

/* __repr__ and __str__ */
static PyObject *
rm__repr(PBSMomObject *self)
{
  if (self->c_host) {
    return PyString_FromFormat("Connected to <%s:%d> with handle <%d>",
                               self->c_host,
                               self->port ? self->port:PBS_MOM_SERVICE_PORT,
                               self->con_hdl);
  } else {
    Py_INCREF(PBS_MOM_REPR_UNK);
    return PBS_MOM_REPR_UNK;
  }
}
/* ########################## GET_SETS #################################### */

static PyObject *
rm_get_fullresp(PBSMomObject *self,void *closure)
{
  /* return None so that we can use in comparisions */
  return PyInt_FromLong(self->fullresp);
}

static int
rm_set_fullresp(PBSMomObject *self,PyObject *value,void *closure)
{

  /* __delete__ */
  if (value == NULL) { /* can't delete op */
    PyErr_Format(PyExc_AttributeError,"cannot delete attribute 'fullresp'");
    return -1;
  }
  /* __set__ */

  if ( !( (value == Py_True) || (value == Py_False) ||
          (PyInt_Check(value)) || (PyLong_Check(value)) ) ) {
    
    PyErr_Format(PyExc_TypeError,
                 "Attribute value must be a integer type");
    return -1;
  }

  self->fullresp = PyInt_AsLong(value) ? 1 : 0;
  fullresp(self->fullresp);

  return 0;
}

static PyGetSetDef rm__getsets [] = {
  {"fullresp" , (getter)rm_get_fullresp, (setter)rm_set_fullresp,
   "get or set the full response flag",NULL},
  {NULL}
};

/* ######################   TYPE METHODS      ############################# */

static char rm_open_doc[] = 
"openrm(host,[port]):\n\
open a stream to the Resource monitor running on <host>\n\
where:\n\
\thost: the hostname of the RM to connect to\n\
\tport: port (0 => default port)\n\
";

static PyObject*
rm_open(PBSMomObject* self,PyObject *args,PyObject *kwds)
{
  static char kw_fmt[] = "|zi:open";

  if ((rm_common_parse_init(self,args,kwds,kw_fmt) == FAILED)) {
    return NULL;
  }
  
  self->con_hdl = openrm(self->c_host,self->port);

  /* check we got valid handle */
  if (self->con_hdl < 0) {
    /* NOTE: in the library pbs_server is always initialized before 
       the call*/
    snprintf(err_buf,PBSPY_STR_BUFFER,"openrm:Failed to connect to <%s>",
             self->c_host);   
    SET_PBS_EXCEPTION(-1,pbs_errno,err_buf);
    return NULL;
  }
  fullresp(self->fullresp);
  Py_RETURN_NONE;
}

static char rm_close_doc[] = 
"close():\n\
close a open RM stream\n\
";

static PyObject*
rm_close(PBSMomObject* self)
{
  int rc = 0;

  if (VALID_CONNECT_HDL(self->con_hdl)) {
    rc = closerm(self->con_hdl);
  } else {
    SET_PBS_EXCEPTION(-1,EINVAL,"Invalid connection handle");
    return NULL;
  }

  if (rc != 0) {
    snprintf(err_buf,PBSPY_STR_BUFFER,"disconnect to <%s> failed",self->c_host);
    SET_PBS_EXCEPTION(-1,pbs_errno,err_buf);
    return NULL;
  } else {
    self->con_hdl = -1;
  }
  Py_RETURN_NONE;
}

static char rm_down_doc[] = 
"downrm():\n\
Shutdown the resoure monitor! connected to\n\
";

static PyObject*
rm_down(PBSMomObject* self)
{

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if ((downrm(self->con_hdl) < 0)) {
    SET_PBS_EXCEPTION(-1,pbs_errno,"shutdown or rm failed");
    return NULL;
  } 

  Py_RETURN_NONE;
}

static char rm_config_doc[] = 
"configrm(file):\n\
Cause the resource monitor to read the file named <file>.\n\
where:\n\
\tfile  : the config file to read\n\
";
static PyObject*
rm_config(PBSMomObject* self,PyObject *args,PyObject *kwds)
{
  char *file = (char *)NULL;
  static char *kwlist[] = {"file",NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "s:config",kwlist,
                                   &file
                                   )) {
    return FAILED;
  }

  if ((configrm(self->con_hdl,file) < 0)) {
    SET_PBS_EXCEPTION(-1,pbs_errno,"failed to read the specified file");
    return NULL;
  } 
  Py_RETURN_NONE;
}

/* addreq */
static char rm_addreq_doc[] = 
"add(line):\n\
Add a request to a single stream\n\
where:\n\
\tstream: the stream returned by openrm\n\
\tline  : the command to send to the resource monitor\n\
";

static PyObject*
rm_addreq(PBSMomObject* self,PyObject *args,PyObject *kwds)
{
  char *line = (char *)NULL;
  static char *kwlist[] = {"line",NULL};

  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   "s:addreq",kwlist,
                                   &line
                                   )) {
    return NULL;
  }

  if ((addreq(self->con_hdl,line) < 0)) {
    SET_PBS_EXCEPTION(-1,pbs_errno,"failed to write the specified line");
    return NULL;
  } 
  Py_RETURN_NONE;
}

/* getreq */
static char rm_getreq_doc[] = 
"get():\n\
Finish (and send) any outstanding message to the resource monitor.\n\
and then return the next response line from the resource monitor.\n\
\n\
";

static PyObject*
rm_getreq(PBSMomObject* self)
{
  char *line = (char *)NULL;
  PyObject *ret_v = (PyObject *)NULL;


  line = getreq(self->con_hdl);
  if (!line) {
    SET_PBS_EXCEPTION(-1,pbs_errno,"getreq failed");
    return NULL;
  }
  ret_v = PyString_FromString(line);
  free(line);
  return ret_v;
}

static char rm_flushreq_doc[] = 
"flush():\n\
Finish and send any outstanding messages to all resource monitors.\n\
Return the number of messages flushed.\n\
\n\
\tNote this flushes all pending messages for *all* resource monitors objects\n\
\n\
";

static PyObject*
rm_flushreq(PBSMomObject* self)
{
  int nmesgs = 0;

  nmesgs = flushreq();
  return PyInt_FromLong(nmesgs);
}


static char rm_activereq_doc[] = 
"active():\n\
Return the stream number of the next stream with something\n\
to read or a negative number (the return from rpp_poll)\n\
if there is no stream to read.\n\
";

static PyObject *
rm_activereq(PBSMomObject* self)
{
  int nstream = 0;

  nstream   = activereq();
  return PyInt_FromLong(nstream);
}

static PyMethodDef rm__methods[] = {
  {"open",(PyCFunction) rm_open, 
   METH_KEYWORDS,rm_open_doc},                                 /* openrm   */
  {"close",(PyCFunction)rm_close, 
   METH_NOARGS,rm_close_doc},                                  /* closerm  */
  {"down",(PyCFunction)rm_down, 
   METH_NOARGS,rm_down_doc},                                   /* downrm   */
  {"config",(PyCFunction) rm_config, 
   METH_KEYWORDS,rm_config_doc},                               /* configrm */
  {"add",(PyCFunction) rm_addreq, 
   METH_KEYWORDS,rm_addreq_doc},                               /* addreq */
  {"write",(PyCFunction) rm_addreq, 
   METH_KEYWORDS,rm_addreq_doc},                               /* alias */
  {"get",(PyCFunction)rm_getreq, 
   METH_NOARGS,rm_getreq_doc},                                 /* getreq  */
  {"read",(PyCFunction)rm_getreq, 
   METH_NOARGS,rm_getreq_doc},                                 /* alias  */
  {"flush",(PyCFunction)rm_flushreq, 
   METH_NOARGS,rm_flushreq_doc},                               /* flushreq  */
  {"active",(PyCFunction)rm_activereq, 
   METH_NOARGS,rm_activereq_doc},                              /* activereq  */
  {NULL}  /* Sentinel */
};

/* ######################   TYPE DEFINITION   ############################# */

static char rm__doc[] =
"Mom()\n\
";

static PyTypeObject PBSMom_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                              /* ob_size*/
    TYPEOPBJECT_MODULE_PREFIX "Mom",                /* tp_name*/
    sizeof(PBSMomObject),                           /* tp_basicsize*/
    0,                                              /* tp_itemsize*/
    (destructor)rm__dealloc,                        /* tp_dealloc*/
    0,                                              /* tp_print*/
    0,                                              /* tp_getattr*/
    0,                                              /* tp_setattr*/
    0,                                              /* tp_compare*/
    (reprfunc)rm__repr,                             /* tp_repr*/
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
    rm__doc,                                        /* tp_doc */
    0,		                                        /* tp_traverse */
    0,		                                        /* tp_clear */
    0,		                                        /* tp_richcompare */
    0,		                                        /* tp_weaklistoffset */
    0,		                                        /* tp_iter */
    0,		                                        /* tp_iternext */
    rm__methods,                                    /* tp_methods */
    0,                                              /* tp_members */
    rm__getsets,                                    /* tp_getset */
    0,                                              /* tp_base */
    0,                                              /* tp_dict */
    0,                                              /* tp_descr_get */
    0,                                              /* tp_descr_set */
    0,                                              /* tp_dictoffset */
    (initproc)rm__init,                             /* tp_init */
    0,                                              /* tp_alloc */
    (newfunc)rm__new,                               /* tp_new */
};


/* @@@@@ UNDEFINE ALL THE DEFINES IN THE BEGINNING                 @@@@@ */

#undef VALID_CONNECT_HDL
#undef VALID_PORT
#undef RETURN_INVALID_HDL
