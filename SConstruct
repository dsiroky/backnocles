import os
import sys
import subprocess
import platform

#==========================================================================

PROJECT_ROOT = Dir("#").abspath
TOOL_PATH = os.path.join(PROJECT_ROOT, "tools/scons")

#==========================================================================
# build options

DEBUG = 1
VERBOSE = 0
SANITIZERS = ""

debug = bool(int(ARGUMENTS.get("debug", DEBUG)))
verbose = bool(int(ARGUMENTS.get("verbose", VERBOSE)))
sanitizers = ARGUMENTS.get("sanitizers", SANITIZERS)

Help("""
Variants
--------
scons verbose=(0|1)              print short or long processing commands (default: %(verbose)i)
scons debug=(0|1)                compile debug or release (default: %(debug)i)
scons sanitizers=address,leak    link sanitizers (not in release build, default: "%(sanitizers)s")
scons run_e2e_tests
""" % {"verbose":VERBOSE, "debug":DEBUG,
        "sanitizers":SANITIZERS})

#==========================================================================

Decider("MD5-timestamp")
CacheDir(os.path.join(PROJECT_ROOT, ".build_cache"))

#==========================================================================

env = Environment(ENV=os.environ, toolpath=[TOOL_PATH])

env["PROJECT_ROOT"] = PROJECT_ROOT
env["ENV"]["PROJECT_ROOT"] = PROJECT_ROOT

if platform.system() == "Windows":
    env["OS"] = "msw"
    env["TARGET_CPU"] = (os.environ.get("TARGET_CPU") or
                        os.environ.get("TARGET_ARCH") or
                        "x86_64")
elif "linux" in sys.platform:
    env["OS"] = "lnx"
    env["TARGET_CPU"] = subprocess.Popen("uname -m", shell=True,
                              stdout=subprocess.PIPE).stdout.read().strip()
else:
    env["OS"] = "osx"
    env["TARGET_CPU"] = subprocess.Popen("uname -m", shell=True,
                              stdout=subprocess.PIPE).stdout.read().strip()
env["ARCH"] = "${OS}_${TARGET_CPU}"

# import OS environment variables
for var in (
        "MSVC_VERSION", "TARGET_ARCH" # for MSVC scons toolkit
        ):
    if var in os.environ:
        env[var] = os.environ[var]

#==========================================================================

env["CC"] = os.environ.get("CC") or env["CC"]
env["CXX"] = os.environ.get("CXX") or env["CXX"]

env["COMPILE_DEBUG"] = debug
env["COMPILE_VERBOSE"] = verbose
env["CXXFILESUFFIX"] = ".cpp" # default is ".cc"
env["USE_MSVC"] = "msvc" in env["TOOLS"]
if env["USE_MSVC"]:
    env["PDB"] = "${TARGET.base}.pdb"

env.Tool("misc")
env.Tool("boost")

env["TEMPDIR"] = env.FindTempDir()

env.SetupLinker("Program")
env.SetupLinker("SharedLibrary")
env.PretifyOutput(verbose,
            ["CC", "CXX", "LINK", "SHCC", "SHCXX", "SHLINK", "AR", "RANLIB",
            "GCH", "GCHSH",
            "EMBEDMANIFEST"])

#==========================================================================

env.AppendUnique(
    CPPPATH=os.environ.get("CPPPATH", "").split(os.pathsep),
    LIBPATH=os.environ.get("LIBPATH", "").split(os.pathsep),
)

conan = SConscript("SConscript_conan")
if conan != None:
    env.AppendUnique(
            CPPPATH=conan["conan"]["CPPPATH"],
            LIBPATH=conan["conan"]["LIBPATH"]
        )

env.AppendUnique(
    CPPPATH=[
        "#",
        "#/src",
        ],
    CPPDEFINES=[
        "PROJECT_ROOT=\\\"%s\\\"" % env["PROJECT_ROOT"].replace("\\", "\\\\"),
        "BOOST_NO_RTTI",
        "BOOST_NO_TYPEID",
    ],
)

if env["USE_MSVC"]:
    env.AppendUnique(
        CCFLAGS=[
            "/std:c++17",
            "/Zm2000",
            "/W4",
            "/wd4503", # "decorated name length exceeded, name was truncated"
            "/wd4800", # "forcing value to bool, performance warning"
            ],
        CXXFLAGS=["/EHa"],
        CPPDEFINES=[
            "WIN32",
            "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS",
            "_CRT_NONSTDC_NO_DEPRECATE",
            "_WINSOCK_DEPRECATED_NO_WARNINGS",
            "_ALLOW_KEYWORD_MACROS",
            "NOMINMAX",
            "WIN32_LEAN_AND_MEAN", # exclude rarely used MSW stuff
            "_WIN32_WINNT=0x0601", # Win 7
            ],
        LINKFLAGS=[
            "/ignore:4204", # missing debugging information for referencing module
            "/MACHINE:X64",
            ],
    )

    if debug:
        env.AppendUnique(
                CPPDEFINES = ["_DEBUG", "DEBUG"],
                CCFLAGS=["/Od", "/Z7", "/MDd"],
                LINKFLAGS=["/DEBUG"],
            )
    else:
        env.AppendUnique(
                CPPDEFINES = ["NDEBUG"],
                CCFLAGS=["/O2", "/Gy", "/MD"],
                LINKFLAGS=["/OPT:REF,ICF"],
            )
