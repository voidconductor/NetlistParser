%define api.prefix {lib}

%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "symtab.h"

	extern int liblex();
	extern void liberror(char *s);

	int lib_cnt = 0;
%}

%union	{
	struct lib_ent *symp;
	char * pins;
}
%token <pins> PINS;
%token <symp> LIB_NAME;

%token <syntax> LIB_MODULE;
%token <syntax> PRIMITIVE;

%start library
%%

library:	/*Стартовый символ*/
	|		library L_prim {lib_cnt++;}
	|		library L_mod {lib_cnt++;}
	;

L_prim:	PRIMITIVE LIB_NAME pinlist
	;
L_mod:	LIB_MODULE LIB_NAME pinlist
	;

pinlist:	PINS
	|		pinlist PINS
	;

%%

extern void liberror(char * s)
{
	fprintf(stderr, "error: %s\n", s);
}