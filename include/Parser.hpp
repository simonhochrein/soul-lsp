#pragma once
#include "SOUL/source/modules/soul_core/soul_core.cpp"
#include <vector>

using namespace soul;

class Parser : SimpleTokeniser
{
public:
	void index(CodeLocation start) {
		while (!matches(Token::eof)) {
			if (matches(Keyword::processor)) {
				parseProcessor();
			}
			skip();
		}
	}
	void parseProcessor() {
		expect(Operator::openBrace);
	}
};