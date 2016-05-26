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
__revision__ = "$Id: test_pbs_attr.py 13 2007-11-30 02:24:22Z kmohraj $"
if __name__ == '__main__':
    import sys,os
    sys.path.append('..')

from init_tests import api,test_params,ConfigParserError
import unittest

class PBSAttrTestCase(unittest.TestCase):
    """Test the various PBSAttr creation, deletion etc..
    """
    def setUp(self):
        """setup prior to *each* test"""

    def tearDown(self):
        """Tear down after each test"""

    def test00_Attr(self):
        """
        """
        print

        Attr = api.Attr
        bop = api.BatchOp('unset')
        name = 'Junk'
        value = None
        #: Parameters, required parameters are name,value
        self.failUnlessRaises((TypeError,),Attr)
        self.failUnlessRaises((TypeError,),Attr,name=name)
        #: Test all the 'op' argument lists
        a = Attr(name,value,bop=None)
        print a
        _fmt = "Got '%s' instead of '%s'"
        self.failUnlessEqual(a.name,name, _fmt % (a.name,name))
        print "\t\t a:",a
        a.bop = 'SET'
        self.failUnlessEqual(a.bop.op,'SET', _fmt % (a.bop.op,'SET'))
        print "\t\t a:",a
        a.bop = api.BatchOp('UNSET')
        self.failUnlessEqual(a.bop.op,'UNSET', _fmt % (a.bop.op,'UNSET'))
        print "\t\t a:",a

        
    def test100_test(self):
        """test list stuff"""
        print

        Attr = api.Attr
        al = [
            Attr(name='Resource_List',resource='ncpus',value='100',bop='SET'),
            Attr(name='Resource_List',resource='mem',value='1gb',bop='SET'),
            Attr(name='Resource_List',resource='scratch',value='10gb'),
            Attr(name='Jobname',value='Hmmmm'),
            ]
        print "Orig List: ",al
        print "calling test_cattrl:\n",api.test_cattr(al)
        
test_suite = unittest.makeSuite(PBSAttrTestCase)

if __name__ == '__main__':
    test_params.initialize()
    unittest.TextTestRunner(verbosity=2).run(test_suite)
    test_params.destroy()
