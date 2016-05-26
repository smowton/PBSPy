# -*- coding: UTF-8 -*
"""Python- PBS (Portable Batch System) Adaptor

NOTES:
 . Python Compile Flags that are overridden
    -Wno-strict-prototypes:

"""

#: Trove classifiers
classifiers = """\
Development Status :: 5 - Production/Beta
Intended Audience :: Developers
License :: OSI Approved :: Apache Software License
Programming Language :: Python
Programming Language :: C
Topic :: Batch Systems
Topic :: Grid Computing :: PBS
Topic :: Grid Computing :: PBSPro
Topic :: Grid Computing :: OpenPBS
Topic :: Grid Computing :: Torque
Topic :: Software Development
Topic :: Software Development :: Libraries :: Python Modules
Operating System :: Microsoft :: Windows
Operating System :: Unix
"""

import sys
import os

_REQUIRED_PYTHON_VERSION = '2.5.1'
emsg = r"""
*** This module requires Python Version > '%s' ***

Even though it does not use any features yet from a C API perspective above 2.2.3. The
distutils package is very buggy in the older versions and I do not have time to backport 
the distutils package to the version that is shipped with 2.5.1. if you are brave you
can:
    . copy the distutils package from the 2.5.1 source tree and overlay it over your
      older python install. I believe the distutils package is supposed to maintainer
      backward compatibility  upto 2.1 (?) version.
      
""" % (_REQUIRED_PYTHON_VERSION,)

if sys.version < _REQUIRED_PYTHON_VERSION:
    sys.stderr.write(emsg)
    raise SystemExit,1

#: Make sure our distutils extensions are found
sys.path.append(os.path.join('.','buildutils'))


import os
import shutil



from distutils.core import setup
from distutils.core import DEBUG
from distutils.errors import DistutilsFileError,DistutilsSetupError
from distutils.sysconfig import customize_compiler,get_config_vars
from distutils.util import convert_path
from distutils import __version__ as distutils_version
from distutils.log import set_verbosity
from distutils.dist import Distribution as _Distribution
import types

#: B Local distexntensions import
from distextensions.tools.compilers import remove_from_config_vars
from distextensions.tools.fileutils import write_version_string_to_package


from distutils.extension import Extension as ExtensionClass
from distextensions.ext.build_ext  import build_extExtended as build_extClass


#: ------------------------------
#: setup functional call GLOBALS
#: ------------------------------

PACKAGE_ROOT_NAME = "PBSPy"
PACKAGE_VERSION = "1.0.1"
PACKAGE_PYTHON_SRC_ROOT = os.path.join('src','main','python')

#: this is the name of out c ext module, also used to generate the init<module> function name
PBSPY_EXT_MODULE_NAME = "_pbs"
PBSPY_EXT_PACKAGE_NAME = "%s.capi" % (PACKAGE_ROOT_NAME,)
PBSPY_EXT_FULL_NAME = "%s.%s" % (PBSPY_EXT_PACKAGE_NAME,PBSPY_EXT_MODULE_NAME)

PBSPY_EXT_MODULE_C_SRC_ROOT = os.path.join('src','main','c')

data_files = []
extensions = []
define_macros = []
packages = []
package_dir = {}

packages.extend([
                    PACKAGE_ROOT_NAME,
                    '%s.capi' % (PACKAGE_ROOT_NAME,),
                ])

package_dir.update({
                        PACKAGE_ROOT_NAME : PACKAGE_PYTHON_SRC_ROOT
                  })
#:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#:                   All pre setup things, including changing the sources
#:                   fixing distutils problems.
#:~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#: TODO : make logging configurable from command line???
#set_verbosity(5)

#: IMPORTANT change Package root __init__.py file to the new version string.
write_version_string_to_package(convert_path(os.path.join(PACKAGE_PYTHON_SRC_ROOT,"__init__.py")),
                                    PACKAGE_VERSION)



#:#
#:  Remove any flags from various variables thar are dependent on python build settings
#:#

