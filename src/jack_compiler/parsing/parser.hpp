#pragma once
#include <string>
#include <vector>
#include <map>
#include <rapidxml/rapidxml.hpp>

/**
 * \brief Contains functions related to the parsing of Jack statements/programs
 */
namespace jack_parser
{
	/**
	 * \brief Contains all the different types of token identification
	 */
	enum class token_classification { unknown, keyword, symbol, integer_constant, string_constant, identifier };

	/**
	 * \brief Contains information about the tokens of a parsed Jack statement
	 */
	struct statement_tokens
	{
		std::vector<std::string> tokens; // each individual parsed token
		std::vector<token_classification> classifications; // the classification of each token
	};

	/**
	 * \brief Splits a Jack statement into a vector of tokens
	 * \param in_string Input Jack statement
	 * \return Returns a vector of tokens found in the string
	 */
	statement_tokens tokenize_string(const std::string& in_string);

	/**
	 * \brief Generates a Jack XML document from a vector of statement tokens
	 * \param tokens_list List of generated instances of statement_tokens
	 * \return Returns a pointer to the generated XML document. Must be deleted when done using
	 */
	rapidxml::xml_document<>* generate_xml_doc(const std::vector<statement_tokens>& tokens_list);

	/**
	 * \brief Parses a Jack file and returns the generated XML file
	 * \param file_name Name of the file to read from
	 * \return Returns a Rapid XML document
	 */
	rapidxml::xml_document<>* parse_file(const char* file_name);

	/**
	 * \brief A collection of the symbols available in the Jack language
	 */
	extern const char symbols[];

	/**
	 * \brief A collection of reserved words in the Jack language
	 */
	extern const std::string reserved_words[];

	/**
	 * \brief A map of classification strings that are used as XML element names
	 */
	extern const std::map<token_classification, std::string> classification_strings;
};