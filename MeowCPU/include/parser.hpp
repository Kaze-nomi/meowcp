#ifndef MEOW_PARSER_H
#define MEOW_PARSER_H
#include <utility>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cstdlib>


enum class TokenType {
    UNDEFINED,
    END_OF_FILE,
    INTEGER_VALUE,
    WORD,
    LABEL,
};

struct Token {
	TokenType type = TokenType::UNDEFINED;
	std::string value;
	int integer;
};

class Parser {
private:
	std::fstream in;
public:
	Parser() = default;
	~Parser() = default;

	void init(const char* file_name);

	Token getToken();
};


void Parser::init(const char* file_name) {
	in.open(file_name, std::ios::in);
}

Token Parser::getToken() {
	struct Token res;
	if (in.eof())
	{
		res.type = TokenType::END_OF_FILE;
		return res;
	}
	std::string s;
	in >> s;
	if (std::regex_match(s, std::regex("[-+]?[0-9]+"))) {
		res.type = TokenType::INTEGER_VALUE;
		res.integer = atoi(s.c_str());
		return res;
	}

	if (s[s.size() - 1] == ':')
	{
		if (std::regex_match(s.substr(0, s.size() - 1), std::regex("[a-zA-Z_][a-zA-Z0-9_]*")))
		{
			res.type = TokenType::LABEL;
			res.value = s;
			return res;
		}
		return res;
	}
	
	if (std::regex_match(s, std::regex("[a-zA-Z_][a-zA-Z0-9_]*")))
	{
		res.type = TokenType::WORD;
		res.value = s.c_str();

		return res;
	}

	return res;
}

#endif // MEOW_PARSER_H