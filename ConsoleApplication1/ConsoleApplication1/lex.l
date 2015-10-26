%option noyywrap

%{
	#pragma warning(disable: 4996)
	#include <iostream>

	int chars = 0;
	int words = 0;
	int lines = 0;
%}

%% 
[a-zA-Z]+		{ words++; chars += strlen(yytext); }
\n				{ chars++; lines++; }
.

%%

int main()
{
	yylex();

	std::cout << "Characters: " << chars 
				<<", words: " << words
					<< ", lines: " << lines << std::endl;

	return 0;
}
