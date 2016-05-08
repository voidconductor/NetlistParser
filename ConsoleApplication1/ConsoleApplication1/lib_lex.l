/*
Designed by Ефимов В.А [3О-411Б]
2016 год
*/

%option noyywrap
%option yylineno
%option prefix = "lib"
%x FOUND
%x PART
		

%{
	#include "lib_parse.hpp"
	#include <string.h>
	#include <stdlib.h>
	#include <iostream>
	#include "symtab.h"
%}

%%

^"primitive"		{BEGIN(FOUND); return PRIMITIVE;}
^"module"			{BEGIN(FOUND); return LIB_MODULE;}
<FOUND>[\_A-Za-z][\_A-Za-z0-9]*		{
						struct lib_ent *sp = lib_search(libtext);
						liblval.symp = sp;
						return LIB_NAME;
					}
<FOUND>"("			{BEGIN(PART);}	
<FOUND>[ \t\n]		{}
<FOUND>.			{}
<PART>[\_A-Za-z][\_A-Za-z0-9]*	{
									liblval.pins = strdup(libtext);
									return PINS;
								}
<PART>")"			{BEGIN(INITIAL);}
<PART>[ \t\n]		{}
<PART>.				{}
[ \t\n]				{}
"//".*				{}
.					{}
%%