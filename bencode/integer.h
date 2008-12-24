#ifndef _BENCODE_INTEGER_H_
#define _BENCODE_INTEGER_H_

#include <bencode/element.h>

namespace BEncode {

	class Integer : public Element {
		public:
			Integer(int64_t = 0);
			~Integer();

			int64_t value() const;

			ElementType type() const;
			std::string toString() const;
			
		private:
			Integer(const Integer&);
			Integer& operator=(const Integer&);

			int64_t _val;
	};

}

#endif
