#include <bencode/parser.h>

#include <bencode/tokenizer.h>
#include <cassert>
#include <stack>
#include <vector>

namespace BEncode {

	struct Node {
		enum NodeType {
			/* 
			 * The order here must match the order in enum TokenType in
			 * bencode/tokenizer.h
			 */
			INT,
			STRING,
			COLON,
			D,
			E,
			I,
			L,

			/* Derivations in the CFG. */
			S,
			VALUE,
			STRVALUE,
			LIST,
			DICT
		};

		Node(enum NodeType t, std::string v = "") : type(t), value(v) {}
		enum NodeType type;
		std::string   value;
	};

	bool operator==(const Node& a, const Token& b) {
		return (uint32_t)a.type == (uint32_t)b.type && a.value == b.value;
	}

	bool operator==(const Token& a, const Node& b) {
		return b == a;
	}

	Element* parse(const char* str, size_t len) {
		/* 
		 * This is a straightforward LL(1) parser for bencoded files. For the
		 * CFG itself, check out bencode/cfg.txt. The rules referred to in the
		 * below code are the ones listed in that file.
		 */
		try {
			std::vector<Token> toks = tokenize(str, len);
			std::stack<Node> stack;

			stack.push(Node::S);

			while (toks.size() && stack.size()) {
				Node cur = stack.top();
				const Token& t = toks.front();
				stack.pop();
				if (t == cur) {
					toks.erase(toks.begin());
				} else {
					switch (cur.type) {
						case Node::S:
							switch (t.type) {
								case Token::D:
								case Token::I:
								case Token::L:
								case Token::INT:
									// rule 1
									stack.push(Node::VALUE); 
									break;
								default:
									assert(1 == 0);
							}
							break;
						case Node::VALUE:
							switch (t.type) {
								case Token::D:
									// rule 5
									stack.push(Node::E);
									stack.push(Node::DICT);
									stack.push(Node::D);
									break;
								case Token::I:
									// rule 2
									stack.push(Node::E);
									stack.push(Node::INT);
									stack.push(Node::I);
									break;
								case Token::L:
									// rule 4
									stack.push(Node::E);
									stack.push(Node::LIST);
									stack.push(Node::L);
									break;
								case Token::INT:
									// rule 3
									stack.push(Node::STRVALUE);
									break;
								default:
									assert(1 == 0);
							}
						case Node::STRVALUE:
							switch (t.type) {
								case Token::INT:
									// rule 6
									stack.push(Node::STRING);
									stack.push(Node::COLON);
									stack.push(Node::INT);
									break;
								default:
									assert(1 == 0);
							}
						case Node::LIST:
							switch (t.type) {
								case Token::D:
								case Token::I:
								case Token::L:
								case Token::INT:
									// rule 7
									stack.push(Node::LIST);
									stack.push(Node::VALUE);
									break;
								case Token::E:
								 	// rule 8
									break;
								default:
									assert(1 == 0);
							}
						case Node::DICT:
							switch (t.type) {
								case Token::E:
									// rule 10
									break;
								case Token::INT:
									// rule 9
									stack.push(Node::DICT);
									stack.push(Node::VALUE);
									stack.push(Node::STRVALUE);
									break;
								default:
									assert(1 == 0);
							}
						default:
							assert(1 == 0);
					}
				}
			}

		} catch (const TokenizerError& e) {
			// TODO
		}
		return NULL;
	}
}
