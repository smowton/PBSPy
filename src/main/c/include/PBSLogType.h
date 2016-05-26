#ifndef PBSPY_LOGTYPE___H
#define PBSPY_LOGTYPE___H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h> /* for offsetof */
#include <Python.h>
#include <log.h>


/* 
** PBSLog_Type
*/


typedef struct {
  PyObject_HEAD
  long mask;                           /* mask */
  int free_daemon_name;                /* if we need to free name */
  char *daemon_name;                   /* name to spit out */
  PyObject *py_daemon_name;            /* a copy of above  */
  char *filename;                      /* absolute path for log */
  char *directory;                     /* log directory */
} PBSLogObject;

staticforward PyTypeObject PBSLog_Type;

#define PBSLogType_Check(op)        PyObject_TypeCheck(op, &PBSLog_Type)


#ifdef __cplusplus
}
#endif

#endif /* !PBSPY_LOGTYPE___H */
 
