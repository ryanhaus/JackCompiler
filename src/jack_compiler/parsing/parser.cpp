#include "parser.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <rapidxml/rapidxml_print.hpp>

#include "parsing_helpers.hpp"

static bool in_multiline_comment = false; // flag for whether or not we are currently in a multiline comment

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

const std::map<jack_parser::token_classification, std::string> jack_parser::classification_strings =
{
	{ token_classification::unknown,			"ERROR"				},
	{ token_classification::keyword,			"keyword"			},
	{ token_classification::symbol,				"symbol"			},
	{ token_classification::integer_constant,	"integerConstant"	},
	{ token_classification::string_constant,	"stringConstant"	},
	{ token_classification::identifier,			"identifier"		},
};

jack_parser::statement_tokens jack_parser::tokenize_string(const std::string& in_string)
{
	statement_tokens statement;

	auto
		c_string = const_cast<char*>(in_string.c_str()), // pointer for the converted c-style string for the input string
		token_string_pointer = c_string; // pointer for the current string (defaults to the beginning of the string)

	// this lambda contains a sequence of actions that will be conducted during some of the iterations in the loop
	auto loop_end_sequence = [&statement, c_string, &token_string_pointer](const unsigned int i, token_classification token_type = token_classification::unknown)
	{
		c_string[i] = 0; // set terminating character in this position
		const auto token_string = parsing_helpers::trim_string(token_string_pointer);

		if (!token_string.empty()) // if string has content
		{
			statement.tokens.emplace_back(token_string); // append string pointer to tokens vector

			// determine token type if it is unknown
			if (token_type == token_classification::unknown)
			{
				const bool
					is_reserved = std::find(std::begin(reserved_words), std::end(reserved_words), token_string) != std::end(reserved_words), // whether or not the current token string is found in the reserved words array
					is_number = parsing_helpers::string_is_number(token_string); // whether or not the string represents a number

				if (is_reserved)
					token_type = token_classification::keyword;
				else if (is_number)
					token_type = token_classification::integer_constant;
				else if (token_string[0] < '0' || token_string[0] > '9') // can only be an identifier if it doesn't start with a digit
					token_type = token_classification::identifier;
			}

			statement.classifications.emplace_back(token_type); // append token type to classifications vector
		}

		token_string_pointer = c_string + i + 1; // set new string pointer
	};

	bool end_sequence = true; // whether or not to do the end sequence after the loop

	for (unsigned int i = 0; i < static_cast<unsigned int>(in_string.size()); i++) // go through each character of the string
	{
		const bool // different logical cases
			is_comment = c_string[i] == '/' && c_string[i + 1] == '/', // whether or not the current and proceeding character forms a comment
			is_space = c_string[i] == ' ', // whether or not the character is a space or a newline
			is_token = std::find(std::begin(symbols), std::end(symbols), c_string[i]) != std::end(symbols), // whether or not the current character is found in the tokens array (and therefore a token)
			is_quotation = c_string[i] == '"'; // whether or not the character is a quotation mark

		if (c_string[i] == '*' && c_string[i + 1] == '/') // end of multiline comment
		{
			in_multiline_comment = false; // reset flag

			// skip to characters after
			i += 2;
			continue;
		}

		// filter out comments
		if (is_comment)
		{
			end_sequence = false; // reset flag
			break;
		}

		if (c_string[i] == '/' && c_string[i + 1] == '*') // if multiline comment
			in_multiline_comment = true; // set flag

		// no more comments
		if (!in_multiline_comment)
		{
			end_sequence = true;

			if (is_space)
			{
				loop_end_sequence(i); // perform the lambda function
			}
			else if (is_token)
			{
				auto current_token_copy = std::string(1, c_string[i]); // holds a copy of the current token

				loop_end_sequence(i, token_classification::unknown); // perform the lambda function

				statement.tokens.emplace_back(parsing_helpers::trim_string(current_token_copy)); // add token to vector after converting to string
				statement.classifications.emplace_back(token_classification::symbol); // is of type token
			}
			else if (is_quotation)
			{
				i++; // go to next character
				token_string_pointer = c_string + i; // set new string pointer

				while (c_string[i] != '"') // while the character is not a quotation mark
					i++; // go to the next character

				// at this point, the character is a quotation mark
				loop_end_sequence(i, token_classification::string_constant); // perform the lambda function
			}
		}
		else end_sequence = false;
	}

	if (end_sequence)
		loop_end_sequence(0);

	//statement.tokens = parsing_helpers::delete_empty_strings(statement.tokens); // delete blank members of our tokens vector
	return statement;
}

rapidxml::xml_document<>* jack_parser::generate_xml_doc(const std::vector<statement_tokens>& tokens_list)
{
	const auto doc = new rapidxml::xml_document<>();

	const auto tokens_node = doc->allocate_node(rapidxml::node_element, "tokens"); // allocate memory for tokens attribute
	doc->append_node(tokens_node); // add to doc

	for (const auto& tokens : tokens_list)
	{
		assert(tokens.tokens.size() == tokens.classifications.size()); // make sure there are an equal number of tokens and classifications
		for (unsigned int i = 0; i < tokens.tokens.size(); i++) // go through each token/classifier pair
		{
			const auto allocated_string = doc->allocate_string((" " + tokens.tokens[i] + " ").c_str()); // allocate string
			const auto new_node = doc->allocate_node(rapidxml::node_element, classification_strings.at(tokens.classifications[i]).c_str(), allocated_string); // allocate memory for appropriate element node. we also have to allocate memory for the value string b/c of how rapid xml uses pointers
			tokens_node->append_node(new_node); // append node to document
		}
	}

	return doc;
}

rapidxml::xml_document<>* jack_parser::parse_file(const char* file_name)
{
	std::vector<statement_tokens> tokens;
	std::ifstream source_file(file_name); // open file stream
	std::string current_line; // string that will contain contents of the current line

	while (std::getline(source_file, current_line)) // go through each line of the file
	{
		std::cout << "COMPILING: " << current_line << std::endl;
		tokens.emplace_back(tokenize_string(current_line));
	}

	const auto xml_document = generate_xml_doc(tokens); // generate an XML document from the tokens

	// output
	std::cout << "DONE" << std::endl << "Generated XML:" << std::endl;
	print(std::cout, *xml_document, 0);

	std::ofstream output_xml_file("out.xml");
	output_xml_file << *xml_document;

	return xml_document;
}
