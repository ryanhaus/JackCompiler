#include "jack_program.hpp"

jack_program::jack_program(const char* file_name)
{
	std::ifstream source_file(file_name); // open file stream
	std::string current_line; // string that will contain contents of the current line

	while (std::getline(source_file, current_line)) // go through each line of the file
	{
		std::cout << "COMPILING: " << current_line << std::endl;

	}
}