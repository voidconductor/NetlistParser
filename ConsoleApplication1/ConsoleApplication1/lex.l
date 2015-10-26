%option noyywrap

%top{
	
	#pragma warning(disable: 4996)

}

%{
	#include <cstdlib>
	#include "bison.hpp"
	
	// this function will be generated using bison
	extern int yyparse();
%}

digits	[0-9]+
binopr	[-+*/]
parenthesis [()]

%%
{digits}	{ 
				yylval.dbl = atof(yytext); 
				return LITERAL_DBL;
			}

{binopr}	{ return yytext[0];  }
\n			{ return '\n'; }
.		
