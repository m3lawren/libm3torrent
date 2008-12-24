#ifndef _BENCODE_H_
#define _BENCODE_H_

#include <string>
#include <vector>

namespace BEncode {
	
	struct Token {
		enum TokenType { 
			INT, 
			STRING, 
			COLON, 
			D, 
			E, 
			I,
			L 
		};

		Token(const char* s, enum TokenType t) : value(s), type(t) {}
		Token(const char* s, size_t n, enum TokenType t) : value(s, n), type(t) {}
		std::string value;
		enum TokenType type;
	};

	std::vector<Token> tokenize(const char*, size_t len);

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

	Element* parse(const char*, size_t);
	Element* parse(const std::string& s) { return parse(s.c_str(), s.length()); }

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
