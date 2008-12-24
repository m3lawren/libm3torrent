import os

pwd = Dir('.')

env = Environment(CCFLAGS='-O2 -Wextra -Wall -Werror -std=c++98 -pedantic', CPPPATH=[pwd], LIBPATH=[pwd])

gcov = ARGUMENTS.get('gcov', 0)
gprof = ARGUMENTS.get('gprof', 0)
ctags = ARGUMENTS.get('ctags', 0)
debug = ARGUMENTS.get('debug', 0)

if int(gcov):
	env.Append(CCFLAGS=' -fprofile-arcs -ftest-coverage', LINKFLAGS=' -fprofile-arcs -ftest-coverage')
if int(gprof):
	env.Append(CCFLAGS=' -pg', LINKFLAGS=' -pg')
if int(ctags):
	os.system('ctags -R *')
if int(debug):
	env.Append(CCFLAGS=' -g')

Export('env')

bencode_objs = SConscript('bencode/SConscript')
test_objs = SConscript('test/SConscript')

env.Library('m3torrent', bencode_objs)
env.Program('libm3torrent_test', test_objs, LIBS=['m3torrent'])

