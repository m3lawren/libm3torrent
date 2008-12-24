pwd = Dir('.')

env = Environment(CCFLAGS='-g -Wextra -Wall -Werror -std=c++98 -pedantic', CPPPATH=[pwd])

sources = [
	'bencode.cc',
	'bencode_bytestr.cc',
	'bencode_dictionary.cc',
	'bencode_integer.cc',
	'bencode_list.cc',
]

env.Library('m3torrent', sources)
