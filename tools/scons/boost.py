import os
import glob
import re
import sys

class Found(BaseException):
    pass

def add_boost(env, libs):
    if env["USE_MSVC"]:
        # compiled boost libraries might have extra prefix and postfix
        paths = []
        if "LIBPATH" in env:
            paths += env.subst(env["LIBPATH"])
        paths += os.environ["LIB"].split(";")
        paths = [p for p in paths if p != ""]

        if "/MDd" in env["CCFLAGS"]:
            SUFFIXES = ["-vc*-mt-gd-*"]
        elif "/MD" in env["CCFLAGS"]:
            SUFFIXES = ["-vc*-mt-*"]
        elif "/MTd" in env["CCFLAGS"]:
            SUFFIXES = ["-vc*-mt-sgd-*"]
        elif "/MT" in env["CCFLAGS"]:
            SUFFIXES = ["-vc*-mt-s-*"]

        renamed_libs = []
        for lib in libs:
            try:
                for suffix in SUFFIXES:
                    for p in paths:
                        fnglob = os.path.join(p, "lib" + lib + suffix + ".lib")
                        files = glob.glob(fnglob)
                        if len(files) > 0:
                            m = re.search(r"([^\\]*[.]lib)", files[0])
                            renamed_libs.append(m.group(1))
                            raise Found()
            except Found:
                continue
            else:
                renamed_libs.append(lib)

        env.AppendUnique(LIBS=renamed_libs)
    else:
        if sys.platform == "darwin":
            rawlibs = libs
            libs = []
            for l in rawlibs:
                if env["COMPILE_DEBUG"]:
                    libs.append(l + "-mt-d")
                else:
                    libs.append(l + "-mt")
        env.AppendUnique(LIBS=libs)

def generate(env):
    libboost_dir = env.get("LIBBOOST_DIR") or os.environ.get("LIBBOOST_DIR")
    if libboost_dir is not None:
        env.AppendUnique(CPPPATH=[os.path.join(libboost_dir, "include")])
    env.AddMethod(add_boost, "AddBoost")

def exists(env):
    return True
