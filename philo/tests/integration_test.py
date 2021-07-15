#!/usr/bin/python3

from pathlib import Path
import random
import subprocess
from enum import IntEnum

LIGHT_RED = "\033[1;31m"
LIGHT_GREEN = "\033[1;32m"
LIGHT_PURPLE = "\033[1;35m"
LIGHT_WHITE = "\033[0;37m"
LIGHT_YELLOW = "\033[0;33m"
RESET = "\033[0m"
RUN_COMMAND = "./philo {}"
EXPECT_OK = "Expect ok"
EXPECT_ERROR = "Expect Error"
EXPECT_DEATH = "Expect Death"

class Status(IntEnum):
    OK = 0
    DEATH = 2
    ERROR = 3

    @classmethod
    def print(cls, expected):
        if expected == cls.OK:
            print(EXPECT_OK)
        elif expected == cls.DEATH:
            print(EXPECT_DEATH)
        else:
            print(EXPECT_ERROR)

def printTestBanner(testName):
    print("{}======================================================================{}".format(LIGHT_PURPLE, LIGHT_WHITE))
    print("                          {}".format(testName))
    print("{}======================================================================{}".format(LIGHT_PURPLE, LIGHT_WHITE))


class ErrorManagement:

    def __init__(self):
        self.name = "Error Management"

    def _runInput(self, testName, inp):
        print("\n{}Test case: {}{}: ./philo {}{}".format(LIGHT_PURPLE, LIGHT_YELLOW, testName, inp, RESET))
        ret = subprocess.run(RUN_COMMAND.format(inp), shell=True)
        assert ret.returncode == int(Status.ERROR)

    def _testInvalidParameters(self):
        self._runInput("Non numeric parameters", "4 A 200 85")
        self._runInput("Invalid Number of Philosophers", "1 310 200 100")
    
    def _testWrongNumberOfParameters(self):
        self._runInput("Less than 5 parameters", "4 20 85")
        self._runInput("More than 6 parameters", "4 20 85 200 80 90")

    def runTests(self):
        printTestBanner("Error management")
        self._testInvalidParameters()
        self._testWrongNumberOfParameters()


class DeadlyParameters:
    
    def __init__(self):
        self.name = "Deadly Parameters"

    def _runInput(self, testName, inp):
        print("\n{}Test case: {}./philo {}".format(LIGHT_PURPLE, RESET, inp))
        ret = subprocess.run(RUN_COMMAND.format(inp), shell=True)
        print("\n\t{}{}{}\n".format(LIGHT_YELLOW, testName, RESET))
        assert ret.returncode == int(Status.DEATH)

    def _testDeathDetection(self):
        self._runInput("Philo should die arround 310ms", "4 310 200 100")
        self._runInput("Philo should die arround 60ms", "4 60 60 60")

    def runTests(self):
        printTestBanner(self.name)
        self._testDeathDetection()



class GoodParameters:
    
    def __init__(self):
        self.name = "Good Parameters"

    def _runInput(self, testName, inp):
        print("\n{}Test case: {}./philo {}".format(LIGHT_PURPLE, RESET, inp))
        ret = subprocess.run(RUN_COMMAND.format(inp), shell=True)
        print("\n\t{}{}{}\n".format(LIGHT_YELLOW, testName, RESET))
        assert ret.returncode == int(Status.OK)

    def _testWithMinMeals(self):
        self._runInput("No philosopher should die", "4 410 200 200 10")
        self._runInput("No philosopher should die", "3 310 100 100 10")

    def runTests(self):
        printTestBanner(self.name)
        self._testWithMinMeals()


def main():
    assert Path('./philo').is_file()
    ErrorManagement().runTests()
    DeadlyParameters().runTests()
    GoodParameters().runTests()

main()
