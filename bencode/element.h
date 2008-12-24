#ifndef _BENCODE_ELEMENT_H_
#define _BENCODE_ELEMENT_H_

#include <string>

namespace BEncode {
	class Element {
		public:
			enum ElementType {
				BYTESTR,
				INTEGER,
				LIST,
				DICTIONARY
			};

			virtual ~Element();

			virtual ElementType type() const = 0;
			virtual std::string toString() const = 0;
	};
}

#endif
