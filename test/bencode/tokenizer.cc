#include <bencode/tokenizer.h>
#include <test/XTest.h>

typedef std::vector<BEncode::Token> Tokens;

#define CHECK_INT(w, v) { \
	std::ostringstream __s; \
	__s << v; \
	XTS_ASSERT_EQUALS(BEncode::Token::INT, (w).type); \
	XTS_ASSERT_EQUALS(__s.str(), (w).value); \
	XTS_ASSERT_EQUALS(v, BEncode::intValue(w)); \
}

XTS_TEST(BEncodeTokenizer, emptyString) {
	Tokens t = BEncode::tokenize("", 0);
	XTS_ASSERT_EQUALS(0, t.size());
}

XTS_TEST(BEncodeTokenizer, singleInts) {
	Tokens t = BEncode::tokenize("1", 1);
	XTS_ASSERT_EQUALS(1, t.size());
	CHECK_INT(t[0], 1);

	t = BEncode::tokenize("0", 1);
	XTS_ASSERT_EQUALS(1, t.size());
	CHECK_INT(t[0], 0);

	// If we don't do this, the compiler complains about integer overflow
	int64_t v = 999 * 1000;
	v *= 1000000;
	v += 888777666;
	t = BEncode::tokenize("999888777666", 12);
	XTS_ASSERT_EQUALS(1, t.size());
	CHECK_INT(t[0], v);

	t = BEncode::tokenize("-5", 2);
	XTS_ASSERT_EQUALS(1, t.size());
	CHECK_INT(t[0], -5);
}
