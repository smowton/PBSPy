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

/* PBSServer Type wraps all the communication to a PBS Server */
/*
** This is the whole ifl API warpped as a PBSServer object. There are some 
** static (global) variables that are used in the library (pbs_server) etc.
*/

#include "../include/PBSServerType.h"
static char id_pbs_server[] = "$Id: PBSServerType.c 13 2007-11-30 02:24:22Z kmohraj $";
static char err_buf[PBSPY_STR_BUFFER];        /* buffer for messages */

/* EXTERNALS */
extern char *pbs_server;
extern int pbs_errno;


/* ######################   TYPE MAGIC METHOD ############################# */


#define SETIFL_PBS_SERVER                       \
  do {                                          \
    if (self->pbs_server) {                     \
      pbs_server = self->pbs_server;            \
    }                                           \
  }while (0)


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
** useful function to close connection and reinit the PBSServerObject with
** sane values.
*/

static void
reset_PBSServerObject(PBSServerObject *self)
{

  if (VALID_CONNECT_HDL(self->con_hdl)) { 
    /* I don't know whether it can be zero */
    (void) pbs_disconnect(self->con_hdl); /* throw away return */
    self->con_hdl = -1;
  }

  PyMem_Free(self->pbs_server);
  self->pbs_server = (char *)NULL;

  /* py_pbs_server */
  Py_CLEAR(self->py_pbs_server);
  Py_INCREF(PBS_UNKNOWN_HOST);
  self->py_pbs_server = PBS_UNKNOWN_HOST;
  
  /* c_host */
  PyMem_Free(self->c_host);
  self->c_host = (char *)NULL;
  /* port */
  self->port = 0 ; 
  /* timeout */
  self->timeout = -1;
}

/* common_init */
static int
common_parse_init(PBSServerObject *self,PyObject *args,
                  PyObject *kwds,char *fmt)
{
  char *host = (char *)NULL;
  int port  = 0;
  int timeout = -1; /* timeout */
  char connect_str[PBSPY_STR_BUFFER];

  static char *kwlist[] = {"host", "port", "timeout",NULL};
  memset(connect_str,0,sizeof connect_str);

  /* NOTES:
  ** The connect API accepts only [host:port] if host is not given then
  ** port is ignored.
  */
  if (!PyArg_ParseTupleAndKeywords(args,kwds,
                                   fmt,kwlist,
                                   &host,
                                   &port,
                                   &timeout
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
  
  if ((timeout >= 0)) {
    self->timeout = timeout;   /* for non-blocking */
  }
  return SUCCESS;
}


/* __new__ */
static PyObject *
svr__new(PyTypeObject *type,PyObject *args,PyObject *kwds)
{

  PBSServerObject *self = (PBSServerObject *)NULL;

  self = (PBSServerObject *)type->tp_alloc(type, 0);
  if (self) {
    self->con_hdl = -1;
    Py_INCREF(PBS_UNKNOWN_HOST); 
    self->py_pbs_server = PBS_UNKNOWN_HOST;
    self->pbs_server = (char *)NULL;
    self->c_host = (char *)NULL;
    self->port = 0;
    self->timeout = -1;
  }
  return (PyObject *)self;
}

/* __del__ */
static void
svr__dealloc(PBSServerObject *self)
{
  reset_PBSServerObject(self);
  self->ob_type->tp_free((PyObject*)self);
}

/* __init__ */
static int
svr__init(PBSServerObject *self,PyObject *args, PyObject *kwds)
{
  static char kw_fmt[] = "|zii:__init__";
  if ((common_parse_init(self,args,kwds,kw_fmt) == FAILED)) {
    return -1;
  }
  return 0;
}

/* __repr__ and __str__ */
static PyObject *
svr__repr(PBSServerObject *self)
{
  if (self->pbs_server) {
    return PyString_FromFormat("Connected to <%s:%d> with handle <%d>",
                               self->pbs_server,
                               self->port ? self->port : PBS_BATCH_SERVICE_PORT,
                               self->con_hdl);
  } else {
    Py_INCREF(self->py_pbs_server);
    return self->py_pbs_server;
  }
}

/* ########################## GET_SETS #################################### */

static PyObject *
svr_get_timeout(PBSServerObject *self,void *closure)
{
  /* return None so that we can use in comparisions */
  return PyInt_FromLong(self->timeout);
}

static int
svr_set_timeout(PBSServerObject *self,PyObject *value,void *closure)
{

  /* __delete__ */
  if (value == NULL) { /* can't delete op */
    PyErr_Format(PyExc_AttributeError,"cannot delete attribute 'timeout'");
    return -1;
  }
  /* __set__ */

  if ( !(PyInt_Check(value) || PyLong_Check(value)) ) {
    
    PyErr_Format(PyExc_TypeError,
                 "Attribute value must be a integer type");
    return -1;
  }

  self->timeout = PyInt_AsLong(value); /* XXX: overflow/underflow? */

  return 0;
}

static PyGetSetDef svr__getsets [] = {
  {"timeout" , (getter)svr_get_timeout, (setter)svr_set_timeout,
   "get or set the timeout value for connect/open call",
   NULL},
  {NULL}
};

/* ######################   TYPE METHODS      ############################# */

static char svr_avail_doc[] = 
"Implements avail() (see the man page for pbs_rescquery)\n\
avail(resc_str):\n\
\tresc_str   :   resc is a single node=specification specification (string)\n\
\tReturn Values:\n\
\t\t'yes' or 'no' or 'never' and in case of error '?'\n\
\n\
\tNOTES:\n\
\tThis is mainly used for older scheduler, it is recommened to use the newer\n\
\t``rescquery(...)`` method.\n\
";

static PyObject*
svr_avail(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *resc  = (char *)NULL;
  char *rstr = (char *)NULL; /* returned by PBS API */

  static char *kwlist[] = {"resc_str",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s:avail",
                                    kwlist,
                                    &resc
                                    ))
      )
    {
      return NULL;
    }

  rstr = avail(self->con_hdl,resc);
  return PyString_FromString(rstr);
}

