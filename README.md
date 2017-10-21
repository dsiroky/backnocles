[![Build Status](https://travis-ci.org/dsiroky/backnocles.svg?branch=master)](https://travis-ci.org/dsiroky/backnocles)

# Backnocles

A multiplatform C++ demo program for educational purposes.

# License

MIT License, see LICENSE.txt.

# Prerequisites

- g++7 / clang++5 / MSVC++2017
- scons, libboost-all-dev, python, python-nose, doxygen, conan

## MSVC++ 2017 installation

Visual Studio 2017 Build Tools
- C++ Desktop

## Install conan packages

1. `conan remote add siroky https://siroky.cz/conan`
1. `conan install -r siroky --build missing .`

# Tests

- unit tests: `scons tests && ./build/last/tests/unit/run`
- border tests: `scons tests && ./build/last/tests/border/run`
- end to end tests: `scons run_e2e_tests`

# Docs

1. `tools/build_doc.sh`
1. Open `build/doxygen/html/index.html`.
