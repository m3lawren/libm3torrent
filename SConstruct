import os

pwd = Dir('.')

env = Environment(CCFLAGS='-g -Wextra -Wall -Werror -std=c++98 -pedantic', CPPPATH=[pwd], LIBPATH=[pwd])
Export('env')

bencode_objs = SConscript('bencode/SConscript')
test_objs = SConscript('test/SConscript')

env.Library('m3torrent', bencode_objs)
env.Program('libm3torrent_test', test_objs, LIBS=['m3torrent'])

os.system('ctags -R *')