/* pbs_asyrunjob */
static char svr_asyrunjob_doc[] = 
"Implements pbs_asyrunjob (see the man page)\n\
asyrunjob(id,[location],[extend]):\n\
\tparameters within [] are optional\n\
Asynchronous Run Job request to the batch server\n\
\tid     :   Job identifier (sequence_number.server)\n\
\tlocation   :   the location where the job should be run (string)\n\
\textend     :    Not used(as per PBS API)\n\
";
static PyObject*
svr_asyrunjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id   = (char *)NULL;
  char *location = NULLSTR;
  char *extend   = (char *)NULL;
  int rc = 0; /* success */
  static char *kwlist[] = {"id","location","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|sz:asyrunjob",
                                    kwlist,
                                    &job_id,
                                    &location,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }
  
  rc = pbs_asyrunjob(self->con_hdl,job_id,location,extend);
  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :asyrunjob");
    return NULL;
  }

  Py_RETURN_NONE;
}

/* pbs_alterjob */
static char svr_alterjob_doc[] = 
"Implements pbs_alterjob (see the man page)\n\
alterjob(id,[attribs],[extend]):\n\
\tparameters within [] are optional\n\
\tIssue a batch request to alter a batch job\n\
\tjob_id :   Job/Dest identifier, can be None/NULL String\n\
\tattribs :   This is a python LIST of Attr() objects \n\
\textend :   Not used(as per PBS API)\n\
";

static PyObject*
svr_alterjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id   = (char *)NULL;
  struct attrl *attrib     = (struct attrl *)NULL;
  PyObject *py_attrib  = (PyObject *)NULL; 
  char *extend   = (char *)NULL;
  int free_attrib = 0;
  int rc = 0;
  static char *kwlist[] = {"id","attribs","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|Oz:alterjob",
                                    kwlist,
                                    &job_id,
                                    &py_attrib,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }
  if (py_attrib && !IS_SEQUENCE_TYPE(py_attrib)) {
    PyErr_Format(PyExc_TypeError,"'attrib' must be tuple or list type");
    return NULL;
  }
  /* good we have a list */
  if ( (attr_to_c_attrl(py_attrib,&attrib) == FAILED) )
    return NULL;
  free_attrib++; /* make sure we free it */

  /* do the honors */
  rc = pbs_alterjob(self->con_hdl,job_id,attrib,extend);
  if (free_attrib)
    _free_attrl(attrib);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :alterjob");
    return NULL;
  }

  Py_RETURN_NONE;

}

/* pbs_connect */
static char svr_connect_doc[] = 
"connect OR open([host],[port],[timeout]):\n\
where:\n\
\t host    : the hostname of the pbs server to connect to [optional]\n\
\t port    : the port at which the server is listening.\n\
\t timeout : specifying timeout (secs) automatically reduces the underlying \n\
\t           call as non-blocking. (NOT IMPLEMENTED)\n\
\t           \n\
\t Note, we called with no arguments, default server and port are used\n\
\t Return Values:\n\
";

static PyObject *
svr_connect(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char connect_str[PBSPY_STR_BUFFER+1];
  static char kw_fmt[] = "|zii:connect";
  char *cp = (char *)NULL;

  memset((char *)connect_str,0,sizeof connect_str);

  if ((common_parse_init(self,args,kwds,kw_fmt) == FAILED)) {
    return NULL;
  }

  if (self->c_host && VALID_PORT(self->port)) {
    snprintf(connect_str,PBSPY_STR_BUFFER,"%s:%d",self->c_host,self->port);
    cp = &connect_str[0]; /* MUST DO */
  } else if (self->c_host) {
    snprintf(connect_str,PBSPY_STR_BUFFER,"%s",self->c_host);
    cp =  &connect_str[0];
  }

    self->con_hdl = pbs_connect(cp);

  /* check we got valid handle */
  if (self->con_hdl < 0) {
    /* NOTE: in the library pbs_server is always initialized before 
       the call*/
    snprintf(err_buf,PBSPY_STR_BUFFER,"Failed to connect to <%s>",pbs_server);   
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno,err_buf);
    return NULL;
  }

  /* good, make a copy of pbs_server also make sure pbs_server is valid */
  PyMem_Free(self->pbs_server);
  if ( !(self->pbs_server = PyMem_New(char,(strlen(pbs_server)+1))) ) {
    /* damn */
    goto error_exit;
  }
  strcpy(self->pbs_server,pbs_server); /* copy the connect host */
  /* copy to py_pbs_server */
  Py_CLEAR(self->py_pbs_server);
  if ( !(self->py_pbs_server = PyString_FromString(self->pbs_server)) ) {
    goto error_exit;
  }
  Py_RETURN_NONE;
  
 error_exit:

  reset_PBSServerObject(self);
  return NULL;
}


/* pbs_disconnect */
static char svr_disconnect_doc [] =
"disconnect() OR close(): close the active connection\n\
";

static PyObject*
svr_disconnect(PBSServerObject* self)
{
  int rc = 0;

  if (VALID_CONNECT_HDL(self->con_hdl)) {
    rc = pbs_disconnect(self->con_hdl);
  }
  if (rc != 0) {
    snprintf(err_buf,PBSPY_STR_BUFFER,"disconnect to <%s> failed",self->pbs_server);
    SET_PBS_EXCEPTION(-1,pbs_errno,err_buf);
    return NULL;
  } else {
    self->con_hdl = -1;
  }

  reset_PBSServerObject(self);
  Py_RETURN_NONE;
}

/* pbs_deljob */
static char svr_deljob_doc[] = 
"Implements pbs_deljob (see the man page)\n\
deljob(id,[extend]):\n\
\tparameters within [] are optional\n\
\tIssue a batch request to delete a batch job\n\
\t    id :   Job/Dest identifier, can be None/NULL String\n\
\textend :   see below (string)\n\
\t  The argument, extend, is overloaded to serve two purposes. If \n\
\t  extend points to a string of the form:\n\
\t  deldelay=nnnn,\n\
\t  it is used to provide control over the delay between sending SIGTERM and\n\
\t  SIGKILL signals to a running job. The characters nnnn specify a unsigned\n\
\t  decimal integer time delay in seconds.  If extend is \n\
\t   a null string,the administrator established default time delay\n\
\t  is used.\n\
\t\n\
\t  If extend points to a string other than the above,it is taken as text to\n\
\t  be appended to the message mailed to the job owner. This mailing occurs\n\
\t  if the job is deleted by a user other than the job owner.\n\
";

