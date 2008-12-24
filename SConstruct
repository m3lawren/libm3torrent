pwd = Dir('.')

env = Environment(CCFLAGS='-g -Wextra -Wall -Werror -std=c++98 -pedantic', CPPPATH=[pwd])
Export('env')

objs = SConscript('bencode/SConscript')

env.Library('m3torrent', objs)
