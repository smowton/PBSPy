import types

def demangle_libraries_str(s):
    """Make sure the libraries string is such that it can be properly parsed
    """

    if type(s) is not types.StringType:
        raise TypeError,"argument must be a string but i got <%s>" % (s,)

    #nuke -l's
    s = s.replace('-l','')

    return s
 
