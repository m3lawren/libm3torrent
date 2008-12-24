#include <bencode/tokenizer.h>
#include <test/XTest.h>

typedef std::vector<BEncode::Token> Tokens;

#define CHECK_INT(w, v) { \
	std::ostringstream __s; \
	__s << v; \
	XTS_ASSERT_EQUALS(BEncode::Token(__s.str().c_str(), BEncode::Token::INT), w); \
	XTS_ASSERT_EQUALS(v, BEncode::intValue(w)); \
}

#define CHECK_STR(w, v) XTS_ASSERT_EQUALS(BEncode::Token(v, BEncode::Token::STRING), w);


bool operator==(const BEncode::TokenizerError& a, const BEncode::TokenizerError& b) {
	return a.description == b.description && a.pos == b.pos;
}

bool operator!=(const BEncode::TokenizerError& a, const BEncode::TokenizerError& b) {
	return !(a == b);
}

std::ostream& operator<<(std::ostream& s, const BEncode::TokenizerError& e) {
	return s << std::endl << "Description: " << e.description << std::endl << "Position:    " << e.pos << std::endl;
}

bool operator==(const BEncode::Token& a, const BEncode::Token& b) {
	return a.value == b.value && a.type == b.type;
}

bool operator!=(const BEncode::Token& a, const BEncode::Token& b) {
	return !(a == b);
}

std::ostream& operator<<(std::ostream& s, enum BEncode::Token::TokenType t) {
#define F(x) case BEncode::Token::x: return s << #x;
	switch (t) {
		F(INT);
		F(STRING);
		F(COLON);
		F(D);
		F(E);
		F(I);
		F(L);
		default:
			return s;
	}
}

std::ostream& operator<<(std::ostream& s, const BEncode::Token& t) {
	return s << std::endl << "Value: " << t.value << std::endl << "Type:  " << t.type << std::endl;
}

#define TOK_TEST(x) XTS_TEST(BEncodeTokenizer, x) { try 
#define TOK_END catch (const BEncode::TokenizerError& e) { XTS_ASSERT_DIFFERS(e, e); } }

static BEncode::Token T_D("d", BEncode::Token::D);
static BEncode::Token T_E("e", BEncode::Token::E);
static BEncode::Token T_I("i", BEncode::Token::I);
static BEncode::Token T_L("l", BEncode::Token::L);
static BEncode::Token T_COLON(":", BEncode::Token::COLON);

TOK_TEST(emptyString) {
	Tokens t = BEncode::tokenize("", 0);
	XTS_ASSERT_EQUALS(0, t.size());
} TOK_END

TOK_TEST(integer) {
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

	t = BEncode::tokenize("01", 2);
	XTS_ASSERT_EQUALS(2, t.size());
	CHECK_INT(t[0], 0);
	CHECK_INT(t[1], 1);
} TOK_END

TOK_TEST(integerInvalid) {
	XTS_ASSERT_THROWS_EQUALS(BEncode::tokenize("-", 1), BEncode::TokenizerError, BEncode::TokenizerError("Invalid integer value.", 0));
	XTS_ASSERT_THROWS_EQUALS(BEncode::tokenize("-0", 2), BEncode::TokenizerError, BEncode::TokenizerError("Invalid integer value.", 0));

} TOK_END

TOK_TEST(constants) {
	Tokens t;

	t = BEncode::tokenize("deil:", 5);
	XTS_ASSERT_EQUALS(5, t.size());
	XTS_ASSERT_EQUALS(T_D, t[0]);
	XTS_ASSERT_EQUALS(T_E, t[1]);
	XTS_ASSERT_EQUALS(T_I, t[2]);
	XTS_ASSERT_EQUALS(T_L, t[3]);
	XTS_ASSERT_EQUALS(T_COLON, t[4]);
} TOK_END

TOK_TEST(string) {
	Tokens t;

	t = BEncode::tokenize("3:foo", 5);
	XTS_ASSERT_EQUALS(3, t.size());
	CHECK_INT(t[0], 3);
	XTS_ASSERT_EQUALS(T_COLON, t[1]);
	CHECK_STR(t[2], "foo");
	
	t = BEncode::tokenize("0:", 2);
	XTS_ASSERT_EQUALS(3, t.size());
	CHECK_INT(t[0], 0);
	XTS_ASSERT_EQUALS(T_COLON, t[1]);
	CHECK_STR(t[2], "");
} TOK_END

/* TODO: is this the logical error message? */
TOK_TEST(stringOverrun) {
	Tokens t;

	XTS_ASSERT_THROWS_EQUALS(BEncode::tokenize("4:foo", 5), BEncode::TokenizerError, BEncode::TokenizerError("Unexpected character.", 2));

	t = BEncode::tokenize("4:d:", 4);
	XTS_ASSERT_EQUALS(4, t.size());
	CHECK_INT(t[0], 4);
	XTS_ASSERT_EQUALS(T_COLON, t[1]);
	XTS_ASSERT_EQUALS(T_D, t[2]);
	XTS_ASSERT_EQUALS(T_COLON, t[3]);
} TOK_END

/* TODO: should we have an error here? */
TOK_TEST(stringNegLength) {
	Tokens t;

	t = BEncode::tokenize("-1:", 3);
	XTS_ASSERT_EQUALS(2, t.size());
	CHECK_INT(t[0], -1);
	XTS_ASSERT_EQUALS(T_COLON, t[1]);
} TOK_END

TOK_TEST(initialColon) {
	Tokens t;
	t = BEncode::tokenize(":", 1);
	XTS_ASSERT_EQUALS(1, t.size());
	XTS_ASSERT_EQUALS(T_COLON, t[0]);
} TOK_END

TOK_TEST(validJunk) {
	Tokens t;
	//               1234 56   78901     23456
	std::string s = "1::33:-120:06::3deildeil:";
	t = BEncode::tokenize(s.c_str(), s.length());

	XTS_ASSERT_EQUALS(16, t.size());
	CHECK_INT(t[0], 1);
	XTS_ASSERT_EQUALS(T_COLON, t[1]);
	CHECK_STR(t[2], ":");
	CHECK_INT(t[3], 33);
	XTS_ASSERT_EQUALS(T_COLON, t[4]);
	CHECK_INT(t[5], -120);
	XTS_ASSERT_EQUALS(T_COLON, t[6]);
	CHECK_INT(t[7], 0);
	CHECK_INT(t[8], 6);
	XTS_ASSERT_EQUALS(T_COLON, t[9]);
	CHECK_STR(t[10], ":3deil");
	XTS_ASSERT_EQUALS(T_D, t[11]);
	XTS_ASSERT_EQUALS(T_E, t[12]);
	XTS_ASSERT_EQUALS(T_I, t[13]);
	XTS_ASSERT_EQUALS(T_L, t[14]);
	XTS_ASSERT_EQUALS(T_COLON, t[15]);
} TOK_END
