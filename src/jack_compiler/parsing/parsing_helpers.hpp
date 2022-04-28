#pragma once
#include <vector>
#include <string>

/**
 * \brief Contains functions that are used to help parse a Jack file (i.e. string manipulators)
 */
namespace parsing_helpers
{
	/**
	 * \brief Splits a string by a delimiter character and returns a vector of each individual segment of the string
	 * \param in_string The input string
	 * \param delimiter The character to split the string by
	 * \param clear_empty Whether or not to clear empty entries in the returning vector
	 * \return Returns a vector of each segment of the string
	 */
	std::vector<std::string> split_string_by(const std::string& in_string, const char delimiter, const bool clear_empty = true);

	/**
	 * \brief Clears empty entries in a vector of strings
	 * \param in_vector Input vector of strings
	 * \return Returns the new vector of strings without empty entries
	 */
	std::vector<std::string> delete_empty_strings(const std::vector<std::string>& in_vector);

	/**
	 * \brief Trims trailing whitespace from around a string
	 * \param in_string Input string
	 * \return Returns the input string without the trailing whitespace
	 */
	std::string trim_string(const std::string& in_string);

	/**
	 * \brief Checks whether or not the string represents a number
	 * \param string The input string
	 * \return Returns whether or not the string represents a number
	 */
	bool string_is_number(const std::string& string);
}