static PyObject*
svr_deljob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id   = NULLSTR;
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"id","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|z:deljob",
                                    kwlist,
                                    &job_id,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_deljob(self->con_hdl,job_id,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :deljob");
    return NULL;
  }

  Py_RETURN_NONE;

}

/* pbs_holdjob */
static char svr_holdjob_doc[] = 
"Implements pbs_holdjob (see the man page)\n\
holdjob(id,[hold_type],[extend]):\n\
\tparameters within [] are optional\n\
\tSwap the order of two jobs within a single queue.\n\
\tid     :   Job identifier (sequence_number.server,string)\n\
\thold_type  :   Hold type (string: 'u' or 'o', 's' :def='u')\n\
\textend     :   Not used(as per PBS API)\n\
";
static PyObject*
svr_holdjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id   = NULLSTR;
  char *hold_type = NULLSTR;
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"id","hold_type","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|sz:holdjob",
                                    kwlist,
                                    &job_id,
                                    &hold_type,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_holdjob(self->con_hdl,job_id,hold_type,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :holdjob");
    return NULL;
  }

  Py_RETURN_NONE;

}

/* pbs_locjob */
static char svr_locjob_doc[] = 
"Implements pbs_locjob (see the man page)\n\
locjob(id,[extend]):\n\
\tparameters within [] are optional\n\
Issue a locate job request to the server\n\
\tid     :   Job identifier (sequence_number.server,string)\n\
\textend     :   Not used(as per PBS API)\n\
";

static PyObject*
svr_locjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id   = NULLSTR;
  char *extend   = (char *)NULL;
  char *ret_s = (char *)NULL;
  static char *kwlist[] = {"id","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|z:locjob",
                                    kwlist,
                                    &job_id,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  ret_s = pbs_locjob(self->con_hdl,job_id,extend);

  if (!(ret_s)) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :locjob");
    return NULL;
  }
  return PyString_FromString(ret_s);

}
static char svr_movejob_doc[] = 
"Implements pbs_movejob (see the man page)\n\
movejob(id,[destination],[extend]):\n\
\tparameters within [] are optional\n\
\tid     :   Job identifier, can be None/NULL String\n\
\tdestination:   Job identifier, can be None/NULL String\n\
\textend     :   Not used(as per PBS API)\n\
";

static PyObject*
svr_movejob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id   = (char *)NULL;
  char *dest = (char *)NULL;
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"id","dest","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|zz:movejob",
                                    kwlist,
                                    &job_id,
                                    &dest,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_movejob(self->con_hdl,job_id,dest,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :movejob");
    return NULL;
  }

  Py_RETURN_NONE;

}

/* pbs_msgjob */
static char svr_msgjob_doc[] = 
"Implements pbs_msgjob (see the man page)\n\
msgjob(id,file,message,[extend]):\n\
\tparameters within [] are optional\n\
\tid         :         Job identifier, can be None/NULL String\n\
\tfid        :   file (integer id) to which message is to be written \n\
\t               (ERR=2,OUT=1)[default = ERR]\n\
\tmessage    :   The message string to be written\n\
\textend     :    Not used(as per PBS API)\n\
";
static PyObject*
svr_msgjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id    = (char *)NULL;
  int fid  = 2; /* file id */
  char *message = "PING MESSAGE";
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"id","message","fid","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "ss|iz:msgjob",
                                    kwlist,
                                    &job_id,
                                    &message,
                                    &fid,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_msgjob(self->con_hdl,job_id,fid,message,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :msgjob");
    return NULL;
  }

  Py_RETURN_NONE;

}

/* pbs_orderjob */
static char svr_orderjob_doc[] = 
"Implements pbs_orderjob (see the man page)\n\
orderjob(job_id1,job_id2,[extend]):\n\
\tparameters within [] are optional\n\
Swap the order of two jobs within a single queue.\n\
\tjob_id1    :   Job identifier (sequence_number.server)\n\
\tjob_id2    :   Job identifier (sequence_number.server)\n\
\textend     :    Not used(as per PBS API)\n\
";

static PyObject*
svr_orderjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id1    = (char *)NULL;
  char *job_id2    = (char *)NULL;
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"job_id1","job_id2","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "ss|z:orderjob",
                                    kwlist,
                                    &job_id1,
                                    &job_id2,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_orderjob(self->con_hdl,job_id1,job_id2,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :orderjob");
    return NULL;
  }

  Py_RETURN_NONE;

}

/* pbs_rerunjob */
static char svr_rerunjob_doc[] = 
"Implements pbs_rerunjob (see the man page)\n\
rerunjob(id,[extend]):\n\
\tparameters within [] are optional\n\
ReRun Job request to the batch server identified by id\n\
\tjob_id     :   Job identifier (sequence_number.server)\n\
\textend     :   Not used(as per PBS API)\n\
";

static PyObject*
svr_rerunjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id    = (char *)NULL;
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"job_id","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|z:rerunjob",
                                    kwlist,
                                    &job_id,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_rerunjob(self->con_hdl,job_id,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :rerunjob");
    return NULL;
  }

  Py_RETURN_NONE;

}
/* pbs_rlsjob */
static char svr_rlsjob_doc[] = 
"Implements pbs_rlsjob (see the man page)\n\
rlsjob(id,[hold_type],[extend]):\n\
\tparameters within [] are optional\n\
Swap the order of two jobs within a single queue.\n\
\tid         :   Job identifier (sequence_number.server,string)\n\
\thold_type  :   Hold type (string: 'u' or 'o', 's' : def='u')\n\
\textend     :   Not used(as per PBS API)\n\
";

static PyObject*
svr_rlsjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id   = NULLSTR;
  char *hold_type = (char *)NULL;
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"id","hold_type","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "s|sz:rlsjob",
                                    kwlist,
                                    &job_id,
                                    &hold_type,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_rlsjob(self->con_hdl,job_id,hold_type,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :rlsjob");
    return NULL;
  }

  Py_RETURN_NONE;

}

