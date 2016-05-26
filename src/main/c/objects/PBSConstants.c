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

#include "../include/utils.h"
#include "../include/OptionType.h"

/*
** All relevant constants go here, these are basically from the pbs_ifl #defines that you want to be included.
** TODO: double check with header files, there are really not needed, but it would immensly help people who
**       have worked extensively with the C API.
*/

static int insert_st = 0;             /* for use to return status */

/*
** The module object gets an attribute names "attr" that has all this constants 
*/
static int
insert_attr_flags(PyObject *mod_d)
{
  PyObject *attr_d = (PyObject *)OptionType_New(1);
  if (attr_d == NULL)
    return -1;

  insert_st = 0;

#ifdef ATTR_a
  insert_st += _setattr_str_const(attr_d,"a",ATTR_a);
  insert_st += _insert_str_const(mod_d,"ATTR_a",ATTR_a);
#endif

#ifdef ATTR_c
  insert_st += _setattr_str_const(attr_d,"c",ATTR_c);
  insert_st += _insert_str_const(mod_d,"ATTR_c",ATTR_c);
#endif
#ifdef ATTR_g
  insert_st += _setattr_str_const(attr_d,"g",ATTR_g);
  insert_st += _insert_str_const(mod_d,"ATTR_g",ATTR_g);
#endif
#ifdef ATTR_h
  insert_st += _setattr_str_const(attr_d,"h",ATTR_h);
  insert_st += _insert_str_const(mod_d,"ATTR_h",ATTR_h);
#endif
#ifdef ATTR_j
  insert_st += _setattr_str_const(attr_d,"j",ATTR_j);
  insert_st += _insert_str_const(mod_d,"ATTR_j",ATTR_j);
#endif

#ifdef ATTR_J
  insert_st += _setattr_str_const(attr_d,"J",ATTR_J);
  insert_st += _insert_str_const(mod_d,"ATTR_J",ATTR_J);
#endif

#ifdef ATTR_k
  insert_st += _setattr_str_const(attr_d,"k",ATTR_k);
  insert_st += _insert_str_const(mod_d,"ATTR_k",ATTR_k);
#endif
#ifdef ATTR_l
  insert_st += _setattr_str_const(attr_d,"l",ATTR_l);
  insert_st += _insert_str_const(mod_d,"ATTR_l",ATTR_l);
#endif
#ifdef ATTR_m
  insert_st += _setattr_str_const(attr_d,"m",ATTR_m);
  insert_st += _insert_str_const(mod_d,"ATTR_m",ATTR_m);
#endif

#ifdef ATTR_o
  insert_st += _setattr_str_const(attr_d,"o",ATTR_o);
  insert_st += _insert_str_const(mod_d,"ATTR_o",ATTR_o);
#endif
#ifdef ATTR_p
  insert_st += _setattr_str_const(attr_d,"p",ATTR_p);
  insert_st += _insert_str_const(mod_d,"ATTR_p",ATTR_p);
#endif
#ifdef ATTR_q
  insert_st += _setattr_str_const(attr_d,"q",ATTR_q);
  insert_st += _insert_str_const(mod_d,"ATTR_q",ATTR_q);
#endif
#ifdef ATTR_r
  insert_st += _setattr_str_const(attr_d,"r",ATTR_r);
  insert_st += _insert_str_const(mod_d,"ATTR_r",ATTR_r);
#endif
#ifdef ATTR_u
  insert_st += _setattr_str_const(attr_d,"u",ATTR_u);
  insert_st += _insert_str_const(mod_d,"ATTR_u",ATTR_u);
#endif
#ifdef ATTR_v
  insert_st += _setattr_str_const(attr_d,"v",ATTR_v);
  insert_st += _insert_str_const(mod_d,"ATTR_v",ATTR_v);
#endif
#ifdef ATTR_A
  insert_st += _setattr_str_const(attr_d,"A",ATTR_A);
  insert_st += _insert_str_const(mod_d,"ATTR_A",ATTR_A);
#endif
#ifdef ATTR_M
  insert_st += _setattr_str_const(attr_d,"M",ATTR_M);
  insert_st += _insert_str_const(mod_d,"ATTR_M",ATTR_M);
#endif
#ifdef ATTR_N
  insert_st += _setattr_str_const(attr_d,"N",ATTR_N);
  insert_st += _insert_str_const(mod_d,"ATTR_N",ATTR_N);
#endif
#ifdef ATTR_S
  insert_st += _setattr_str_const(attr_d,"S",ATTR_S);
  insert_st += _insert_str_const(mod_d,"ATTR_S",ATTR_S);
#endif

#ifdef ATTR_depend
  insert_st += _setattr_str_const(attr_d,"depend",ATTR_depend);
  insert_st += _insert_str_const(mod_d,"ATTR_depend",ATTR_depend);
#endif

#ifdef ATTR_inter
  insert_st += _setattr_str_const(attr_d,"inter",ATTR_inter);
  insert_st += _insert_str_const(mod_d,"ATTR_inter",ATTR_inter);
#endif
#ifdef ATTR_stagein
  insert_st += _setattr_str_const(attr_d,"stagein",ATTR_stagein);
  insert_st += _insert_str_const(mod_d,"ATTR_stagein",ATTR_stagein);
#endif
#ifdef ATTR_stageout
  insert_st += _setattr_str_const(attr_d,"stageout",ATTR_stageout);
  insert_st += _insert_str_const(mod_d,"ATTR_stageout",ATTR_stageout);
#endif

#ifdef ATTR_auth_u
  insert_st += _setattr_str_const(attr_d,"auth_u",ATTR_auth_u);
  insert_st += _insert_str_const(mod_d,"ATTR_auth_u",ATTR_auth_u);
#endif
#ifdef ATTR_auth_g
  insert_st += _setattr_str_const(attr_d,"auth_g",ATTR_auth_g);
  insert_st += _insert_str_const(mod_d,"ATTR_auth_g",ATTR_auth_g);
#endif
#ifdef ATTR_auth_h
  insert_st += _setattr_str_const(attr_d,"auth_h",ATTR_auth_h);
  insert_st += _insert_str_const(mod_d,"ATTR_auth_h",ATTR_auth_h);
#endif
#ifdef ATTR_pwd
  insert_st += _setattr_str_const(attr_d,"pwd",ATTR_pwd);
  insert_st += _insert_str_const(mod_d,"ATTR_pwd",ATTR_pwd);
#endif
#ifdef ATTR_cred
  insert_st += _setattr_str_const(attr_d,"cred",ATTR_cred);
  insert_st += _insert_str_const(mod_d,"ATTR_cred",ATTR_cred);
#endif

#ifdef ATTR_ctime
  insert_st += _setattr_str_const(attr_d,"ctime",ATTR_ctime);
  insert_st += _insert_str_const(mod_d,"ATTR_ctime",ATTR_ctime);
#endif
#ifdef ATTR_exechost
  insert_st += _setattr_str_const(attr_d,"exechost",ATTR_exechost);
  insert_st += _insert_str_const(mod_d,"ATTR_exechost",ATTR_exechost);
#endif
#ifdef ATTR_mtime
  insert_st += _setattr_str_const(attr_d,"mtime",ATTR_mtime);
  insert_st += _insert_str_const(mod_d,"ATTR_mtime",ATTR_mtime);
#endif
#ifdef ATTR_qtime
  insert_st += _setattr_str_const(attr_d,"qtime",ATTR_qtime);
  insert_st += _insert_str_const(mod_d,"ATTR_qtime",ATTR_qtime);
#endif
#ifdef ATTR_session
  insert_st += _setattr_str_const(attr_d,"session",ATTR_session);
  insert_st += _insert_str_const(mod_d,"ATTR_session",ATTR_session);
#endif
#ifdef ATTR_euser
  insert_st += _setattr_str_const(attr_d,"euser",ATTR_euser);
  insert_st += _insert_str_const(mod_d,"ATTR_euser",ATTR_euser);
#endif
#ifdef ATTR_egroup
  insert_st += _setattr_str_const(attr_d,"egroup",ATTR_egroup);
  insert_st += _insert_str_const(mod_d,"ATTR_egroup",ATTR_egroup);
#endif
#ifdef ATTR_hashname
  insert_st += _setattr_str_const(attr_d,"hashname",ATTR_hashname);
  insert_st += _insert_str_const(mod_d,"ATTR_hashname",ATTR_hashname);
#endif
#ifdef ATTR_hopcount
  insert_st += _setattr_str_const(attr_d,"hopcount",ATTR_hopcount);
  insert_st += _insert_str_const(mod_d,"ATTR_hopcount",ATTR_hopcount);
#endif
#ifdef ATTR_security
  insert_st += _setattr_str_const(attr_d,"security",ATTR_security);
  insert_st += _insert_str_const(mod_d,"ATTR_security",ATTR_security);
#endif
#ifdef ATTR_sched_hint
  insert_st += _setattr_str_const(attr_d,"sched_hint",ATTR_sched_hint);
  insert_st += _insert_str_const(mod_d,"ATTR_sched_hint",ATTR_sched_hint);
#endif
#ifdef ATTR_substate
  insert_st += _setattr_str_const(attr_d,"substate",ATTR_substate);
  insert_st += _insert_str_const(mod_d,"ATTR_substate",ATTR_substate);
#endif
#ifdef ATTR_name
  insert_st += _setattr_str_const(attr_d,"name",ATTR_name);
  insert_st += _insert_str_const(mod_d,"ATTR_name",ATTR_name);
#endif
#ifdef ATTR_owner
  insert_st += _setattr_str_const(attr_d,"owner",ATTR_owner);
  insert_st += _insert_str_const(mod_d,"ATTR_owner",ATTR_owner);
#endif
#ifdef ATTR_used
  insert_st += _setattr_str_const(attr_d,"used",ATTR_used);
  insert_st += _insert_str_const(mod_d,"ATTR_used",ATTR_used);
#endif
#ifdef ATTR_state
  insert_st += _setattr_str_const(attr_d,"state",ATTR_state);
  insert_st += _insert_str_const(mod_d,"ATTR_state",ATTR_state);
#endif
#ifdef ATTR_queue
  insert_st += _setattr_str_const(attr_d,"queue",ATTR_queue);
  insert_st += _insert_str_const(mod_d,"ATTR_queue",ATTR_queue);
#endif
#ifdef ATTR_server
  insert_st += _setattr_str_const(attr_d,"server",ATTR_server);
  insert_st += _insert_str_const(mod_d,"ATTR_server",ATTR_server);
#endif
#ifdef ATTR_maxrun
  insert_st += _setattr_str_const(attr_d,"maxrun",ATTR_maxrun);
  insert_st += _insert_str_const(mod_d,"ATTR_maxrun",ATTR_maxrun);
#endif
#ifdef ATTR_total
  insert_st += _setattr_str_const(attr_d,"total",ATTR_total);
  insert_st += _insert_str_const(mod_d,"ATTR_total",ATTR_total);
#endif
#ifdef ATTR_comment
  insert_st += _setattr_str_const(attr_d,"comment",ATTR_comment);
  insert_st += _insert_str_const(mod_d,"ATTR_comment",ATTR_comment);
#endif
#ifdef ATTR_cookie
  insert_st += _setattr_str_const(attr_d,"cookie",ATTR_cookie);
  insert_st += _insert_str_const(mod_d,"ATTR_cookie",ATTR_cookie);
#endif
#ifdef ATTR_qrank
  insert_st += _setattr_str_const(attr_d,"qrank",ATTR_qrank);
  insert_st += _insert_str_const(mod_d,"ATTR_qrank",ATTR_qrank);
#endif
#ifdef ATTR_altid
  insert_st += _setattr_str_const(attr_d,"altid",ATTR_altid);
  insert_st += _insert_str_const(mod_d,"ATTR_altid",ATTR_altid);
#endif
#ifdef ATTR_etime
  insert_st += _setattr_str_const(attr_d,"etime",ATTR_etime);
  insert_st += _insert_str_const(mod_d,"ATTR_etime",ATTR_etime);
#endif
#ifdef ATTR_aclgren
  insert_st += _setattr_str_const(attr_d,"aclgren",ATTR_aclgren);
  insert_st += _insert_str_const(mod_d,"ATTR_aclgren",ATTR_aclgren);
#endif
#ifdef ATTR_aclgroup
  insert_st += _setattr_str_const(attr_d,"aclgroup",ATTR_aclgroup);
  insert_st += _insert_str_const(mod_d,"ATTR_aclgroup",ATTR_aclgroup);
#endif
#ifdef ATTR_aclhten
  insert_st += _setattr_str_const(attr_d,"aclhten",ATTR_aclhten);
  insert_st += _insert_str_const(mod_d,"ATTR_aclhten",ATTR_aclhten);
#endif
#ifdef ATTR_aclhost
  insert_st += _setattr_str_const(attr_d,"aclhost",ATTR_aclhost);
  insert_st += _insert_str_const(mod_d,"ATTR_aclhost",ATTR_aclhost);
#endif
#ifdef ATTR_acluren
  insert_st += _setattr_str_const(attr_d,"acluren",ATTR_acluren);
  insert_st += _insert_str_const(mod_d,"ATTR_acluren",ATTR_acluren);
#endif
#ifdef ATTR_acluser
  insert_st += _setattr_str_const(attr_d,"acluser",ATTR_acluser);
  insert_st += _insert_str_const(mod_d,"ATTR_acluser",ATTR_acluser);
#endif
#ifdef ATTR_altrouter
  insert_st += _setattr_str_const(attr_d,"altrouter",ATTR_altrouter);
  insert_st += _insert_str_const(mod_d,"ATTR_altrouter",ATTR_altrouter);
#endif
#ifdef ATTR_chkptmin
  insert_st += _setattr_str_const(attr_d,"chkptmin",ATTR_chkptmin);
  insert_st += _insert_str_const(mod_d,"ATTR_chkptmin",ATTR_chkptmin);
#endif
#ifdef ATTR_enable
  insert_st += _setattr_str_const(attr_d,"enable",ATTR_enable);
  insert_st += _insert_str_const(mod_d,"ATTR_enable",ATTR_enable);
#endif
#ifdef ATTR_fromroute
  insert_st += _setattr_str_const(attr_d,"fromroute",ATTR_fromroute);
  insert_st += _insert_str_const(mod_d,"ATTR_fromroute",ATTR_fromroute);
#endif
#ifdef ATTR_killdelay
  insert_st += _setattr_str_const(attr_d,"killdelay",ATTR_killdelay);
  insert_st += _insert_str_const(mod_d,"ATTR_killdelay",ATTR_killdelay);
#endif
#ifdef ATTR_maxgrprun
  insert_st += _setattr_str_const(attr_d,"maxgrprun",ATTR_maxgrprun);
  insert_st += _insert_str_const(mod_d,"ATTR_maxgrprun",ATTR_maxgrprun);
#endif
#ifdef ATTR_qtype
  insert_st += _setattr_str_const(attr_d,"qtype",ATTR_qtype);
  insert_st += _insert_str_const(mod_d,"ATTR_qtype",ATTR_qtype);
#endif
#ifdef ATTR_rescassn
  insert_st += _setattr_str_const(attr_d,"rescassn",ATTR_rescassn);
  insert_st += _insert_str_const(mod_d,"ATTR_rescassn",ATTR_rescassn);
#endif
#ifdef ATTR_rescdflt
  insert_st += _setattr_str_const(attr_d,"rescdflt",ATTR_rescdflt);
  insert_st += _insert_str_const(mod_d,"ATTR_rescdflt",ATTR_rescdflt);
#endif
#ifdef ATTR_rescmax
  insert_st += _setattr_str_const(attr_d,"rescmax",ATTR_rescmax);
  insert_st += _insert_str_const(mod_d,"ATTR_rescmax",ATTR_rescmax);
#endif
#ifdef ATTR_rescmin
  insert_st += _setattr_str_const(attr_d,"rescmin",ATTR_rescmin);
  insert_st += _insert_str_const(mod_d,"ATTR_rescmin",ATTR_rescmin);
#endif
#ifdef ATTR_rndzretry
  insert_st += _setattr_str_const(attr_d,"rndzretry",ATTR_rndzretry);
  insert_st += _insert_str_const(mod_d,"ATTR_rndzretry",ATTR_rndzretry);
#endif
#ifdef ATTR_routedest
  insert_st += _setattr_str_const(attr_d,"routedest",ATTR_routedest);
  insert_st += _insert_str_const(mod_d,"ATTR_routedest",ATTR_routedest);
#endif
#ifdef ATTR_routeheld
  insert_st += _setattr_str_const(attr_d,"routeheld",ATTR_routeheld);
  insert_st += _insert_str_const(mod_d,"ATTR_routeheld",ATTR_routeheld);
#endif
#ifdef ATTR_routewait 
  insert_st += _setattr_str_const(attr_d,"routewait",ATTR_routewait);
  insert_st += _insert_str_const(mod_d,"ATTR_routewait",ATTR_routewait);
#endif
#ifdef ATTR_routeretry
  insert_st += _setattr_str_const(attr_d,"routeretry",ATTR_routeretry);
  insert_st += _insert_str_const(mod_d,"ATTR_routeretry",ATTR_routeretry);
#endif
#ifdef ATTR_routelife
  insert_st += _setattr_str_const(attr_d,"routelife",ATTR_routelife);
  insert_st += _insert_str_const(mod_d,"ATTR_routelife",ATTR_routelife);
#endif
#ifdef ATTR_rsvexpdt
  insert_st += _setattr_str_const(attr_d,"rsvexpdt",ATTR_rsvexpdt);
  insert_st += _insert_str_const(mod_d,"ATTR_rsvexpdt",ATTR_rsvexpdt);
#endif
#ifdef ATTR_rsvsync
  insert_st += _setattr_str_const(attr_d,"rsvsync",ATTR_rsvsync);
  insert_st += _insert_str_const(mod_d,"ATTR_rsvsync",ATTR_rsvsync);
#endif
#ifdef ATTR_start
  insert_st += _setattr_str_const(attr_d,"start",ATTR_start);
  insert_st += _insert_str_const(mod_d,"ATTR_start",ATTR_start);
#endif
#ifdef ATTR_count
  insert_st += _setattr_str_const(attr_d,"count",ATTR_count);
  insert_st += _insert_str_const(mod_d,"ATTR_count",ATTR_count);
#endif
#ifdef ATTR_number 
  insert_st += _setattr_str_const(attr_d,"number",ATTR_number);
  insert_st += _insert_str_const(mod_d,"ATTR_number",ATTR_number);
#endif

#ifdef ATTR_aclroot
  insert_st += _setattr_str_const(attr_d,"aclroot",ATTR_aclroot);
  insert_st += _insert_str_const(mod_d,"ATTR_aclroot",ATTR_aclroot);
#endif
#ifdef ATTR_managers
  insert_st += _setattr_str_const(attr_d,"managers",ATTR_managers);
  insert_st += _insert_str_const(mod_d,"ATTR_managers",ATTR_managers);
#endif
#ifdef ATTR_dfltque
  insert_st += _setattr_str_const(attr_d,"dfltque",ATTR_dfltque);
  insert_st += _insert_str_const(mod_d,"ATTR_dfltque",ATTR_dfltque);
#endif
#ifdef ATTR_defnode
  insert_st += _setattr_str_const(attr_d,"defnode",ATTR_defnode);
  insert_st += _insert_str_const(mod_d,"ATTR_defnode",ATTR_defnode);
#endif
#ifdef ATTR_locsvrs
  insert_st += _setattr_str_const(attr_d,"locsvrs",ATTR_locsvrs);
  insert_st += _insert_str_const(mod_d,"ATTR_locsvrs",ATTR_locsvrs);
#endif
#ifdef ATTR_logevents
  insert_st += _setattr_str_const(attr_d,"logevents",ATTR_logevents);
  insert_st += _insert_str_const(mod_d,"ATTR_logevents",ATTR_logevents);
#endif
#ifdef ATTR_logfile
  insert_st += _setattr_str_const(attr_d,"logfile",ATTR_logfile);
  insert_st += _insert_str_const(mod_d,"ATTR_logfile",ATTR_logfile);
#endif
#ifdef ATTR_mailfrom
  insert_st += _setattr_str_const(attr_d,"mailfrom",ATTR_mailfrom);
  insert_st += _insert_str_const(mod_d,"ATTR_mailfrom",ATTR_mailfrom);
#endif /* if defined ATTR_mailfrom */

#ifdef ATTR_nodepack
  insert_st += _setattr_str_const(attr_d,"nodepack",ATTR_nodepack);
  insert_st += _insert_str_const(mod_d,"ATTR_nodepack",ATTR_nodepack);
#endif /* if defined ATTR_nodepack */

#ifdef ATTR_nodefailrq
  insert_st += _setattr_str_const(attr_d,"nodefailrq",ATTR_nodefailrq);
  insert_st += _insert_str_const(mod_d,"ATTR_nodefailrq",ATTR_nodefailrq);
#endif /* if defined ATTR_nodefailrq */

#ifdef ATTR_operators
  insert_st += _setattr_str_const(attr_d,"operators",ATTR_operators);
  insert_st += _insert_str_const(mod_d,"ATTR_operators",ATTR_operators);
#endif /* if defined ATTR_operators */

#ifdef ATTR_queryother
  insert_st += _setattr_str_const(attr_d,"queryother",ATTR_queryother);
  insert_st += _insert_str_const(mod_d,"ATTR_queryother",ATTR_queryother);
#endif /* if defined ATTR_queryother */

#ifdef ATTR_resccost
  insert_st += _setattr_str_const(attr_d,"resccost",ATTR_resccost);
  insert_st += _insert_str_const(mod_d,"ATTR_resccost",ATTR_resccost);
#endif /* if defined ATTR_resccost */

#ifdef ATTR_rescavail
  insert_st += _setattr_str_const(attr_d,"rescavail",ATTR_rescavail);
  insert_st += _insert_str_const(mod_d,"ATTR_rescavail",ATTR_rescavail);
#endif /* if defined ATTR_rescavail */



#ifdef ATTR_schedit
  insert_st += _setattr_str_const(attr_d,"schedit",ATTR_schedit);
  insert_st += _insert_str_const(mod_d,"ATTR_schedit",ATTR_schedit);
#endif /* if defined ATTR_schedit */

#ifdef ATTR_scheduling
  insert_st += _setattr_str_const(attr_d,"scheduling",ATTR_scheduling);
  insert_st += _insert_str_const(mod_d,"ATTR_scheduling",ATTR_scheduling);
#endif /* if defined ATTR_scheduling */

#ifdef ATTR_status
  insert_st += _setattr_str_const(attr_d,"status",ATTR_status);
  insert_st += _insert_str_const(mod_d,"ATTR_status",ATTR_status);
#endif /* if defined ATTR_status */

#ifdef ATTR_syscost
  insert_st += _setattr_str_const(attr_d,"syscost",ATTR_syscost);
  insert_st += _insert_str_const(mod_d,"ATTR_syscost",ATTR_syscost);
#endif /* if defined ATTR_syscost */

#ifdef ATTR_NODE_Host
  insert_st += _setattr_str_const(attr_d,"NODE_Host",ATTR_NODE_Host);
  insert_st += _insert_str_const(mod_d,"ATTR_NODE_Host",ATTR_NODE_Host);
#endif /* if defined ATTR_NODE_Host */


#ifdef ATTR_NODE_np
  insert_st += _setattr_str_const(attr_d,"NODE_np",ATTR_NODE_np);
  insert_st += _insert_str_const(mod_d,"ATTR_NODE_np",ATTR_NODE_np);
#endif /* if defined ATTR_NODE_np */

#ifdef ATTR_NODE_state
  insert_st += _setattr_str_const(attr_d,"NODE_state",ATTR_NODE_state);
  insert_st += _insert_str_const(mod_d,"ATTR_NODE_state",ATTR_NODE_state);
#endif /* if defined ATTR_NODE_state */

#ifdef ATTR_NODE_properties
  insert_st += _setattr_str_const(attr_d,"NODE_properties",ATTR_NODE_properties);
  insert_st += _insert_str_const(mod_d,"ATTR_NODE_properties",ATTR_NODE_properties);
#endif /* if defined ATTR_NODE_properties */

#ifdef ATTR_NODE_ntype
  insert_st += _setattr_str_const(attr_d,"NODE_ntype",ATTR_NODE_ntype);
  insert_st += _insert_str_const(mod_d,"ATTR_NODE_ntype",ATTR_NODE_ntype);
#endif /* if defined ATTR_NODE_ntype */

#ifdef ATTR_NODE_jobs
  insert_st += _setattr_str_const(attr_d,"NODE_jobs",ATTR_NODE_jobs);
  insert_st += _insert_str_const(mod_d,"ATTR_NODE_jobs",ATTR_NODE_jobs);
#endif /* if defined ATTR_NODE_jobs */


  if (insert_st == 0) {
    insert_st += PyDict_SetItemString(mod_d,"attr",attr_d);
  } 
  Py_CLEAR(attr_d); /* we don't need this whether inserted or not */

  return insert_st;

}

