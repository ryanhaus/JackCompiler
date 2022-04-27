#include <cstdio>
#include <cstdlib>

#include "jack_compiler/jack_program.hpp"
#include "jack_compiler/parsing/parser.hpp"

int main()
{
	const auto tokens = jack_parser::tokenize_string("if (x < 0) { let state = \"negative\"; }");

	for (const auto& str : tokens)
		printf("Token: %s\n", str.c_str());

	return 0;
}