else:
    env.AppendUnique(
        CCFLAGS=[
                "-pipe",
                "-pthread",

                "-fPIC",
                "-fstrict-aliasing",
                "-fsigned-char",
                "-fno-rtti",

                "-pedantic",
                "-Wall",
                "-Wextra",
                "-Wdeprecated",
                "-Wfloat-conversion",
                "-Wfloat-equal",
                "-Wmissing-braces",
                "-Wstrict-aliasing",
                "-Wswitch",
                "-Wswitch-default",
                "-Wuninitialized",
                "-Wunreachable-code",
                "-Wunused-function",
                "-Wunused-parameter",
                "-Wunused-result",
                "-Wunused-value",
                "-Wunused-variable",
            ],
        CFLAGS=["-std=c11"],
        CXXFLAGS=[
                "-std=c++17",

                "-Wnon-virtual-dtor",
                "-Wold-style-cast",
                ],
        LINKFLAGS=["-pthread"]
        )

    if "clang" in env["CC"]:
        env.AppendUnique(
                CPPDEFINES=[
                    # clang does not turn on WUR checking in libstdc++
                    "__attribute_warn_unused_result__=__attribute__ ((__warn_unused_result__))",
                    "__wur=__attribute__ ((__warn_unused_result__))",
                ],
                CCFLAGS=[
                    "-stdlib=libc++",

                    "-fcolor-diagnostics",
                    "-ferror-limit=10",

                    "-Wabstract-vbase-init",
                    "-Waddress-of-array-temporary",
                    "-Warray-bounds",
                    "-Warray-bounds-pointer-arithmetic",
                    "-Wassign-enum",
                    "-Wattributes",
                    "-Wbool-conversion",
                    "-Wbridge-cast",
                    "-Wbuiltin-requires-header",
                    "-Wc++11-narrowing",
                    "-Wcast-align",
                    "-Wchar-subscripts",
                    "-Wconditional-uninitialized",
                    "-Wconstant-conversion",
                    "-Wconstant-logical-operand",
                    "-Wconstexpr-not-const",
                    "-Wconsumed",
                    "-Wdangling-else",
                    "-Wdangling-field",
                    "-Wdeprecated-increment-bool",
                    "-Wduplicate-method-match",
                    "-Weffc++",
                    "-Wempty-body",
                    "-Wenum-conversion",
                    "-Wint-conversion",
                    "-Wparentheses",
                    "-Wpointer-sign",
                    "-Wshadow",
                    "-Wshorten-64-to-32",
                    "-Wtautological-compare",
                    "-Wthread-safety-analysis",
                    "-Wundeclared-selector",
                ],
            LINKFLAGS=[
                    "-stdlib=libc++",
                ]
            )
    else:
        env.AppendUnique(
                CCFLAGS=[
                    "-fmax-errors=10"
                ]
            )


    if debug:
        env.AppendUnique(
                CPPDEFINES = ["_DEBUG"],
                CCFLAGS=[
                    "-O0",
                    "-ggdb3",
                    "-fno-omit-frame-pointer",
                    "-fno-optimize-sibling-calls",
                    "-fno-inline-functions",
                    "-funwind-tables"
                ]
            )
        if sanitizers:
            SANITIZERS = ["-fsanitize=" + sanitizers]
            env.AppendUnique(CCFLAGS=SANITIZERS, LINKFLAGS=SANITIZERS)
    else:
        env.AppendUnique(
                CPPDEFINES=["NDEBUG"],
                CCFLAGS=[
                    "-O3",
                    "-Werror",
                    "-Wdisabled-optimization",
                    "-Wno-error=strict-overflow", # gcc false warnings
                ]
            )
        if sys.platform != "darwin":
            env.AppendUnique(
                    LINKFLAGS=["-Wl,--strip-all"],
                )

#==================================================================

Export("env")

#==================================================================

build_dir_name = "build/${ARCH}_%s" % ("debug" if debug else "release")
env["BUILD_DIR"] = env.Dir(build_dir_name)
env.SConsignFile("$BUILD_DIR/.sconsign")

SConscript("SConscript", variant_dir=env["BUILD_DIR"])

if env["OS"] != "msw":
    if sys.platform == "darwin":
        env.Execute("rm -f build/last ; ln -sf ${BUILD_DIR.abspath} build/last")
        env.Execute("rm -f build/${ARCH}_last ; ln -sf ${BUILD_DIR.abspath} build/${ARCH}_last")
    else:
        env.Execute("ln -sfrn $BUILD_DIR build/last")
        env.Execute("ln -sfrn $BUILD_DIR build/${ARCH}_last")
