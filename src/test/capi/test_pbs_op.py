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
__revision__ = "$Id: test_pbs_op.py 13 2007-11-30 02:24:22Z kmohraj $"
if __name__ == '__main__':
    import sys,os
    sys.path.append('..')


from init_tests import api,test_params,ConfigParserError
import unittest

_valid_ops = [('SET','+'),
              ('UNSET','-'),
              ('INCR','++'),
              ('DECR','--'),
              ('EQ','='),
              ('NE','!='),
              ('GE','>='),
              ('GT','>'),
              ('LE','<='),
              ('LT','<'),
              ('DFLT','?')
              ]
class BatchOpTestCase(unittest.TestCase):
    """
    """
    def setUp(self):
        """setup prior to *each* test"""

    def tearDown(self):
        """Tear down after each test"""

        
    def test00_new(self):
        """Test the various ways of creating BatchOp
        """
        print
        BatchOp = api.BatchOp
        for (a,s) in _valid_ops:
            op_n = BatchOp(a).op
            op_s = BatchOp(s).op
            #print "op_n:%s, op_s:%s, a: %s, s: %s" % (op_n,op_s,a,s)
            #: op should always be a
            self.failUnlessEqual(op_n,a,"Got <%s> instead of <%s>" % (op_n,a))
            self.failUnlessEqual(op_s,a,"Got <%s> instead of <%s>" % (op_n,a))

    def test01_batchop_exception(self):
        """Test the various exceptions raised.
        """
        exc_l = (AttributeError,TypeError)
        BatchOp = api.BatchOp
        self.failUnlessRaises(exc_l,delattr,BatchOp(),'op')
        self.failUnlessRaises(exc_l,setattr,BatchOp(),'op',1)
        self.failUnlessRaises(exc_l,setattr,BatchOp(),'op','x')
        self.failUnlessRaises(exc_l,api.BatchOp,'xcxc')
        bop = api.BatchOp('DFLT')
        bop.op = '+'
        self.failUnlessEqual(bop.op,'SET')
        
test_suite = unittest.makeSuite(BatchOpTestCase)

if __name__ == '__main__':
    test_params.initialize()
    unittest.TextTestRunner(verbosity=2).run(test_suite)
    test_params.destroy()
