#!/bin/bash
# setup Travis CI environment.

set -e

mkdir -p $HOME/.conan
cat > $HOME/.conan/settings.yml << EOL
os:
    Windows:
    Linux:
arch: [x86_64]
compiler:
    gcc:
        version: ["7.0", "7.1", "7.2"]
        libcxx: [libstdc++]
    clang:
        version: ["5.0"]
        libcxx: [libstdc++, libc++]
build_type: [None, Debug, Release]
EOL

pip3 install --user conan nose
conan remote add siroky https://siroky.cz/conan
conan install -r siroky --build=missing .