/* pbs_selectjob */
static char svr_selectjob_doc[] = 
"Implements pbs_selectjob (see the man page)\n\
selectjob([attribs],[extend]):\n\
\tparameters within [] are optional\n\
\tattribs    : This is a python LIST of Attr() objects with op.\n\
\textend     : Not used(as per PBS API)\n\
";

static PyObject*
svr_selectjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  struct attropl *attrib     = (struct attropl *)NULL;
  PyObject *py_attrib  = (PyObject *)NULL; 
  char *extend   = (char *)NULL;
  int free_attrib = 0;
  char **ret_s = (char **)NULL; /* return list */
  PyObject *ret_v = (PyObject *)NULL;

  static char *kwlist[] = {"attribs","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "|Oz:selectjob",
                                    kwlist,
                                    &py_attrib,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }
  if (py_attrib && !IS_SEQUENCE_TYPE(py_attrib)) {
    PyErr_Format(PyExc_TypeError,"'attrib' must be tuple or list type");
    return NULL;
  }
  /* good we have a list */
  if ( (attr_to_c_attropl(py_attrib,&attrib) == FAILED) )
    return NULL;
  free_attrib++; /* make sure we free it */

  /* do the honors */
  ret_s = pbs_selectjob(self->con_hdl,attrib,extend);
  if (free_attrib)
    _free_attropl(attrib);

  if ( !(ret_s) ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :selectjob");
    return NULL;
  }
  /* now we have to build the array list */
  ret_v = argv_to_sequence(ret_s);  /* if it is null then it is ok */
  free(ret_s); /* As per API */
  return ret_v;
}

/* pbs_sigjob */
static char svr_sigjob_doc[] = 
"Implements pbs_sigjob (see the man page)\n\
sigjob(id,signal,[extend]):\n\
\tparameters within [] are optional\n\
\tsend a signal to a batch job.\n\
\tid         :   Job identifier (sequence_number.server,string)\n\
\tsignal     :   signal ('suspend'|'resume'|'1' etc|'KILL' etc )\n\
\textend     :   Not used(as per PBS API)\n\
";

static PyObject*
svr_sigjob(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  char *job_id    = (char *)NULL;
  char *signal    = (char *)NULL;
  char *extend   = (char *)NULL;
  int rc = 0;
  static char *kwlist[] = {"id","signal","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "ss|z:sigjob",
                                    kwlist,
                                    &job_id,
                                    &signal,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  /* do the honors */
  rc = pbs_sigjob(self->con_hdl,job_id,signal,extend);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :sigjob");
    return NULL;
  }

  Py_RETURN_NONE;

}

static char svr_manager_doc[] = 
"Implements pbs_manager (see the man page)\n\
manager(command,obj_type,obj_name,attrib,[extend])\n\
\tparameters within [] are optional\n\
\tcommand :  Operation to perform (CREATE,DELETE,SET,UNSET) (int)\n\
\tobj_type:  Queue/Server/node (integer), see pbs_ifl.h \n\
\tobj_name:  name of the object (string)\n\
\tattrib  :  LIST of Attrop() objects ('op' is used) \n\
\textend  :  Not used(as per PBS API)\n\
\n\
\t return None(0) on success, else PBSError exception is raised\n\
\tNOTES:\n\
\t      Only the ``queue``,``server`` and ``node`` object types are \n\
\t      supported.\n\
";

static PyObject*
svr_manager(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  struct attropl *attrib     = (struct attropl *)NULL;
  PyObject *py_attrib  = (PyObject *)NULL; 
  char *extend   = (char *)NULL;
  int free_attrib = 0;
  int command = -1;
  int obj_type = -1;
  char *obj_name = (char *)NULL;
  int rc = 0; /* success */

  static char *kwlist[] = {"command","obj_type","obj_name",
                           "attribs","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "iisO|z:manager",
                                    kwlist,
                                    &command,
                                    &obj_type,
                                    &obj_name,
                                    &py_attrib,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }
  if (py_attrib && !IS_SEQUENCE_TYPE(py_attrib)) {
    PyErr_Format(PyExc_TypeError,"'attrib' must be tuple or list type");
    return NULL;
  }
  /* good we have a list */
  if ( (attr_to_c_attropl(py_attrib,&attrib) == FAILED) )
    return NULL;
  free_attrib++; /* make sure we free it */

  /* XXX : should check for valid obj_type,command */
  /* do the honors */
  rc = pbs_manager(self->con_hdl,command,obj_type,obj_name,attrib,extend);
  if (free_attrib)
    _free_attropl(attrib);

  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :manager");
    return NULL;
  }
  
  Py_RETURN_NONE;
}

typedef struct batch_status *(*query_func)(int,char *,struct attrl *,char *);

static PyObject *
generic_query(PBSServerObject* self, PyObject* args,PyObject *kwds,
              char *fmt,query_func func) 
{
  char *id = (char *)NULL;
  char *extend = (char *)NULL;
  struct attrl *attrib     = (struct attrl *)NULL;
  PyObject *py_attrib  = (PyObject *)NULL; 
  struct batch_status *bsp  = (struct batch_status *)NULL;
  PyObject            *ret_v = (PyObject *)NULL;
  int free_attrib = 0;

  static char *kwlist[] = {"id","attribs","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    fmt,
                                    kwlist,
                                    &id,
                                    &py_attrib,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }
  if (py_attrib && !IS_SEQUENCE_TYPE(py_attrib)) {
    PyErr_Format(PyExc_TypeError,"'attrib' must be tuple or list type");
    return NULL;
  }
  /* good we have a list */
  if ( (attr_to_c_attrl(py_attrib,&attrib) == FAILED) )
    return NULL;
  free_attrib++; /* make sure we free it */
  
  /* do the honors */
  bsp = func(self->con_hdl,id,attrib,extend);
  if (free_attrib)
    _free_attrl(attrib);

  if ((bsp == NULL) && (pbs_errno != PBSE_NONE)){ /* oops bail!*/
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno,NULL);
    return NULL;
  } 

  if ( !(ret_v = bs_to_py_list(bsp)) ) {
    pbs_statfree(bsp);
    return NULL;
  }
  pbs_statfree(bsp);
  return ret_v;
  
}
static char svr_statserver_doc[] =
"Implements the pbs_statserver (see the man page)\n\
statserver([attribs],[extend]):\n\
\tparameters within [] are optional\n\
\tattrib:    This is python list of max four element TUPLES or None\n\
\t\te.g: [(name,resource,value,op),(name,resource,value,op)...]\n\
\t\t     use \"None\" or empty element for NULL\n\
\textend: Not used\n\
";

