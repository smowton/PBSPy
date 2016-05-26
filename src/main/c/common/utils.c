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
** set_dict_string
** Returns: 0 on success, -1 on failure.
** NOTE: if value is NULL, then None is inserted.
*/

static int
set_dict_string(PyObject *d,char *k,const char *v)
{
  PyObject *tmp = (PyObject *)NULL;
  int rc = 0;
  if (v) {
    tmp = PyString_FromString(v);
  } else {
    Py_INCREF(Py_None);
    tmp = Py_None;
  }
  if (!tmp) {
    return -1;
  }
  rc = PyDict_SetItemString(d,k,tmp);
  Py_DECREF(tmp);
  return rc;
}
/*
** 
*/
static PyObject *
argv_count_to_sequence(char **argv,int count)
{
  PyObject *rv = (PyObject *)NULL;
  int i;

  if ( (rv = PyList_New(count)) == NULL ) {
    return rv;
  }
  for (i = 0; i < count; i++) {
    PyObject *tmp = PyString_FromString(argv[i]);
    if (tmp) {
      if (PyList_SetItem(rv,i,tmp) == -1) { /* NOTE: steals ref to tmp */
        Py_DECREF(tmp);Py_DECREF(rv); rv = NULL;
        break;
      } 
    } else {
      Py_DECREF(rv); rv = NULL;
      break;
    } /* if (tmp) */
  }
  return rv;
}

/*
** Take a NULL terminated argv list and return a New Reference to a list
** of strings.
*/
static PyObject *
argv_to_sequence(char **argv)
{
  PyObject *av;
  PyObject *item;

  av = PyList_New(0);
  if (av != NULL) {
    while(*argv != (char *)NULL) {
      if ( (item = PyString_FromString(*argv)) == (PyObject *)NULL) {
        Py_DECREF(av); av = (PyObject *)NULL;
        break;
      } else {
        PyList_Append(av,item);
        Py_DECREF(item);
      }
      argv++;
    }
  }
  return (av);
}

/*
** NOTES:
**    - Caller MUST free the argv_p with PyMem_Free
** changed added add_null
*/
static int
sequence_to_argv(PyObject *arglist,int *argc_p,char ***argv_p,int add_null)
{
  int i;
  int argc;
  char **argv;
  PyObject *v;

  if (!((PyTuple_Check(arglist)) || (PyList_Check(arglist)))) {
    PyErr_Format(PyExc_TypeError,"must be a tuple or list of strings");
    return -1;
  }

  *argc_p = argc = PySequence_Size(arglist);
  if (add_null) {
    *argv_p = argv = PyMem_New(char *,(argc + 1));
  } else {
    *argv_p = argv = PyMem_New(char *,argc);
  }

  if (argv == NULL) {
    return -1;
  }

  memset((char *)argv,0,sizeof argv); /* zero the bytes */

  for  (i = 0; i < argc; i++) {
    v = PySequence_GetItem(arglist, i);
    if (v == NULL) {
      goto cleanup;
    }
    if (!PyString_Check(v)) {
      PyErr_SetString(PyExc_TypeError, 
                      "need list of strings as argument");
      goto cleanup;
    }

    argv[i] = PyString_AsString(v); /* NOTE: MUST NOT free the string */
  }
  
  return 0;
  
cleanup:
  if (argv != NULL) {
    PyMem_Free(argv);
    *argv_p = NULL;
    *argc_p = 0;
  }
  return -1;
}




/*
** helper function to introduce in the module dictionary
*/

static int
_insert_str_const(PyObject *d, char *name, char *code)
{
  PyObject *u = PyString_FromString(name);
  PyObject *v = PyString_FromString(code);
  int status = -1;
  /*
  ** error checking will be done later
  */
  if (u && v) {
    /* insert in modules dict */
    status = PyDict_SetItem(d, u, v);
  }
  Py_XDECREF(u);
  Py_XDECREF(v);
  return status;
}

/*
** helper function to introduce in the module dictionary
*/

static int
_insert_int_const(PyObject *d, char *name,int code)
{
  PyObject *u = PyString_FromString(name);
  PyObject *v = PyInt_FromLong(code);
  int status = -1;
  if (u && v) {
    /* insert in modules dict */
    status = PyDict_SetItem(d, u, v);
  }
  Py_XDECREF(u);
  Py_XDECREF(v);
  return status;
}

/*
** helper function to introduce in the module dictionary
*/

static int
_insert_long_const(PyObject *d, char *name,long code)
{
  PyObject *u = PyString_FromString(name);
  PyObject *v = PyLong_FromLong(code);
  int status = -1;
  if (u && v) {
    /* insert in modules dict */
    status = PyDict_SetItem(d, u, v);
  }
  Py_XDECREF(u);
  Py_XDECREF(v);
  return status;
}

/*
**=========================================================================**
**               Object Converters for PyArgParse                          **
**=========================================================================**
*/


/*
** convert_to_boolean_int
**   Converter for a Python Object passed from the interpreter to the C/API
**   to a boolean integer 0 (false) or 1 (true).
**
**
*/

static int
convert_to_boolean_int(PyObject *arg, void *valuep)
{
  BoolArg *ret = (BoolArg *)valuep;
  int rv = 1;

  ret->defined = 1; /* set the defined flag since we are here!! */
#ifdef PyBool_Check
  if (PyBool_Check(arg)) {
    ret->value = (arg == Py_True) ? 1 : 0;
  } else if (PyInt_Check(arg)) {
#else                              /* not defined */
  if (PyInt_Check(arg)) {
#endif                             /* PyBool_Check */
    int val = PyInt_AsLong(arg);
    if ((val != 0) || (val != 1)) 
      goto ERROR;
    ret->value = val;
  }

  return rv;

 ERROR:
  rv = 0;
  PyErr_Format(PyExc_TypeError,
               "Illegal %s: need a boolean or integer value 0 or 1",
               ((BoolArg *)valuep)->name);
  return 0;
  
}

/* converter for keyword args or String or None, mainly used in Parse..Args*/
static int
convert_to_string(PyObject *arg,void *store)
{
  PyObject *new_val = (PyObject *)NULL;
  if (arg == Py_None) {
    Py_INCREF(Py_None);
    new_val = Py_None;
  } else if (PyString_Check(arg)) {
    Py_INCREF(arg);
    new_val = arg;
  } else {
    PyErr_Format(PyExc_TypeError,
                 "Illegal value for must be string or None");
    return FAILED;
  }

  /* ok now copy it into the store which must be of type PyObject ** */
  Py_CLEAR(*(PyObject **)store);
  *(PyObject **)store = new_val;
  return SUCCESS;

}

/*
** convert_to_complex_string
**   Converter for a Python String Object passed to C/API, where it None value
**   is special, in the sense that it defines a Null value.
*/

static int
convert_to_complex_string(PyObject *arg, void *valuep)
{
  CStringArg *ret = (CStringArg *)valuep;
  int rv = 1;
  
  ret->arg = arg; /* just store a reference, *MUST* not be passed back */
  if (arg == Py_None) {
    ret->defined = 1;
    ret->value = 0;
  } else if (PyString_Check(arg)) {
    ret->defined = 1;
    ret->value = PyString_AsString(arg);
  } else {
    PyErr_Format(PyExc_TypeError,
                 "Illegal %s: need a string value or None",
                 ret->name);
    rv = 0;
  }
  
  return rv;
}

