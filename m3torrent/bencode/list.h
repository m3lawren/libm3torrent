#ifndef _BENCODE_LIST_H_
#define _BENCODE_LIST_H_

#include <bencode/element.h>

#include <vector>

namespace BEncode {

	class List : public Element {
		public:
			List();
			~List();

			size_t length() const;
			const Element& operator[](size_t) const;

			ElementType type() const;
			std::string toString() const;

		private:
			List(const List&);
			List& operator=(const List&);

			std::vector<Element*> _elements;
	};

}

#endif
