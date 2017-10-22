pip install --egg scons
pip install conan nose pypiwin32
conan remote add siroky https://siroky.cz/conan
conan install -r siroky --build=missing .
