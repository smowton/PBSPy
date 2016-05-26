# -*- coding: UTF-8 -*
"""
Common utitlity functions not found in the standard distutils modules

"""


import os
import types

def search_file(filename,path,exts=None):
    """Search for `filename` in the directories specified in `path`

    where::
      . ``path`` : a list of os.pathsep string or list of directories
    """

    if type(path) == types.StringType:
        dir_list = [d.strip() for d in path.split(os.pathsep)]
    elif type(path) in (types.ListType,types.TupleType):
        dir_list = path
    else:
        raise TypeError,"path argument must be os.pathsep string or a list of directories"

    filename_list = []
    if exts is not None and type(exts) in (types.ListType,types.TupleType):
        for e in exts:
            filename_list.append('%s.%s' % (filename,e))
    else:
        filename_list.append(filename)
    for d in dir_list:
        for f in filename_list:
            filepath = os.path.abspath(os.path.join(d,f))
            if os.path.exists(filepath):
                return filepath

    return None
#: end f(search_file)

def write_version_string_to_package(filename,new_version_str):
    """convenience function to replace the value of __version__ to new_version_str in filename.
    
        arguments:
            filename       : name of file containing __version__
            new_version_str: the new version string
        
        Notes:
            - No check for exceptions whether filename exists or not.
            - if __version__ line is not found then nothing changes
            - the value of __version__ if present *MUST* be in double quotes.
            - __version__ is a standalone statement and must start at column 1.
    """
    import fileinput
    import sys
    def findversionstring(line):
        q_si = line.find('"')
        if q_si == -1:
            sys.stderr.write("WARNING: *** __init__.py has __version__  NOT in double quotes\n")
            return ""
        q_ei = line.find('"',q_si+1)
        return line[q_si+1:q_ei]
    #:end nested_f(findversionstring)
    found_version_line = False
    version_file = fileinput.FileInput(filename,inplace=1)
    for line in version_file:
        if line.startswith("__version__"):
            current_version_str = findversionstring(line)
            if current_version_str == new_version_str:
                sys.stderr.write("    INFO: version string *unchanged* in '%s'\n" % (filename,))
            line = line.replace(current_version_str,new_version_str)
            found_version_line = True
        sys.stdout.write(line)
    version_file.close()
    if not found_version_line:
        raise SystemExit,"ERROR: no __version__ line found in %s" % (filename,)
    
#: end f(sync_module_version)
 
if __name__ == '__main__':

    print search_file('stdio.h',"/opt/include:/opt/junk:/usr/include")
    try:
        print search_file('stdio.h',2323)
    except (TypeError),(e):
        print e
    print search_file('stdio.h',['/opt/include','/usr/include'])
    print """\t* search_file('/usr/include/stdio.h',[])"""
    print search_file('/usr/include/stdio.h',[])

