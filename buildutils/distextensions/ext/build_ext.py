# -*- coding: UTF-8 -*
"""Module providing a wrapper for the standard build_ext class, as there are some deficiencies. Namely:
    
    - No ability to add subcommands? and we need it to run some configure scripts

""" 


from distutils.command.build_ext   import build_ext as build_extStandardClass
from types import StringType
from distutils import log

class build_extExtended(build_extStandardClass):
    """
    """
    command_name = "build_ext"
    
    def update_options_after_running_subcommands(self):
        """
            ASSUMPTIONS:
                - it is assumed that the attribute values support '+' operator, otherwise bad thigs happen.
            TODO:
                cmd_obj 
        """
        attrs_to_be_updated  = ('include_dirs','libraries','library_dirs','rpath','define','undef')
        for cmd_name in self.get_sub_commands():
            cmd_obj = self.distribution.get_command_obj(cmd_name)
            for attr in attrs_to_be_updated:
                cmd_obj_val = getattr(cmd_obj,attr,None)
                cur_val = getattr(self,attr,None)
                if cur_val is not None:
                    setattr(self, attr, cur_val + cmd_obj_val)
                else:
                    setattr(self, attr, cmd_obj_val)
            #: end for attr
        #: end for cmd_name
    #: end m(update_options_after_running_subcommands)
    
    def run (self):
        """
            NOTES:
                - At this point finalize_options should have already been called, hence we can expect the data
                  types for various attribute values to be consistent with the semantics.
        """
        if not self.extensions:
            return
        # Run all relevant sub-commands.  This will be some subset of:
        for cmd_name in self.get_sub_commands():
            self.run_command(cmd_name)
        #: now update any options that were discovered as a result
        self.update_options_after_running_subcommands()
        #: call the master
        build_extStandardClass.run(self)
   #: end m(run)
   
#: end c(build_extExtended)
