# -*- coding: UTF-8 -*
"""Module providing PBS Configure Class.

""" 
 
from distextensions.ext.configure import Configure, DEBUG
from distutils import log
from types import StringType
import string
import os

_RM_HEADER_FILE = "rm.h"
_LOG_HEADER_FILE = "log.h"

class PBSPyConfigure(Configure):
    
    command_name = "pbs_config"
    description = "Base Configure like script for PBS macros,libraries"
    
    user_options = Configure.user_options + [
        ('pbs-libraries=', 'p',
         "comma separated list of pbs library names without prefix (unix) 'lib' or ext"),
        ('enable-rm', None,
         "Enable Resource Monitor API (IMPORTANT must have rm.h available in the <pbs-install-root>/include"),
        ('enable-log', None,
         "Enable PBS Log API (IMPORTANT must have log.h available in the <pbs-install-root>/include"),
        ]
    
    boolean_options = Configure.boolean_options + [
                         'enable_rm', 'enable_log'
                      ]
    
    def initialize_options (self):
        """
        """
        Configure.initialize_options(self)
        
        self.pbs_libraries = None
        self.enable_rm = 0
        self.enable_log = 0
        
    #: end m(initialize_options)
    
    def finalize_options (self):
        """finalize option is buggy as distribution.include_dirs do not do proper string to list conversion
        
            NOTES:
                - distutils.command.config finalize_option is broken as it takes it from the distribution module
                  and is broken.
        """
        
        Configure.finalize_options(self)
            
        if self.pbs_libraries is None:
            self.pbs_libraries = []
            
        if type(self.pbs_libraries) is StringType:
            self.pbs_libraries = [s.strip() for s in self.pbs_libraries.split(',')]
        #: now add it to self.libraries
        self.libraries.extend(self.pbs_libraries)
        
        log.debug("self.pbs_libraries: %s" % (self.pbs_libraries,))
            
    #: end m(finalize_options)
    def _check_selstat(self):
        """Check whether we have the 3 argument or four argument version of
        pbs_selstat
        """
        
        func_calls = {
            'three_args' : 'pbs_selstat(connect,NULL,NULL)',
            'four_args'  : 'pbs_selstat(connect,NULL,NULL,NULL)'
            }
        test_body = """
            int
            main(int argc,char **argv) {
                struct batch_status *bsp;
                int connect;
                bsp = %s;
            }
        """

        name = ''
        for define,func_call in func_calls.iteritems():
            status = self.try_compile(body=test_body % (func_call,),
                                   headers=['stdlib.h','pbs_ifl.h'],
                                   include_dirs=self.include_dirs)
            if status:
                name = '%s_PBS_SELSTAT' % (define.upper(),)
                break

        if name != '':
            self.define.append((name,None))
    #: end m(_check_selstat)
    
    def _check_configuration(self):
        """Check any macros that need to be defined because of configuration data
        
        NOTES:
            - Check for whether to compile rm type
            - Check for whether to compile log type
        """
        emsg_fmt = """
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        WARNING: Could not find the required header file '%s' even though the options is enabled.
        Cannot continue disable it by *not* specifying it on the command line or config file (setup.cfg)
        Copy the header file from the OpenPBS/Torque source distribution.
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        """
        if self.enable_rm:
            status = self.try_cpp(body="\n/* empty */\n",
                                  headers=[_RM_HEADER_FILE],
                                  include_dirs=self.include_dirs)
            if status: 
                self.define.append(('PBSPY_ENABLE_MOM',None))
            else:
                print emsg_fmt % (_RM_HEADER_FILE,)
                raise SystemExit,2
        else:
            self.undef.append('PBSPY_ENABLE_MOM')
        
        if self.enable_log:
            status = self.try_cpp(body="\n/* empty */\n",
                                  headers=[_LOG_HEADER_FILE],
                                  include_dirs=self.include_dirs)
            if status:
                self.define.append(('PBSPY_ENABLE_LOG',None))
            else:
                print emsg_fmt % (_LOG_HEADER_FILE,)
                raise SystemExit,2
        else:
            self.undef.append('PBSPY_ENABLE_LOG')
        #: end if
        
    #: end m(_check_cmdline)
    
    run_commands = Configure.run_commands + [
                        ('checking for selstat',_check_selstat),
                        ('checking for macros from configuration data',_check_configuration)
                   ]
#: end c(PBSPyConfigure)


