#include <bencode/bytestr.h>

#include <sstream>

namespace BEncode {
	ByteStr::ByteStr() : _buf(NULL), _len(0) {
	}

	ByteStr::~ByteStr() {
	}

	size_t ByteStr::length() const {
		return _len;
	}

	const char* ByteStr::data() const {
		return _buf;
	}

	Element::ElementType ByteStr::type() const {
		return BYTESTR;
	}

	std::string ByteStr::toString() const {
		std::ostringstream s;
		s << length() << ':' << std::string(data(), length());
		return s.str();
	}
}
