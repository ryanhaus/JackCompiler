#include "parsing_helpers.hpp"

std::vector<std::string> parsing_helpers::split_string_by(const std::string& in_string, const char delimiter, const bool clear_empty)
{
	/*
	 * This function will implement a method of splitting a string by replacing
	 * any instances of the delimiter character with a terminating character and
	 * appending the segment_start pointer to the vector, taking advantage of how
	 * C-style strings work.
	 */

	std::vector<std::string> segments; // will hold each segment of the string

	const auto c_string_start = const_cast<char*>(in_string.c_str()); // pointer to the start of the c-style string
	auto segment_start = c_string_start; // pointer of the start of the segment (initially is the start of the string)

	for (unsigned int i = 0; i < static_cast<unsigned int>(in_string.size()); i++) // go through each character of the string
		if (in_string.at(i) == delimiter) // if the character is equal to the delimiter
		{
			c_string_start[i] = 0; // set terminating character
			segments.emplace_back(std::string(segment_start)); // push segment to vector, taking advantage of how C-style strings work

			segment_start = c_string_start + i; // set new pointer
		}

	if (clear_empty) // if we want to clear empty entries
		segments = delete_empty_strings(segments); // use our method to do so

	return segments;
}

std::vector<std::string> parsing_helpers::delete_empty_strings(const std::vector<std::string>& in_vector)
{
	std::vector<std::string> new_vector; // a blank vector

	for (const auto& str : in_vector) // go through each entry
		if (!str.empty())
			new_vector.emplace_back(str); // add to new vector if it isn't empty

	return new_vector;
}