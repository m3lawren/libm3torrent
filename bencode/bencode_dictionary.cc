#include <bencode/bencode.h>

#include <sstream>

namespace BEncode {
	Dictionary::Dictionary() {
	}

	Dictionary::~Dictionary() {
		for (size_t idx = 0; idx < _k.size(); idx++) {
			delete _k[idx];
			delete _v[idx];
		}
	}

	size_t Dictionary::length() const {
		return _k.size();
	}

	const ByteStr& Dictionary::key(size_t idx) const {
		return *_k[idx];
	}

	const Element& Dictionary::val(size_t idx) const {
		return *_v[idx];
	}

	Element::ElementType Dictionary::type() const {
		return DICTIONARY;
	}

	std::string Dictionary::toString() const {
		std::ostringstream s;
		for (size_t idx = 0; idx < length(); idx++) {
			s << key(idx).toString() << val(idx).toString();
		}
		return s.str();
	}
}
