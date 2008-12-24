#ifndef _BENCODE_TOKENIZER_H_
#define _BENCODE_TOKENIZER_H_

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

	int64_t            intValue(const Token&);
	std::vector<Token> tokenize(const char*, size_t len);
}


#endif
