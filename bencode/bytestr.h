#ifndef _BENCODE_BYTESTR_H_
#define _BENCODE_BYTESTR_H_

#include <bencode/element.h>

namespace BEncode {
	
	class ByteStr : public Element {
		public:
			ByteStr();
			~ByteStr();

			size_t length() const;	
			const char* data() const;

			ElementType type() const;
			std::string toString() const;

		private:
			ByteStr(const ByteStr&);
			ByteStr& operator=(const ByteStr&);

			char* _buf;
			uint64_t _len;
	};

}

#endif
