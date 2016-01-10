%option noyywrap

%{
	#include "bison.hpp"
	#include "symtab.h"
%}

%%
"module"		{ return MODULE;}
"endmodule"		{ return ENDMODULE;}
"input"			{ return INPUT;}
"output"		{ return OUTPUT;}
"wire"			{ return WIRE;}
"reg"			{ return REG;}

[\_A-Za-z][\_A-Za-z0-9]*	{
							struct symbol *sp = lookup(yytext);
							yylval.symp = sp;
							return NAME;
						}
[0-9]+					{
							yylval.size_arr = atoi(yytext);
							return SIZE_A;
						}
[ \t\n]					{}
"//".*					{}
";"		|
.		return yytext[0];
%%