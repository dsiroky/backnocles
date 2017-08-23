# -*- coding: utf-8 -*-
"""
Test server run.
"""

import os
import unittest

from subprocess import Popen, PIPE

import nose.tools as nt

#===========================================================================

SERVER_PROG = os.environ["SERVER_PROG"]

#===========================================================================

class TestRun(unittest.TestCase):
    def TestOutput(self):
        process = Popen([SERVER_PROG], stdout=PIPE)
        (output, err) = process.communicate()
        exit_code = process.wait()

        nt.assert_equal(exit_code, 0)
