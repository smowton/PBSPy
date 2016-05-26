# -*- coding: UTF-8 -*
"""Module providing a Configure Class.

""" 
from distutils.command.config import config
from distutils.core import DEBUG
from distutils import log
from types import StringType
import string
import os
class Configure(config):
    """Utility class to run a ./configure like command to setup the extension objects
        
        Conventions:
    """
    command_name = "config"
    
    user_options = config.user_options + [
        ('include-dirs=', 'I',
         "list of directories to search for header files"),
        ('rpath=', 'R',
         "directories to search for shared C libraries at runtime"),
        ]
    #:BUG missing in config, hence dump_source and noisy never get initialized properly
    boolean_options = ['dump_source', 'noisy']
    run_commands = []
    def initialize_options (self):
        """
        """
        config.initialize_options(self)
        self.define = None
        self.undef = None
        self.rpath = None
    #: end m(initialize_options)
    
    def finalize_options (self):
        """finalize option is buggy as distribution.include_dirs do not do proper string to list conversion
        
            NOTES:
                - distutils.command.config finalize_option is broken as it takes it from the distribution module
                  and is broken.
        """
        if self.include_dirs is None:
            self.include_dirs = self.distribution.include_dirs or []
        if type(self.include_dirs) is StringType:
            self.include_dirs = string.split(self.include_dirs, os.pathsep)

        if self.libraries is None:
            self.libraries = []
        if type(self.libraries) is StringType:
            self.libraries = [self.libraries]

        if self.library_dirs is None:
            self.library_dirs = []
        if type(self.library_dirs) is StringType:
            self.library_dirs = string.split(self.library_dirs, os.pathsep)
            
        if self.define is None:
            self.define = []
            
        if type(self.define) is StringType:
            self.define = [(s.strip(),None) for s in self.define.split(',')]
        log.debug("self.define: %s" % (self.define,))
        
        if self.undef is None:
            self.undef = []
        if type(self.undef) is StringType:
            self.undef = [s.strip() for s in self.undef.split(',')]
        
        if self.rpath is None:
            self.rpath = []
        if type(self.rpath) is StringType:
            self.rpath = string.split(self.rpath, os.pathsep)
            
    #: end m(finalize_options)
    
    def run(self):
        for cmd_name,cmd in self.run_commands:
            print "*" * 80
            print "\tRunning configure command: '%s'" % (cmd_name,)
            cmd(self) 
            print "*" * 80
#: end c(Configure)
