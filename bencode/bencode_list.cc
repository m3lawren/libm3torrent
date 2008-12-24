#include <bencode/bencode.h>

#include <sstream>

namespace BEncode {	
	List::List() {
	}

	List::~List() {
		for (size_t idx = 0; idx < _elements.size(); idx++) {
			delete _elements[idx];
		}
	}

	size_t List::length() const {
		return _elements.size();
	}

	const Element& List::operator[](size_t idx) const {
		return *_elements[idx];
	}

	Element::ElementType List::type() const {
		return LIST;
	}

	std::string List::toString() const {
		std::ostringstream s;
		for (size_t idx = 0; idx < length(); idx++) {
			s << (*this)[idx].toString();
		}
		return s.str();
	}
}