/* Not attributes names but well known values */
/*
** The module object gets an attribute names "values" that has all this constants 
*/

static int
insert_value_flags(PyObject *mod_d)
{
  PyObject *attr_v = (PyObject *)OptionType_New(1);
  if (attr_v == NULL)
    return -1;

  insert_st = 0;

#ifdef CHECKPOINT_UNSPECIFIED
  insert_st += _setattr_str_const(attr_v,"CHECKPOINT_UNSPECIFIED",CHECKPOINT_UNSPECIFIED);
  insert_st += _insert_str_const(mod_d,"CHECKPOINT_UNSPECIFIED",CHECKPOINT_UNSPECIFIED);
#endif /* if defined CHECKPOINT_UNSPECIFIED */

#ifdef NO_HOLD
  insert_st += _setattr_str_const(attr_v,"NO_HOLD",NO_HOLD);
  insert_st += _insert_str_const(mod_d,"NO_HOLD",NO_HOLD);
#endif /* if defined NO_HOLD */

#ifdef NO_JOIN
  insert_st += _setattr_str_const(attr_v,"NO_JOIN",NO_JOIN);
  insert_st += _insert_str_const(mod_d,"NO_JOIN",NO_JOIN);
#endif /* if defined NO_JOIN */

#ifdef NO_KEEP
  insert_st += _setattr_str_const(attr_v,"NO_KEEP",NO_KEEP);
  insert_st += _insert_str_const(mod_d,"NO_KEEP",NO_KEEP);
#endif /* if defined NO_KEEP */

#ifdef MAIL_AT_ABORT
  insert_st += _setattr_str_const(attr_v,"MAIL_AT_ABORT",MAIL_AT_ABORT);
  insert_st += _insert_str_const(mod_d,"MAIL_AT_ABORT",MAIL_AT_ABORT);
#endif /* if defined MAIL_AT_ABORT */

#ifdef DELDELAY
  insert_st += _setattr_str_const(attr_v,"DELDELAY",DELDELAY);
  insert_st += _insert_str_const(mod_d,"DELDELAY",DELDELAY);
#endif /* if defined DELDELAY */

#ifdef USER_HOLD
  insert_st += _setattr_str_const(attr_v,"USER_HOLD",USER_HOLD);
  insert_st += _insert_str_const(mod_d,"USER_HOLD",USER_HOLD);
#endif /* if defined USER_HOLD */

#ifdef OTHER_HOLD
  insert_st += _setattr_str_const(attr_v,"OTHER_HOLD",OTHER_HOLD);
  insert_st += _insert_str_const(mod_d,"OTHER_HOLD",OTHER_HOLD);
#endif /* if defined OTHER_HOLD */

#ifdef SYSTEM_HOLD
  insert_st += _setattr_str_const(attr_v,"SYSTEM_HOLD",SYSTEM_HOLD);
  insert_st += _insert_str_const(mod_d,"SYSTEM_HOLD",SYSTEM_HOLD);
#endif /* if defined SYSTEM_HOLD */


#ifdef ND_free
  insert_st += _setattr_str_const(attr_v,"ND_free",ND_free);
  insert_st += _insert_str_const(mod_d,"ND_free",ND_free);
#endif /* if defined ND_free */

#ifdef ND_offline
  insert_st += _setattr_str_const(attr_v,"ND_offline",ND_offline);
  insert_st += _insert_str_const(mod_d,"ND_offline",ND_offline);
#endif /* if defined ND_offline */

#ifdef ND_down
  insert_st += _setattr_str_const(attr_v,"ND_down",ND_down);
  insert_st += _insert_str_const(mod_d,"ND_down",ND_down);
#endif /* if defined ND_down */

#ifdef ND_reserve
  insert_st += _setattr_str_const(attr_v,"ND_reserve",ND_reserve);
  insert_st += _insert_str_const(mod_d,"ND_reserve",ND_reserve);
#endif /* if defined ND_reserve */


#ifdef ND_busy
  insert_st += _setattr_str_const(attr_v,"ND_busy",ND_busy);
  insert_st += _insert_str_const(mod_d,"ND_busy",ND_busy);
#endif /* if defined ND_busy */

#ifdef ND_state_unknown
  insert_st += _setattr_str_const(attr_v,"ND_state_unknown",ND_state_unknown);
  insert_st += _insert_str_const(mod_d,"ND_state_unknown",ND_state_unknown);
#endif /* if defined ND_state_unknown */

#ifdef ND_timeshared
  insert_st += _setattr_str_const(attr_v,"ND_timeshared",ND_timeshared);
  insert_st += _insert_str_const(mod_d,"ND_timeshared",ND_timeshared);
#endif /* if defined ND_timeshared */

#ifdef ND_cluster
  insert_st += _setattr_str_const(attr_v,"ND_cluster",ND_cluster);
  insert_st += _insert_str_const(mod_d,"ND_cluster",ND_cluster);
#endif /* if defined ND_cluster */


#ifdef MAX_ENCODE_BFR
  insert_st += _setattr_int_const(attr_v,"MAX_ENCODE_BFR",MAX_ENCODE_BFR);
  insert_st += _insert_int_const(mod_d,"MAX_ENCODE_BFR",MAX_ENCODE_BFR);
#endif /* if defined MAX_ENCODE_BFR */

#ifdef MGR_CMD_CREATE
  insert_st += _setattr_int_const(attr_v,"MGR_CMD_CREATE",MGR_CMD_CREATE);
  insert_st += _insert_int_const(mod_d,"MGR_CMD_CREATE",MGR_CMD_CREATE);
#endif /* if defined MGR_CMD_CREATE */

#ifdef MGR_CMD_DELETE
  insert_st += _setattr_int_const(attr_v,"MGR_CMD_DELETE",MGR_CMD_DELETE);
  insert_st += _insert_int_const(mod_d,"MGR_CMD_DELETE",MGR_CMD_DELETE);
#endif /* if defined MGR_CMD_DELETE */

#ifdef MGR_CMD_SET
  insert_st += _setattr_int_const(attr_v,"MGR_CMD_SET",MGR_CMD_SET);
  insert_st += _insert_int_const(mod_d,"MGR_CMD_SET",MGR_CMD_SET);
#endif /* if defined MGR_CMD_SET */

#ifdef MGR_CMD_UNSET
  insert_st += _setattr_int_const(attr_v,"MGR_CMD_UNSET",MGR_CMD_UNSET);
  insert_st += _insert_int_const(mod_d,"MGR_CMD_UNSET",MGR_CMD_UNSET);
#endif /* if defined MGR_CMD_UNSET */

#ifdef MGR_CMD_LIST
  insert_st += _setattr_int_const(attr_v,"MGR_CMD_LIST",MGR_CMD_LIST);
  insert_st += _insert_int_const(mod_d,"MGR_CMD_LIST",MGR_CMD_LIST);
#endif /* if defined MGR_CMD_LIST */

#ifdef MGR_CMD_PRINT
  insert_st += _setattr_int_const(attr_v,"MGR_CMD_PRINT",MGR_CMD_PRINT);
  insert_st += _insert_int_const(mod_d,"MGR_CMD_PRINT",MGR_CMD_PRINT);
#endif /* if defined MGR_CMD_PRINT */

#ifdef MGR_CMD_ACTIVE
  insert_st += _setattr_int_const(attr_v,"MGR_CMD_ACTIVE",MGR_CMD_ACTIVE);
  insert_st += _insert_int_const(mod_d,"MGR_CMD_ACTIVE",MGR_CMD_ACTIVE);
#endif /* if defined MGR_CMD_ACTIVE */

#ifdef MGR_OBJ_NONE
  insert_st += _setattr_int_const(attr_v,"MGR_OBJ_NONE",MGR_OBJ_NONE);
  insert_st += _insert_int_const(mod_d,"MGR_OBJ_NONE",MGR_OBJ_NONE);
#endif /* if defined MGR_OBJ_NONE */

#ifdef MGR_OBJ_SERVER
  insert_st += _setattr_int_const(attr_v,"MGR_OBJ_SERVER",MGR_OBJ_SERVER);
  insert_st += _insert_int_const(mod_d,"MGR_OBJ_SERVER",MGR_OBJ_SERVER);
#endif /* if defined MGR_OBJ_SERVER */

#ifdef MGR_OBJ_QUEUE
  insert_st += _setattr_int_const(attr_v,"MGR_OBJ_QUEUE",MGR_OBJ_QUEUE);
  insert_st += _insert_int_const(mod_d,"MGR_OBJ_QUEUE",MGR_OBJ_QUEUE);
#endif /* if defined MGR_OBJ_QUEUE */

#ifdef MGR_OBJ_JOB
  insert_st += _setattr_int_const(attr_v,"MGR_OBJ_JOB",MGR_OBJ_JOB);
  insert_st += _insert_int_const(mod_d,"MGR_OBJ_JOB",MGR_OBJ_JOB);
#endif /* if defined MGR_OBJ_JOB */

#ifdef MGR_OBJ_NODE
  insert_st += _setattr_int_const(attr_v,"MGR_OBJ_NODE",MGR_OBJ_NODE);
  insert_st += _insert_int_const(mod_d,"MGR_OBJ_NODE",MGR_OBJ_NODE);
#endif /* if defined MGR_OBJ_NODE */



#ifdef MSG_OUT
  insert_st += _setattr_int_const(attr_v,"MSG_OUT",MSG_OUT);
  insert_st += _insert_int_const(mod_d,"MSG_OUT",MSG_OUT);
#endif /* if defined MSG_OUT */

#ifdef MSG_ERR
  insert_st += _setattr_int_const(attr_v,"MSG_ERR",MSG_ERR);
  insert_st += _insert_int_const(mod_d,"MSG_ERR",MSG_ERR);
#endif /* if defined MSG_ERR */


#ifdef SHUT_SIG
  insert_st += _setattr_int_const(attr_v,"SHUT_SIG",SHUT_SIG);
  insert_st += _insert_int_const(mod_d,"SHUT_SIG",SHUT_SIG);
#endif /* if defined SHUT_SIG */

#ifdef SHUT_IMMEDIATE
  insert_st += _setattr_int_const(attr_v,"SHUT_IMMEDIATE",SHUT_IMMEDIATE);
  insert_st += _insert_int_const(mod_d,"SHUT_IMMEDIATE",SHUT_IMMEDIATE);
#endif /* if defined SHUT_IMMEDIATE */

#ifdef SHUT_DELAY
  insert_st += _setattr_int_const(attr_v,"SHUT_DELAY",SHUT_DELAY);
  insert_st += _insert_int_const(mod_d,"SHUT_DELAY",SHUT_DELAY);
#endif /* if defined SHUT_DELAY */

#ifdef SHUT_QUICK
  insert_st += _setattr_int_const(attr_v,"SHUT_QUICK",SHUT_QUICK);
  insert_st += _insert_int_const(mod_d,"SHUT_QUICK",SHUT_QUICK);
#endif /* if defined SHUT_QUICK */



#ifdef SIG_RESUME
  insert_st += _setattr_str_const(attr_v,"SIG_RESUME",SIG_RESUME);
  insert_st += _insert_str_const(mod_d,"SIG_RESUME",SIG_RESUME);
#endif /* if defined SIG_RESUME */

#ifdef SIG_SUSPEND
  insert_st += _setattr_str_const(attr_v,"SIG_SUSPEND",SIG_SUSPEND);
  insert_st += _insert_str_const(mod_d,"SIG_SUSPEND",SIG_SUSPEND);
#endif /* if defined SIG_SUSPEND */


#ifdef PBS_MAXHOSTNAME
  insert_st += _setattr_int_const(attr_v,"PBS_MAXHOSTNAME",PBS_MAXHOSTNAME);
  insert_st += _insert_int_const(mod_d,"PBS_MAXHOSTNAME",PBS_MAXHOSTNAME);
#endif /* if defined PBS_MAXHOSTNAME */


#ifdef PBS_MAXUSER
  insert_st += _setattr_int_const(attr_v,"PBS_MAXUSER",PBS_MAXUSER);
  insert_st += _insert_int_const(mod_d,"PBS_MAXUSER",PBS_MAXUSER);
#endif /* if defined PBS_MAXUSER */

#ifdef PBS_MAXGRPN
  insert_st += _setattr_int_const(attr_v,"PBS_MAXGRPN",PBS_MAXGRPN);
  insert_st += _insert_int_const(mod_d,"PBS_MAXGRPN",PBS_MAXGRPN);
#endif /* if defined PBS_MAXGRPN */

#ifdef PBS_MAXQUEUENAME
  insert_st += _setattr_int_const(attr_v,"PBS_MAXQUEUENAME",PBS_MAXQUEUENAME);
  insert_st += _insert_int_const(mod_d,"PBS_MAXQUEUENAME",PBS_MAXQUEUENAME);
#endif /* if defined PBS_MAXQUEUENAME */

#ifdef PBS_MAXSEQNUM
  insert_st += _setattr_int_const(attr_v,"PBS_MAXSEQNUM",PBS_MAXSEQNUM);
  insert_st += _insert_int_const(mod_d,"PBS_MAXSEQNUM",PBS_MAXSEQNUM);
#endif /* if defined PBS_MAXSEQNUM */

#ifdef PBS_MAXPORTNUM
  insert_st += _setattr_int_const(attr_v,"PBS_MAXPORTNUM",PBS_MAXPORTNUM);
  insert_st += _insert_int_const(mod_d,"PBS_MAXPORTNUM",PBS_MAXPORTNUM);
#endif /* if defined PBS_MAXPORTNUM */

#ifdef PBS_MAXDEST
  insert_st += _setattr_int_const(attr_v,"PBS_MAXDEST",PBS_MAXDEST);
  insert_st += _insert_int_const(mod_d,"PBS_MAXDEST",PBS_MAXDEST);
#endif /* if defined PBS_MAXDEST */

#ifdef PBS_USE_IFF
  insert_st += _setattr_int_const(attr_v,"PBS_USE_IFF",PBS_USE_IFF);
  insert_st += _insert_int_const(mod_d,"PBS_USE_IFF",PBS_USE_IFF);
#endif /* if defined PBS_USE_IFF */

#ifdef PBS_INTERACTIVE
  insert_st += _setattr_int_const(attr_v,"PBS_INTERACTIVE",PBS_INTERACTIVE);
  insert_st += _insert_int_const(mod_d,"PBS_INTERACTIVE",PBS_INTERACTIVE);
#endif /* if defined PBS_INTERACTIVE */

#ifdef PBS_TERM_BUF_SZ
  insert_st += _setattr_int_const(attr_v,"PBS_TERM_BUF_SZ",PBS_TERM_BUF_SZ);
  insert_st += _insert_int_const(mod_d,"PBS_TERM_BUF_SZ",PBS_TERM_BUF_SZ);
#endif /* if defined PBS_TERM_BUF_SZ */

#ifdef PBS_TERM_CCA
  insert_st += _setattr_int_const(attr_v,"PBS_TERM_CCA",PBS_TERM_CCA);
  insert_st += _insert_int_const(mod_d,"PBS_TERM_CCA",PBS_TERM_CCA);
#endif /* if defined PBS_TERM_CCA */

#ifdef PBS_BATCH_SERVICE_NAME
  insert_st += _setattr_str_const(attr_v,"PBS_BATCH_SERVICE_NAME",PBS_BATCH_SERVICE_NAME);
  insert_st += _insert_str_const(mod_d,"PBS_BATCH_SERVICE_NAME",PBS_BATCH_SERVICE_NAME);
#endif /* if defined PBS_BATCH_SERVICE_NAME */

#ifdef PBS_BATCH_SERVICE_PORT
  insert_st += _setattr_int_const(attr_v,"PBS_BATCH_SERVICE_PORT",PBS_BATCH_SERVICE_PORT);
  insert_st += _insert_int_const(mod_d,"PBS_BATCH_SERVICE_PORT",PBS_BATCH_SERVICE_PORT);
#endif /* if defined PBS_BATCH_SERVICE_PORT */

#ifdef PBS_BATCH_SERVICE_NAME_DIS
  insert_st += _setattr_str_const(attr_v,"PBS_BATCH_SERVICE_NAME_DIS",PBS_BATCH_SERVICE_NAME_DIS);
  insert_st += _insert_str_const(mod_d,"PBS_BATCH_SERVICE_NAME_DIS",PBS_BATCH_SERVICE_NAME_DIS);
#endif /* if defined PBS_BATCH_SERVICE_NAME_DIS */

#ifdef PBS_BATCH_SERVICE_PORT_DIS
  insert_st += _setattr_int_const(attr_v,"PBS_BATCH_SERVICE_PORT_DIS",PBS_BATCH_SERVICE_PORT_DIS);
  insert_st += _insert_int_const(mod_d,"PBS_BATCH_SERVICE_PORT_DIS",PBS_BATCH_SERVICE_PORT_DIS);
#endif /* if defined PBS_BATCH_SERVICE_PORT_DIS */

#ifdef PBS_MOM_SERVICE_NAME
  insert_st += _setattr_str_const(attr_v,"PBS_MOM_SERVICE_NAME",PBS_MOM_SERVICE_NAME);
  insert_st += _insert_str_const(mod_d,"PBS_MOM_SERVICE_NAME",PBS_MOM_SERVICE_NAME);
#endif /* if defined PBS_MOM_SERVICE_NAME */

#ifdef PBS_MOM_SERVICE_PORT
  insert_st += _setattr_int_const(attr_v,"PBS_MOM_SERVICE_PORT",PBS_MOM_SERVICE_PORT);
  insert_st += _insert_int_const(mod_d,"PBS_MOM_SERVICE_PORT",PBS_MOM_SERVICE_PORT);
#endif /* if defined PBS_MOM_SERVICE_PORT */

#ifdef PBS_MANAGER_SERVICE_NAME
  insert_st += _setattr_str_const(attr_v,"PBS_MANAGER_SERVICE_NAME",PBS_MANAGER_SERVICE_NAME);
  insert_st += _insert_str_const(mod_d,"PBS_MANAGER_SERVICE_NAME",PBS_MANAGER_SERVICE_NAME);
#endif /* if defined PBS_MANAGER_SERVICE_NAME */

#ifdef PBS_MANAGER_SERVICE_PORT
  insert_st += _setattr_int_const(attr_v,"PBS_MANAGER_SERVICE_PORT",PBS_MANAGER_SERVICE_PORT);
  insert_st += _insert_int_const(mod_d,"PBS_MANAGER_SERVICE_PORT",PBS_MANAGER_SERVICE_PORT);
#endif /* if defined PBS_MANAGER_SERVICE_PORT */

#ifdef PBS_SCHEDULER_SERVICE_NAME
  insert_st += _setattr_str_const(attr_v,"PBS_SCHEDULER_SERVICE_NAME",PBS_SCHEDULER_SERVICE_NAME);
  insert_st += _insert_str_const(mod_d,"PBS_SCHEDULER_SERVICE_NAME",PBS_SCHEDULER_SERVICE_NAME);
#endif /* if defined PBS_SCHEDULER_SERVICE_NAME */

#ifdef PBS_SCHEDULER_SERVICE_PORT
  insert_st += _setattr_int_const(attr_v,"PBS_SCHEDULER_SERVICE_PORT",PBS_SCHEDULER_SERVICE_PORT);
  insert_st += _insert_int_const(mod_d,"PBS_SCHEDULER_SERVICE_PORT",PBS_SCHEDULER_SERVICE_PORT);
#endif /* if defined PBS_SCHEDULER_SERVICE_PORT */



  if (insert_st == 0) {
    insert_st += PyDict_SetItemString(mod_d,"values",attr_v);
  } 
  Py_CLEAR(attr_v); /* we don't need this whether inserted or not */

  return insert_st;

}
