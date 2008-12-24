#ifndef _BENCODE_PARSER_H_
#define _BENCODE_PARSER_H_

#include <string>

namespace BEncode {

	class Element;
	
	Element* parse(const char*, size_t);
	Element* parse(const std::string& s) { return parse(s.c_str(), s.length()); }

}

#endif
