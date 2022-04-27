#pragma once
#include <string>
#include <vector>

/**
 * \brief Contains functions related to the parsing of Jack statements/programs
 */
namespace jack_parser
{
	/**
	 * \brief Splits a Jack statement into a vector of tokens
	 * \param in_string Input Jack statement
	 * \return Returns a vector of tokens found in the string
	 */
	std::vector<std::string> tokenize_string(const std::string& in_string);

	/**
	 * \brief A collection of the symbols available in the Jack language
	 */
	extern const char symbols[];

	/**
	 * \brief A collection of reserved words in the Jack language
	 */
	extern const std::string reserved_words[];
};