static PyObject*
svr_statserver(PBSServerObject* self, PyObject* args,PyObject *kwds)
{
  char                *extend     = (char *)NULL;
  struct attrl        *attrib     = (struct attrl *)NULL;
  PyObject            *py_attrib  = (PyObject *)NULL; 
  struct batch_status *bsp        = (struct batch_status *)NULL;
  PyObject            *ret_v      = (PyObject *)NULL;
  int free_attrib = 0;

  static char *kwlist[] = {"attribs","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                   "|Oz:statserver",
                                   kwlist,
                                   &py_attrib,
                                   &extend
                                    ))
      )
    {
      return NULL;
    }
  if (py_attrib && !IS_SEQUENCE_TYPE(py_attrib)) {
    PyErr_Format(PyExc_TypeError,"'attrib' must be tuple or list type");
    return NULL;
  }
  /* good we have a list */
  if ( (attr_to_c_attrl(py_attrib,&attrib) == FAILED) )
    return NULL;
  free_attrib++; /* make sure we free it */
  
  /* do the honors */
  bsp = pbs_statserver(self->con_hdl,attrib,extend);
  if (free_attrib)
    _free_attrl(attrib);

  if ((bsp == NULL) && (pbs_errno != PBSE_NONE)){ /* oops bail!*/
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno,NULL);
    return NULL;
  } 

  if ( !(ret_v = bs_to_py_list(bsp)) ) {
    pbs_statfree(bsp);
    return NULL;
  }
  pbs_statfree(bsp);
  return ret_v;
}

static char svr_statjob_doc[] = 
"Implements pbs_statjob (see the man page)\n\
statjob([id],[attribs],[extend]):\n\
\tparameters within [] are optional\n\
\tid     :   Job/Dest identifier, can be None/NULL String\n\
\tattribs :   This is a python LIST of Attr() objects \n\
\t           (to return only these attributes).\n\
\textend :   Not used(as per PBS API)\n\
";

static PyObject*
svr_statjob(PBSServerObject* self, PyObject* args,PyObject *kwds)
{
  static char fmt[] = "|zOz:statjob";
  return generic_query(self,args,kwds,fmt,pbs_statjob);
}


static char svr_statque_doc[] = 
"Implements pbs_statque (see the man page)\n\
statque([id],[attribs],[extend]):\n\
\tparameters within [] are optional\n\
\tid: the name of a queue (queue_name), can be None or empty string\n\
\tattribs: This is a LIST of Attr() objects.\n\
\textend: Not used(as per PBS API)\n\
";

static PyObject*
svr_statque(PBSServerObject* self, PyObject* args,PyObject *kwds)
{
  static char fmt[] = "|zOz:statque";
  return generic_query(self,args,kwds,fmt,pbs_statque);
}

static char svr_statnode_doc[] = 
"Implements pbs_statnode (see the man page)\n\
statnode([id],[attribs],[extend]):\n\
\tparameters within [] are optional\n\
\tid: the name of a node (node_name), can be None or empty string\n\
\tattribs: This is a LIST of Attr() objects.\n\
\textend: Not used(as per PBS API)\n\
";

static PyObject*
svr_statnode(PBSServerObject* self, PyObject* args,PyObject *kwds)
{
  static char fmt[] = "|zOz:statnode";
  return generic_query(self,args,kwds,fmt,pbs_statnode);
}

static char svr_selstat_doc[] = 
"Implements pbs_selstat (see the man page)\n\
selstat(connect,[select_list],[attribs]):\n\
\tparameters within [] are optional\n\
\tconnect    :   This is the connection id received from the connect() call\n\
\tselect_list: This is a python LIST of Attr() objects with 'op' specified\n\
\t             (the selection criterion.\n\
\tattribs    : This is a list of Attr() instances, specifying the attributes\n\
\t             of interest to return with jobs matching the selection \n\
\t             criterion.\n\
\textend     : Not used(as per PBS API)\n\
\tNOTE:\n\
\tOpenPBS has the following signature:\n\
\tselstat(connect,[select_list],extend)\n\
";

static PyObject*
svr_selstat(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  struct attropl *select_list     = (struct attropl *)NULL;
  PyObject *py_select_list  = (PyObject *)NULL; 
  int free_select_list = 0;
#ifdef FOUR_ARGS_PBS_SELSTAT
  struct attrl *attrib     = (struct attrl *)NULL;
  PyObject *py_attrib  = (PyObject *)NULL; 
  int free_attrib = 0;
#endif

  char *extend   = (char *)NULL;
  struct batch_status *bsp        = (struct batch_status *)NULL;
  PyObject *ret_v = (PyObject *)NULL;

  static char *kwlist[] = {"select_list",
#ifdef FOUR_ARGS_PBS_SELSTAT
                           "attribs",
#endif
                           "extend",
                           NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
#ifdef FOUR_ARGS_PBS_SELSTAT
                                    "|OOz:selstat",
#else
                                    "|OO:selstat",
#endif
                                    kwlist,
#ifdef FOUR_ARGS_PBS_SELSTAT
                                    &py_attrib,
#endif
                                    &extend
                                    ))
      )
    {
      return NULL;
    }
#ifdef FOUR_ARGS_PBS_SELSTAT
  if (py_attrib && !IS_SEQUENCE_TYPE(py_attrib)) {
    PyErr_Format(PyExc_TypeError,"'attrib' must be tuple or list type");
    return NULL;
  }
#endif
  if (py_select_list && !IS_SEQUENCE_TYPE(py_select_list)) {
    PyErr_Format(PyExc_TypeError,"'select_list' must be tuple or list type");
    return NULL;
  }

  /* good we have a list */
  if ( (attr_to_c_attropl(py_select_list,&select_list) == FAILED) )
    return NULL;

  free_select_list++; /* make sure we free it */

