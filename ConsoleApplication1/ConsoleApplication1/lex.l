%option noyywrap
%option yylineno

%{
	
	#include "bison.hpp"
	#include "symtab.h"
	#include <string.h>
	#include <iostream> //Для отладки
	#include <stdlib.h>
	#include <malloc.h>

	char curr_module[80];
	int mod_begin;
	int mod_end=9999;
%}

%%
"module "[\_A-Za-z][\_A-Za-z0-9]*   { 
								mod_begin = yylineno;
								char *tmp = strdup(yytext);
								for(int i = 7; i < yyleng; i++)
								{
									curr_module[i-7] = tmp[i];
								}
								yyless(7);
								return MODULE; 
							}
"endmodule"					{ return ENDMODULE; mod_end = yylineno;}
"input"						{ return INPUT;}
"output"					{ return OUTPUT;}
"wire"						{ return WIRE;}
"reg"						{ return REG;}

[\_A-Za-z][\_A-Za-z0-9]*	{
								struct symbol *sp = lookup(yytext);
								if (yylineno >= mod_begin && yylineno <= mod_end)
								{
									sp->host_module = strdup(curr_module);
								}
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