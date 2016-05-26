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
if __name__ == '__main__':
    import sys,os
    sys.path.append('..')

from init_tests import api,test_params,ConfigParserError
import unittest

class PBSServerTestCase(unittest.TestCase):
    """
    """
    def setUp(self):
        """setup prior to *each* test"""

        PBSServer = api.Server
        self.svr = PBSServer('localhost')
        self.svr.connect()
        
    def tearDown(self):
        """Tear down after each test"""

        self.svr = None
        
    def test20_statserver(self):
        """Test for pbs_statserver
        """
        print
        x = self.svr.statserver([])
        for i in x:
            print i.attribs
            print i.name
            print "<%s>" % (i.text,)
    def test30_statque(self):
        """Test for pbs_statque
        """
        print
        x = self.svr.statque()
        for i in x:
            print i.attribs
            print i.name
            print "<%s>" % (i.text,)

    def test40_statjob(self):
        """Test for pbs_statjob
        """
        print
        self.svr.connect('localhost',timeout=5)
        print self.svr.timeout
        x = self.svr.statjob()
        for i in x:
            print i.attribs
            print i.name
            print "<%s>" % (i.text,)

    def Xtest50_rescquery(self):
        """Test for pbs_rescquery"""
        print
        x = self.svr.rescquery(["node"])
        print x
        
    def Xtest60_submit(self):
        """test submit
        """

        print
    def Xtest666_printing(self):
        """test with print output"""
        print
        x = self.svr.submit()
        print x
test_suite = unittest.makeSuite(PBSServerTestCase)


if __name__ == '__main__':
    test_params.initialize()
    unittest.TextTestRunner(verbosity=2).run(test_suite)
    test_params.destroy()