#ifdef FOUR_ARGS_PBS_SELSTAT
  /* good we have a list */
  if ( (attr_to_c_attrl(py_attrib,&attrib) == FAILED) ) {
    if (free_select_list)
      _free_attropl(select_list);
    return NULL;
  }
  free_attrib++;
#endif

  /* do the honors */
#ifdef FOUR_ARGS_PBS_SELSTAT
  bsp = pbs_selstat(self->con_hdl,select_list,attrib,extend);
#else
  bsp = pbs_selstat(self->con_hdl,select_list,extend);
#endif
  if (free_select_list)
    _free_attropl(select_list);
#ifdef FOUR_ARGS_PBS_SELSTAT
  if (free_attrib)
    _free_attrl(attrib);
#endif

  if ((bsp == NULL) && (pbs_errno != PBSE_NONE)){ /* oops bail!*/
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno,NULL);
    return NULL;
  } 

  if ( !(ret_v = bs_to_py_list(bsp)) ) {
    pbs_statfree(bsp);
    return NULL;
  }
  pbs_statfree(bsp);
  return ret_v;
}

static char svr_terminate_doc[] = 
"Implements pbs_terminate (see the man page)\n\
\tterminate(manner,[extend]):\n\
\tparameters within [] are optional\n\
\tmanner     : an integer with values\n\
\t             0 => SHUT_IMMEDIATE\n\
\t             1 => SHUT_DELAY\n\
\t             2 => SHUT_QUICK (DEFAULT)\n\
\textend     : Not used\n\
";

static PyObject*
svr_terminate(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  int manner = 2 ; /* SHUT_QUICK */
  char *extend = (char *)NULL;
  int rc = 0; /* success */

  static char *kwlist[] = {"manner","extend",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "|iz:terminate",
                                    kwlist,
                                    &manner,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  rc = pbs_terminate(self->con_hdl,manner,extend);
  if (rc != 0 ) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :terminate");
    return NULL;
  }
  
  Py_RETURN_NONE;
}

/*
**  ALL RESOURCE stuff 
*/

static char svr_rescquery_doc[] = 
"Implements pbs_rescquery (see the man page)\n\
rescquery(rlist):\n\
\trlist:     python LIST of resources (-l ) only 'nodes' is supported.\n\
";
static PyObject*
svr_rescquery(PBSServerObject* self, PyObject* args,PyObject *kwds)
{
  char *extend = (char *)NULL;
  char **rlist = (char **)NULL;
  int  n_rlist  = 0;   /* num of elements in rlist */
  int free_rlist = 0;
  PyObject *py_rlist = (PyObject *)py_rlist;
  static char *name[]    = {"available","allocated","reserved","down"};
  int  *all[]     = {NULL,       NULL,        NULL,      NULL};
  int n_all = sizeof all / sizeof all[0];
  PyObject *ret_v      = NULL; /* a dictionary to return */
  PyObject *tmp = (PyObject *)NULL;
  int  rc = 0;
  int  i;
  int  j;
  int add_null = 0;  /* don't append a NULL string */
  static char *kwlist[] = {"rlist","extend",NULL};

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "O|z:rescquery",
                                    kwlist,
                                    &py_rlist,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }

  if (!IS_SEQUENCE_TYPE(py_rlist)) {
    PyErr_Format(PyExc_TypeError,"`rlist` must be a list or tuple sequence");
    return NULL;
  }
  if ( (sequence_to_argv(py_rlist,&n_rlist,&rlist,add_null) != 0) ) {
    return NULL;
  }
  free_rlist++;

  /* allocate the individual arrays in all */
  for (i = 0;i < n_all; i++) {
    if (!(all[i] = PyMem_New(int,n_rlist)))
      goto cleanup;
  }

  rc = pbs_rescquery(self->con_hdl,rlist,n_rlist,
                     all[0],all[1],all[2],all[3]);
  if (rc != 0) { /* oops something went wrong */
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno,":rescquery");
    goto cleanup;
  }

  /* build the dict to return */
  if ( !(ret_v = PyDict_New()) )
    goto cleanup;
  
  for(i = 0; i < n_all; i++) {
    if ( !(tmp = PyTuple_New(n_rlist)) )
      goto free_ret_v;
    for(j = 0; j < n_rlist; j++) {
      PyTuple_SET_ITEM(tmp,j,PyInt_FromLong(all[i][j]));
    }
    /* now insert into the dictionary */
    if ( (PyDict_SetItemString(ret_v,name[i],tmp) == -1) ) {
      /* man, major cleanup */
      Py_DECREF(tmp);
      goto free_ret_v;
    }
    /* good, decref the tmp as dict already has one */
    Py_DECREF(tmp);
  } /* for(i = 0 .... */

  /* Great ,no errors  we should bolt to cleanup */
  goto cleanup;

 free_ret_v:
  Py_DECREF(ret_v);
  ret_v = (PyObject *) NULL;

 cleanup:
  if (free_rlist)
    PyMem_Free(rlist);
  for(i = 0; i < n_all; i++)
    PyMem_Free(all[i]);

  return ret_v;
}

/* totpool */
static char svr_totpool_doc[] = 
"Implements totpool() (see the man page for pbs_rescquery)\n\
totpool([update]):\n                                        \
the parameters in [] are optional \n\
\tupdate :   if nonzero then refresh(int)\n\
\n\
\treturns the total  number  of  nodes  known  to  the  PBS server.\n\
\tThis  is the sum of the number of nodes available, allocated, reserved,\n\
\tand down.\n\
";

static PyObject*
svr_totpool(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  int update = 0; /* whether to refresh */
  int rc = 0;
  static char *kwlist[] = {"update",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "|i:totpool",
                                    kwlist,
                                    &update
                                    ))
      )
    {
      return NULL;
    }
  rc = totpool(self->con_hdl,update);
  if (rc != 0) {
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :totpool");
    return NULL;
  }
  return PyInt_FromLong(rc);
}

