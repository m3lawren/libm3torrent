#include <bencode.h>

#include <sstream>

namespace BEncode {
	Integer::Integer(int64_t v) : _val(v) {
	}

	Integer::~Integer() {
	}

	int64_t Integer::value() const {
		return _val;
	}

	Element::ElementType Integer::type() const {
		return INTEGER;
	}

	std::string Integer::toString() const {
		std::ostringstream s;
		s << 'i' << _val << 'e';
		return s.str();
	}
}
