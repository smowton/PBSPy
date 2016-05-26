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

class PBS_CMDSTestCase(unittest.TestCase):
    """
    """
    def setUp(self):
        """setup prior to *each* test"""

    def tearDown(self):
        """Tear down after each test"""
test_suite = unittest.makeSuite(PBS_CMDSTestCase)

if __name__ == '__main__':
    test_params.initialize()
    unittest.TextTestRunner(verbosity=2).run(test_suite)
    test_params.destroy()
