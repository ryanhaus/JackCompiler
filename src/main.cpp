#include <cstdio>
#include <cstdlib>

#include "jack_compiler/tokenizer/tokenizer.hpp"

int main()
{
	const auto xml_document = jack_tokenizer::tokenize_file("Main.jack");

	delete xml_document;
	return 0;
}