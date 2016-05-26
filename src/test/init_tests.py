__copyright__ = """
 
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

"""
__revision__ = "$Id: init_tests.py 13 2007-11-30 02:24:22Z kmohraj $"

import sys,os
from distutils.util import get_platform
plat_specifier = '.%s-%s' % (get_platform(),sys.version[0:3])
TESTDIR_ROOT = os.path.abspath(os.path.dirname(__file__))
src_root = os.path.abspath('%s/../..' % (TESTDIR_ROOT,))
build_lib = '%s/build/lib%s' % (src_root,plat_specifier)
emsg = ''
if os.path.exists(build_lib) and os.path.isdir(build_lib):
    sys.path.insert(0,build_lib)
else:
    emsg = """Cannot find the build directory <%s>""" % (build_lib,)

try:
    import PBSPy.capi as api
except (ImportError),(e):
    raise ImportError,"%s:\n\t%s" % (e,emsg)

#:
from ConfigParser import ConfigParser
#: rename the root error
from ConfigParser import Error as ConfigParserError

DEFAULT_SEC = 'DEFAULT'
TEST_SEC = 'TEST_DATA'

_DEFAULT_MAP =  {
    'default':('',None),
    'boolean':(False,None),
    'float':(0.0,None),
    'int': (0,None)
    }
        
class TestConfigParams(object):
    """An encapsulation for all test objects"""
    
    def __new__(type,*args,**kwds):
        """return the singleton"""
        if not '_TheInstance' in type.__dict__:
            type._TheInstance = object.__new__(type)
        return type._TheInstance

    def initialize(self,*args,**kwds):
        """
        """

        filenames = ['data/api_test.cfg','data/lib_test.cfg']
        filenames = [os.path.join(TESTDIR_ROOT,s) for s in filenames]
        self.initialized = False
        cfg_o = self.cfg_obj = ConfigParser()
        try:
            cfg_o.read(filenames)
        except (ConfigParserError),(e):
            print "Error: %s" % (e,)
        self.test_dir = TESTDIR_ROOT
        _DEFAULT_MAP['default'] = ('',cfg_o.get)
        _DEFAULT_MAP['boolean'] = (False,cfg_o.getboolean)
        _DEFAULT_MAP['float'] = (0.0,cfg_o.getfloat)
        _DEFAULT_MAP['int'] = (0,cfg_o.getint)

    def __getattribute__(self,a):
        """
        """
        #print "\t**** a:",a
        try:
            return object.__getattribute__(self,a)
        except (AttributeError),(e):
            return getattr(self.cfg_obj,a)

    def getval(self,option,section='DEFAULT',opt_type='default'):
        """
        """

        cfg_o = self.cfg_obj
        #: set the default return value
        rv = _DEFAULT_MAP[opt_type][0] #: if caller screws up the key raise
        if cfg_o.has_option(section,option):
            rv = _DEFAULT_MAP[opt_type][1](section,option)
        return rv

    def get_config_obj(self):
        """
        """

        return self.cfg_obj

    def destroy(self):
        """
        """
        pass
    
test_params = TestConfigParams()

