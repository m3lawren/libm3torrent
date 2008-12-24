#include <bencode/parser.h>

#include <bencode/tokenizer.h>
#include <vector>

namespace BEncode {

	Element* parse(const char* str, size_t len) {
		std::vector<Token> toks = tokenize(str, len);
		return NULL;
	}
}