/* usepool */
static char svr_usepool_doc[] = 
"Implements usepool() (see the man page for pbs_rescquery)\n\
usepool([update]):\n\
the parameters in [] are optional \n\
returns the number of nodes currently in use,alloc+reser+down\n\
\tupdate :   if nonzero then refresh(int)\n\
";

static PyObject*
svr_usepool(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  int update = 0; /* whether to refresh */
  int rc = 0;
  static char *kwlist[] = {"update",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "|i:usepool",
                                    kwlist,
                                    &update
                                    ))
      )
    {
      return NULL;
    }
  rc = usepool(self->con_hdl,update);
  if (rc != 0) {
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :usepool");
    return NULL;
  }
  return PyInt_FromLong(rc);
}

/* pbs_submit */
static char svr_submit_doc[] =
"Implements the pbs_submit (see the man page)\n\
Issue a batch request to submit a new batch job\n\
submit([attribs],[script],[destination],[extend]):\n\
\tparameters within [] are optional\n\
\tattribs     : This is python LIST of Atrrop() objects, \n\
\t             please see Attrop.__doc__\n\
\tscript     : The path name to the job script cane be None or \"\"\n\
\tdestination: the name of the queue can be None or \"\"\n\
\textend     : Not used\n\
";
static PyObject*
svr_submit(PBSServerObject *self, PyObject *args, PyObject *kwds)
{

  struct attropl *attrib     = (struct attropl *)NULL;
  PyObject *py_attrib  = (PyObject *)NULL; 
  int free_attrib = 0;
  char *script = (char *)NULL;
  char *dest = NULLSTR;
  char *extend = (char *)NULL;
  char *job_id = (char *)NULL; /* returned from pbs_submit */
  PyObject *ret_v = (PyObject *)NULL;
  static char *kwlist[] = {"attribs","script","dest",
                           "extend",NULL};

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "|Ozsizz:submit",
                                    kwlist,
                                    &py_attrib,
                                    &script,
                                    &dest,
                                    &extend
                                    ))
      )
    {
      return NULL;
    }
  if (py_attrib && !IS_SEQUENCE_TYPE(py_attrib)) {
    PyErr_Format(PyExc_TypeError,"'attrib' must be tuple or list type");
    return NULL;
  }
  /* good we have a list */
  if ( (attr_to_c_attropl(py_attrib,&attrib) == FAILED) )
    return NULL;
  free_attrib++; /* make sure we free it */


  if ( !(job_id = pbs_submit(self->con_hdl,attrib,script,dest,extend)) ) {
    goto done;
  }
  ret_v = PyString_FromString(job_id);
  free(job_id);
 done:
  if (free_attrib)
    _free_attropl(attrib);
  return ret_v;
}

static char svr_rescreserve_doc[] = 
"Implements pbs_rescreserve (see the man page)\n\
rescreserve(rlist,[resource_id]):\n\
\trlist      : python LIST of resources (-l ) only 'nodes' supported\n\
\tresource_id: None for new reservation, else existing resource_id (int)\n\
";

static PyObject*
svr_rescreserve(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  PyObject *py_rlist = (PyObject *)NULL;
  PyObject *py_rid   = (PyObject *)NULL;

  resource_t rid = RESOURCE_T_NULL; /* default */
  char **rlist = (char **)NULL;
  int  nresc = 0; 
  int rc = 0;
  static char *kwlist[] = {"rlist","resource_id",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "O|O:rescreserve",
                                    kwlist,
                                    &py_rlist,
                                    &py_rid
                                    ))
      )
    {
      return NULL;
    }
  if (py_rid) {
    if ((py_rid == Py_None)) {
      /* do nothing leave rid alone */
    } else if ((PyInt_Check(py_rid))) {
      rid = (resource_t)PyInt_AsLong(py_rid);
    } else { /* bad value */
      PyErr_Format(PyExc_TypeError,"rid must be an integer or value returned"
                   " from a previous query call");
      return NULL;
    }
  } /* end py_rid */
  if ((py_rlist)) {
    if ( (sequence_to_argv(py_rlist,&nresc,&rlist,0) == -1) ) {
      return NULL;
    }
  }
  rc = pbs_rescreserve(self->con_hdl,rlist,nresc,&rid);
  PyMem_Free(rlist); /* make sure we free it */
  if (rc != 0) {
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :rescreserve");
    return NULL;
  }
  
  return PyInt_FromLong(rid);
}

static char svr_rescrelease_doc[] = 
"Implements pbs_rescrelease (see the man page)\n\
rescrelease(resource_id):\n\
\tresource_id: existing resource_id (int)\n\
";

static PyObject*
svr_rescrelease(PBSServerObject *self, PyObject *args, PyObject *kwds)
{
  PyObject *py_rid   = (PyObject *)NULL;

  resource_t rid = RESOURCE_T_NULL; /* default */
  int rc = 0;
  static char *kwlist[] = {"resource_id",NULL};

  RETURN_INVALID_HDL;  /* blow out if connection not initialized */

  if (!(PyArg_ParseTupleAndKeywords(args,kwds,
                                    "O|O:rescreserve",
                                    kwlist,
                                    &py_rid
                                    ))
      )
    {
      return NULL;
    }
  if (py_rid) {
    if ((py_rid == Py_None)) {
      /* do nothing leave rid alone */
    } else if ((PyInt_Check(py_rid))) {
      rid = (resource_t)PyInt_AsLong(py_rid);
    } else { /* bad value */
      PyErr_Format(PyExc_TypeError,"rid must be an integer or value returned"
                   " from a previous query call");
      return NULL;
    }
  } /* end py_rid */
  rc = pbs_rescrelease(self->con_hdl,rid);
  if (rc != 0) {
    SET_PBS_EXCEPTION(self->con_hdl,pbs_errno," :rescreserve");
    return NULL;
  }
  
  Py_RETURN_NONE;
}




