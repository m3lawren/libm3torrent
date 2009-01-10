#ifndef _BENCODE_DICTIONARY_H_
#define _BENCODE_DICTIONARY_H_

#include <bencode/element.h>

#include <vector>

namespace BEncode {

	class ByteStr;

	class Dictionary : public Element {
		public:
			Dictionary();
			~Dictionary();

			size_t length() const;
			const ByteStr& key(size_t) const;
			const Element& val(size_t) const;

			ElementType type() const;
			std::string toString() const;

		private:
			Dictionary(const Dictionary&);
			Dictionary& operator=(const Dictionary&);

			std::vector<ByteStr*> _k;
			std::vector<Element*> _v;
	};

}

#endif
