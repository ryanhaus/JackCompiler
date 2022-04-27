#include "parser.hpp"

#include <algorithm>
#include "parsing_helpers.hpp"

const char jack_parser::symbols[] =
{
	'(',	')',																	// arithmetic grouping, and parameter/argument-lists grouping
	'[',	']',																	// array indexing
	'{',	'}',																	// program structure and statement grouping
	',',																			// variable list separator
	';',																			// statement terminator
	'=',																			// assignment and comparison operator
	'.',																			// class membership
	'+',	'-',	'*',	'/',	'&',	'|',	'~',	'<',	'>',			// operators

};

const std::string jack_parser::reserved_words[] =
{
	"class",	"constructor",	"method",	"function",								// program components
	"int",		"boolean",		"char",		"void",									// primitive types
	"var",		"static",		"field",											// variable declarations
	"let",		"do",			"if",		"else",		"while",	"return",		// statements
	"true",		"false",		"null",												// constant values
	"this",																			// object reference
};

std::vector<std::string> jack_parser::tokenize_string(const std::string& in_string)
{
	std::vector<std::string> tokens; // vector of tokens

	auto
		c_string = const_cast<char*>(in_string.c_str()), // pointer for the converted c-style string for the input string
		token_string_pointer = c_string; // pointer for the current string (defaults to the beginning of the string)

	// this lambda contains a sequence of actions that will be conducted during some of the iterations in the loop
	auto loop_end_sequence = [&tokens, c_string, &token_string_pointer](const unsigned int i)
	{
		c_string[i] = 0; // set terminating character in this position
		tokens.emplace_back(std::string(token_string_pointer)); // append string pointer to tokens vector
		token_string_pointer = c_string + i + 1; // set new string pointer
	};

	for (unsigned int i = 0; i < static_cast<unsigned int>(in_string.size()); i++) // go through each character of the string
	{
		const bool // different logical cases
			is_space = c_string[i] == ' ', // whether or not the character is a space
			is_token = std::find(std::begin(symbols), std::end(symbols), c_string[i]) != std::end(symbols), // whether or not the current character is found in the tokens array (and therefore a token)
			is_quotation = c_string[i] == '"'; // whether or not the character is a quotation mark
		
		if (is_space)
			loop_end_sequence(i); // perform the lambda function
		else if (is_token)
		{
			auto current_token_copy = std::string(1, c_string[i]); // holds a copy of the current token

			loop_end_sequence(i); // perform the lambda function
			tokens.emplace_back(current_token_copy); // add token to vector after converting to string
		}
		else if (is_quotation)
		{
			i++; // go to next character
			token_string_pointer = c_string + i; // set new string pointer

			while (c_string[i] != '"') // while the character is not a quotation mark
				i++; // go to the next character

			// at this point, the character is a quotation mark
			loop_end_sequence(i); // perform the lambda function

			// reformat to be a string
			tokens[tokens.size() - 1] = "\"" + tokens[tokens.size() - 1] + "\""; // add quotation marks around
		}
	}

	return parsing_helpers::delete_empty_strings(tokens);
}