static PyMethodDef svr__methods[] = {
  {"avail",(PyCFunction) svr_avail, 
   METH_KEYWORDS,svr_avail_doc},                       /* avail */
  {"asyrunjob",(PyCFunction) svr_asyrunjob, 
   METH_KEYWORDS,svr_asyrunjob_doc},                   /* asyrunjob */
  {"alterjob",(PyCFunction) svr_alterjob, 
   METH_KEYWORDS,svr_alterjob_doc},                    /* alterjob */
  {"connect",(PyCFunction) svr_connect,  
   METH_KEYWORDS,svr_connect_doc},
  {"open",(PyCFunction) svr_connect, 
   METH_KEYWORDS,svr_connect_doc},                     /* alias for connect */
  {"disconnect",(PyCFunction) svr_disconnect, 
   METH_NOARGS,svr_disconnect_doc},
  {"close",(PyCFunction) svr_disconnect, 
   METH_NOARGS,svr_disconnect_doc},                    /* alias for disconn. */
  {"deljob",(PyCFunction) svr_deljob, 
   METH_KEYWORDS,svr_deljob_doc},                      /* deljob */
  {"holdjob",(PyCFunction) svr_holdjob, 
   METH_KEYWORDS,svr_holdjob_doc},                     /* holdjob */
  {"locjob",(PyCFunction) svr_locjob, 
   METH_KEYWORDS,svr_locjob_doc},                      /* locjob */
  {"movejob",(PyCFunction) svr_movejob, 
   METH_KEYWORDS,svr_movejob_doc},                     /* movejob */
  {"msgjob",(PyCFunction) svr_msgjob, 
   METH_KEYWORDS,svr_msgjob_doc},                      /* msgjob */
  {"orderjob",(PyCFunction) svr_orderjob, 
   METH_KEYWORDS,svr_orderjob_doc},                    /* orderjob */
  {"rerunjob",(PyCFunction) svr_rerunjob, 
   METH_KEYWORDS,svr_rerunjob_doc},                    /* rerunjob */
  {"rlsjob",(PyCFunction) svr_rlsjob, 
   METH_KEYWORDS,svr_rlsjob_doc},                      /* rlsjob */
  {"selectjob",(PyCFunction) svr_selectjob, 
   METH_KEYWORDS,svr_selectjob_doc},                   /* selectjob */
  {"sigjob",(PyCFunction) svr_sigjob, 
   METH_KEYWORDS,svr_sigjob_doc},                      /* sigjob */
  {"manager",(PyCFunction) svr_manager, 
   METH_KEYWORDS,svr_manager_doc},                     /* manager */
  {"statserver",(PyCFunction) svr_statserver, 
   METH_KEYWORDS,svr_statserver_doc},                  /* statserver */
  {"statjob",(PyCFunction) svr_statjob, 
   METH_KEYWORDS,svr_statjob_doc},                     /* statjob */
  {"statque",(PyCFunction) svr_statque, 
   METH_KEYWORDS,svr_statque_doc},                     /* statque */
  {"statnode",(PyCFunction) svr_statnode, 
   METH_KEYWORDS,svr_statnode_doc},                     /* statnode */  
  {"selstat",(PyCFunction) svr_selstat, 
   METH_KEYWORDS,svr_selstat_doc},                     /* selstat */
  {"terminate",(PyCFunction) svr_terminate, 
   METH_KEYWORDS,svr_terminate_doc},                   /* terminate */
  {"rescquery",(PyCFunction) svr_rescquery, 
   METH_KEYWORDS,svr_rescquery_doc},                   /* rescquery */
  {"totpool",(PyCFunction) svr_totpool, 
   METH_KEYWORDS,svr_totpool_doc},                     /* totpool */
  {"usepool",(PyCFunction) svr_usepool, 
   METH_KEYWORDS,svr_usepool_doc},                     /* usepool */
  {"submit",(PyCFunction) svr_submit, 
   METH_KEYWORDS,svr_submit_doc},                      /* submit */
  {"rescreserve",(PyCFunction) svr_rescreserve, 
   METH_KEYWORDS,svr_rescreserve_doc},                 /* rescreserve */
  {"rescrelease",(PyCFunction) svr_rescrelease, 
   METH_KEYWORDS,svr_rescrelease_doc},                 /* rescrelease */
  {NULL}  /* Sentinel */
};

/* ######################   TYPE DEFINITION   ############################# */

static char svr__doc[] =
"Server()\n\
";

static PyTypeObject PBSServer_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                                                 /* ob_size*/
    TYPEOPBJECT_MODULE_PREFIX "Server",                /* tp_name*/
    sizeof(PBSServerObject),                           /* tp_basicsize*/
    0,                                                 /* tp_itemsize*/
    (destructor)svr__dealloc,                          /* tp_dealloc*/
    0,                                                 /* tp_print*/
    0,                                                 /* tp_getattr*/
    0,                                                 /* tp_setattr*/
    0,                                                 /* tp_compare*/
    (reprfunc)svr__repr,                               /* tp_repr*/
    0,                                                 /* tp_as_number*/
    0,                                                 /* tp_as_sequence*/
    0,                                                 /* tp_as_mapping*/
    0,                                                 /* tp_hash */
    0,                                                 /* tp_call*/
    0,                                                 /* tp_str*/
    0,                                                 /* tp_getattro*/
    0,                                                 /* tp_setattro*/
    0,                                                 /* tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,          /* tp_flags*/
    svr__doc,                                          /* tp_doc */
    0,		                                           /* tp_traverse */
    0,		                                           /* tp_clear */
    0,		                                           /* tp_richcompare */
    0,		                                           /* tp_weaklistoffset */
    0,		                                           /* tp_iter */
    0,		                                           /* tp_iternext */
    svr__methods,                                      /* tp_methods */
    0,                                                 /* tp_members */
    svr__getsets,                                      /* tp_getset */
    0,                                                 /* tp_base */
    0,                                                 /* tp_dict */
    0,                                                 /* tp_descr_get */
    0,                                                 /* tp_descr_set */
    0,                                                 /* tp_dictoffset */
    (initproc)svr__init,                               /* tp_init */
    0,                                                 /* tp_alloc */
    (newfunc)svr__new,                                 /* tp_new */
};


/* @@@@@ UNDEFINE ALL THE DEFINES IN THE BEGINNING                 @@@@@ */

#undef VALID_CONNECT_HDL
#undef VALID_PORT
#undef RETURN_INVALID_HDL
