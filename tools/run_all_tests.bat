call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

scons tests && build\msw_x86_64_debug\tests\unit\run && build\msw_x86_64_debug\tests\border\run && scons run_e2e_tests
