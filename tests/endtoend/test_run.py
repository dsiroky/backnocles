# -*- coding: utf-8 -*-
"""
Test server run.
"""

import os
import unittest
import threading
import time

from subprocess import Popen, PIPE

import nose.tools as nt

#===========================================================================

SERVER_PROG = os.environ["SERVER_PROG"]

#===========================================================================

class KillAfterTimeout(object):
    def __init__(self, process, timeout):
        """
        @param timeout [s]
        """
        self.process = process
        self.timer = threading.Timer(timeout, self.kill_him)
        self.timer.start()

    def __enter__(self):
        pass

    def __exit__(self, tp, value, traceback):
        self.cancel()

    def cancel(self):
        self.timer.cancel()

    def kill_him(self):
        try:
            self.process.terminate()
        except:
            pass

#===========================================================================

class TestRun(unittest.TestCase):
    def TestAwaitsInput(self):
        """
        No input - the program will be terminated after a timeout.
        """
        process = Popen([SERVER_PROG], stdout=PIPE)
        tstart = time.time();
        with KillAfterTimeout(process, 1):
            (output, err) = process.communicate()
            exit_code = process.wait()
        tend = time.time()

        nt.assert_not_equal(exit_code, 0)
        # killed by timeout
        nt.assert_greater(tend - tstart, 0.5)

    #----------------------------------------------------

    def TestCommandQuit_ProgramQuits(self):
        """
        The program will terminate immediately after issuing the quit command.
        """
        process = Popen([SERVER_PROG], stdout=PIPE, stdin=PIPE)

        tstart = time.time();
        with KillAfterTimeout(process, 1):
            (output, err) = process.communicate("quit\n")
            exit_code = process.wait()
        tend = time.time()

        nt.assert_equal(exit_code, 0)
        # a natural death
        nt.assert_less(tend - tstart, 0.5)

    #----------------------------------------------------

    def TestCommandAddAnimal_ProgramPrintsAnimalAdded(self):
        process = Popen([SERVER_PROG], stdout=PIPE, stdin=PIPE)

        with KillAfterTimeout(process, 1):
            (output, err) = process.communicate("add animal hippo\nquit\n")
            exit_code = process.wait()

        nt.assert_equal(exit_code, 0)
        nt.assert_equal(output, "hippo added\n")
