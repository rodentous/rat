#include "tokeniser.h"

#include "types.h"
#include <map>


std::map<std::string, int> keywords {
	{"exit" , 10},
	{"write", 10},
	{"var"  , 1},
};

std::map<std::string, int> operators {
	{"=" , 5},
	{"==", 4},
	{">=", 4},
	{"<=", 4},
	{"+" , 3},
	{"-" , 3},
	{"*" , 2},
	{"/" , 2},
	{";" , -1},
};

bool is_number(std::string text)
{
	for (char character : text)
	{
		if (!std::isdigit(character) && !std::isalnum(character) && character != '.' && character != '_')
			return false;
	}
	return std::isdigit(text.at(0));
}

bool is_word(std::string text)
{
	for (char character : text)
	{
		if (!std::isalnum(character) && character != '_')
			return false;
	}
	return std::isalpha(text.at(0));
}

bool is_operator(std::string text)
{
	return operators.contains(text);
}

Token add_token(std::string text, int line, int column)
{
	Token token;
	token.line = line;
	token.column = column;
	if (is_word(text))
	{
		// Keyword
		if (keywords.contains(text))
		{
			token.type = Token::KEYWORD;
			token.value = text;
			token.priority = keywords[text];
		}
		// Variable
		else
		{
			token.type = Token::VARIABLE;
			token.value = text;
			token.priority = 0;
		}
	}
	// Number
	else if (is_number(text))
	{
		token.type = Token::CONSTANT;
		token.value = text;
		token.priority = 0;
	}
	// Operator
	else if (is_operator(text))
	{
		token.type = Token::OPERATOR;
		token.value = text;
		token.priority = operators[text];
	}
	return token;
}

std::vector<Token> get_tokens(std::string text)
{
	std::vector<Token> tokens;
	std::string buffer;

	int line = 0;
	int column = 0;

	for (char character : text)
	{
		if (character == '\n')
		{
			line++;
			column = 0;	
		}
		else
			column++;

		
		if (buffer.empty())
		{
			if (!std::isspace(character))
				buffer.push_back(character);
		}

		else if (is_word(buffer + character))
			buffer.push_back(character);

		else if (is_number(buffer + character))
			buffer.push_back(character);

		else if (is_operator(buffer + character))
			buffer.push_back(character);

		else
		{
			tokens.push_back(add_token(buffer, line, column - buffer.size()));
			buffer.clear();

			if (!std::isspace(character))
				buffer.push_back(character);
		}
	}
	if (!buffer.empty())
		tokens.push_back(add_token(buffer, line, column - buffer.size()));

	return tokens;
}
