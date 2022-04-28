#include <cstdio>
#include <cstdlib>

#include "jack_compiler/parsing/parser.hpp"

int main()
{
	const auto xml_document = jack_parser::parse_file("Main.jack");

	delete xml_document;
	return 0;
}