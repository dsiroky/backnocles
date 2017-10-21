#!/bin/bash

set -e

scons tests
./build/last/tests/unit/run
./build/last/tests/border/run
scons run_e2e_tests
