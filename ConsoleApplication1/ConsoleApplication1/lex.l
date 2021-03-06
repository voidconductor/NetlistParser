/*
Designed by ������ �.� [3�-411�]
2016 ���
*/

%option noyywrap
%option yylineno

%{
	#include "bison.hpp"
	#include "symtab.h"
	#include <string.h>
	#include <stdlib.h>
%}

%%
"module"					{ return MODULE; }
"endmodule"					{ return ENDMODULE;}
"input"						{ return INPUT;}
"output"					{ return OUTPUT;}
"wire"						{ return WIRE;}
"reg"						{ return REG;}

[\_A-Za-z][\_A-Za-z0-9]*	{
								struct symbol *sp = lookup(yytext);
								if(sp->first_used == 0)
								{
									sp->first_used = yylineno;
								}
								yylval.symp = sp;
								return NAME;
							}
[0-9]+						{
								yylval.size_arr = atoi(yytext);
								return SIZE_A;
							}
[ \t\n]						{}
"//".*						{}
";"							|
.							return yytext[0];
%%