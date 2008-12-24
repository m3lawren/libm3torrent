#include <bencode/tokenizer.h>

#include <bencode/element.h>
#include <cassert>
#include <sstream>

#define _THROW(d, ptr) { \
	std::ostringstream __s; \
	size_t pos = len - (end - ptr); \
	__s << d; \
	throw TokenizerError(__s.str(), pos); \
}

#define THROW(d) _THROW(d, str)
#define THROW_FIRST(d) _THROW(d, first)

namespace BEncode {

	int64_t intValue(const struct Token& t) {
		assert(t.type == Token::INT);
		std::istringstream s(t.value);
		std::ostringstream o;
		int64_t v;
		s >> v;
		o << v;
		assert(o.str() == t.value);
		return v;
	}

	std::vector<Token> tokenize(const char* str, size_t len) {
		std::vector<Token> toks;
		const char* const end = &str[len];
		const char* first = str;
		enum { SNONE, SNEG, SINT } state = SNONE;

		while (str < end) {
			switch (state) {
				case SNONE:
					first = str;
					switch (*str) {
						case 'i':
							toks.push_back(Token("i", Token::I));
							state = SNONE;
							break;
						case 'l':
							toks.push_back(Token("l", Token::L));
							state = SNONE;
							break;
						case 'd':
							toks.push_back(Token("d", Token::D));
							state = SNONE;
							break;
						case 'e':
							toks.push_back(Token("e", Token::E));
							state = SNONE;
							break;
						case ':':
							if (toks.back().type == Token::INT) {
								int64_t slen = intValue(toks.back());
								toks.push_back(Token(":", Token::COLON)); // in here because we check toks.back()
								if (slen >= 0 && slen < end - str) {
									toks.push_back(Token(str + 1, slen, Token::STRING));
									str += slen;
								}
							} else {
								toks.push_back(Token(":", Token::COLON));
							}
							state = SNONE;
							break;
						case '-':
							state = SNEG;
							break;
						case '0':
							toks.push_back(Token("0", Token::INT));
							break;
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							state = SINT;
							break;
						default:
							THROW("Unexpected character.");
					}
					*str++;
					break;
				case SNEG:
					switch (*str) {
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							state = SINT;
							break;
						default:
							THROW_FIRST("Invalid integer value.");
					}
					*str++;
					break;
				case SINT:
					switch (*str) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							*str++;
							break;
						default:
							toks.push_back(Token(first, str - first, Token::INT));
							state = SNONE;
							break;
					}
					break;
			}
		}
	
		switch (state) {
			case SNONE:
				break;
			case SINT:
				toks.push_back(Token(first, str - first, Token::INT));
				break;
			case SNEG:
				THROW_FIRST("Invalid integer value.");
		}

		return toks;
	}

	Element* parse(const char* str, size_t len) {
		std::vector<Token> toks = tokenize(str, len);
		return NULL;
	}
}
