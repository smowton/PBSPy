# -*- coding: UTF-8 -*

#REMOVE_CC_OPTS_FLAGS = (
#    '-Wstrict-prototypes',
#    )
from distutils.core import DEBUG


def remove_from_config_vars(config_vars_remove_flags_map={}):
    from distutils import sysconfig
    save_sysconfig_init_posix = sysconfig._init_posix
    def hack_sysconfig_init_posix(config_vars_remove_flags_map=config_vars_remove_flags_map):
        """hack to remove flags from default build of python.
        
            NOTES:
                - IMPORTANT to maintain a 0 arg for _init_posix() clone which is myself. That is why default
                  is provided.
        """
        save_sysconfig_init_posix()
        g = sysconfig._config_vars
        for (optvar,remove_optval) in config_vars_remove_flags_map.iteritems():
            new_value = current_value = g.get(optvar,'')
            for flag in remove_optval:
                new_value = current_value.replace(flag,'')
            g[optvar] = new_value
            if DEBUG:
                print "config var: '%s', old val: '%s', new val: '%s'" % (optvar,current_value,new_value)
    #: end hack_sysconfig_init_posix
    sysconfig._init_posix = hack_sysconfig_init_posix
#: end    remove_from_config_vars 


if __name__ == '__main__':
    from distutils.ccompiler import show_compilers
    show_compilers()