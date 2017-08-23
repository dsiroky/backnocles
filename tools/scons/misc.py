# -*- coding: utf-8 -*-
"""
Default stuff.
"""

import os
import sys

import SCons

#==========================================================================

def embed_manifest(env, srcs):
    env.AddPostAction(srcs, SCons.Action.Action(
              "mt.exe -nologo -manifest ${TARGET}.manifest -outputresource:$TARGET;2",
              "$EMBEDMANIFESTCOMSTR"))
    return []

#--------------------------------------------------------------------------

def find_temp_dir(env):
    if os.name == "nt":
        tmp = os.environ.get("TMP")
        if tmp is not None:
            return tmp
        else:
            for d in ("c:\\tmp",):
                if os.path.isdir(d):
                    return d
            else:
                return "c:\\temp"
    else:
        return os.environ.get("TMP") or "/tmp"

#--------------------------------------------------------------------------

def find_library_name(env, lib):
    """
    Rename library name according to the build type.
    """
    if env["USE_MSVC"]:
        paths = []
        if "LIBPATH" in env:
            paths += env.subst(env["LIBPATH"])
        paths += os.environ["LIB"].split(";")
        paths = [p for p in paths if p != ""]

        if env["COMPILE_DEBUG"]:
            debug = "d"
        else:
            debug = ""

        for p in paths:
            libn = "%s%s" % (lib, debug)
            fn = os.path.join(p, libn + ".lib")
            if os.path.isfile(fn):
                lib = libn
                break

    return lib

#--------------------------------------------------------------------------

def setup_linker(env, builder_name):
    def builder(env, target, sources, **args):
        orig_builder = env["BUILDERS"][builder_name + "original"]
        target = orig_builder(env, target, sources, **args)
        if env["USE_MSVC"]:
            # scons issue #1745
            env.Clean(target[0], os.path.splitext(target[0].abspath)[0] + ".ilk")
        # for incremental linking
        env.Precious(target)
        return target

    env["BUILDERS"][builder_name + "original"] = env["BUILDERS"][builder_name]
    env["BUILDERS"][builder_name] = builder

#--------------------------------------------------------------------------

def pretify_output(env, verbose, commands):
    try:
        # MSW terminal coloring
        import colorama
        colorama.init()
    except ImportError:
        pass

    # pretty print
    if not verbose:
      for k in commands:
          env[k + "COMSTR"] = "%s $TARGET" % k

    # coloring
    if sys.stdout.isatty():
        for k in commands:
            envk = k + "COMSTR"
            if envk in env:
                env[envk] = "\033[36m%s\033[0m" % env[envk]

#==========================================================================

def generate(env):
    env.AddMethod(embed_manifest, "EmbedManifest")
    env.AddMethod(find_library_name, "FindLibraryName")
    env.AddMethod(pretify_output, "PretifyOutput")
    env.AddMethod(find_temp_dir, "FindTempDir")
    env.AddMethod(setup_linker, "SetupLinker")

def exists(env):
    return True