#:##
#:  Check both OPT and CFLAGS as depending on the python version it might be messed up
#:##
remove_from_config_vars(
                            {
                            'OPT'           : ('-Wstrict-prototypes',),
                            'CFLAGS'        : ('-Wstrict-prototypes',),
                            }
)

PATHJOIN = os.path.join
def prefix_path(path,src_list):
    """
    prefix the path to every file in the src_list
    """

    return [os.path.join(path,e) for e in src_list]


EXTERNAL_DIR=os.path.join('Modules','external')

sources = prefix_path(PATHJOIN(PBSPY_EXT_MODULE_C_SRC_ROOT,'module'),['mainModule.c'])

#: TODO, update the list, depends is broken anyway use -f to force the rebuild.
#: depend on our includes
depends = prefix_path(PATHJOIN(PBSPY_EXT_MODULE_C_SRC_ROOT,'include'),
                        (
                            'pbsmodule.h',
                            'OptionType.h',
                            'PBSAttrType.h',
                        )
                     )

#: depend on our common 
depends.extend(
    prefix_path(PATHJOIN(PBSPY_EXT_MODULE_C_SRC_ROOT,'common'),
                (
                  'utils.c',
                )
               ))

#: depened on our objects
depends.extend(
    prefix_path( PATHJOIN(PBSPY_EXT_MODULE_C_SRC_ROOT,'objects'),
                    (
                        'OptionType.c',
                        'PBSAttrType.c',
                    )
               ))


from pbs_config import PBSPyConfigure
from distutils.dist import Distribution
from distutils.errors import DistutilsArgError

#: #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#:                       All Command Objects
#: #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#: we need build_ext to call our configure like command classes, so we use our
#: extended build_ext instead of the standard one in the distro.

build_extClass.sub_commands = build_extClass.sub_commands + [
                                                                ('pbs_config', lambda x:True)
                                                            ]
cmd_classes = {
                'build_ext'    : build_extClass,
                'pbs_config'   : PBSPyConfigure
              }

#: #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#:                       All Extension Objects
#: #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

define_macros = []

define_macros.append(('PACKAGE_ROOT_NAME',PACKAGE_ROOT_NAME))
define_macros.append(('PACKAGE_VERSION',PACKAGE_VERSION))
define_macros.append(('PBSPY_EXT_MODULE_NAME','"%s"' % (PBSPY_EXT_MODULE_NAME,)))
define_macros.append(('PBSPY_EXT_PACKAGE_NAME','"%s"' % (PBSPY_EXT_PACKAGE_NAME,)))
define_macros.append(('MODULE_FUNCTION_NAME',"init%s" % (PBSPY_EXT_MODULE_NAME,)))

PBSPyExtension = ExtensionClass(
                   name=PBSPY_EXT_FULL_NAME,
                   sources=sources,
                   depends=depends,
                   define_macros=define_macros,
                   language="c"
                 )


#: ############################################################################
#:             INVOKE setup (aka MAIN Program)
#: ############################################################################

#: build the setup keyword argument list.
setup_args = {
                'name'                             : PACKAGE_ROOT_NAME,
                #distclass=PBSPy_Distribution,
                'version'                          : PACKAGE_VERSION,
                'maintainer'                       : "Mohan Kannapareddy",
                'maintainer_email'                 : "kmoh.raj@gmail.com",
                'author'                           : "Mohan Kannapareddy",
                'author_email'                     : "kmoh.raj@gmail.com",
                'url'                              : "http://code.google.com/p/py-pbs",
                'download_url'                     : "http://code.google.com/p/py-pbs/downloads/list",
                'license'                          : "Apache Public License 2.0",
                'platforms'                        : ["any"],
                'description'                      : __doc__.split("\n")[0],
                'long_description'                 : "\n".join(__doc__.split("\n")[2:]),
                'classifiers'                      : filter(None, classifiers.split("\n")),
                'data_files'                       : data_files,
                'packages'                         : packages,
                'package_dir'                      : package_dir,
                'cmdclass'                         : cmd_classes,
                'ext_modules'                      : [PBSPyExtension]
             }

setup(**setup_args)

