# About

**Backnocles** is a multiplatform C++ demo program for educational purposes.

# License

MIT License, see LICENSE.txt.

# Prerequisites

- g++/clang++, scons, libboost-all-dev, python, python-nose, doxygen

# Tests

- unit tests: `scons tests && ./build/last/tests/unit/run`
- border tests: `scons tests && ./build/last/tests/border/run`
- end to end tests: `scons run_e2e_tests`

# Docs

1. `./build_doc.sh`
1. Open `build/doxygen/html/index.html`.
