%define api.prefix {lib}

%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <iostream>
	#include "symtab.h"

	extern int liblex();
	extern void liberror(char *s);

	int lib_cnt = 0;
%}

%union	{
	struct lib_ent *symp;
	char * pins;
	char ** pin_list;
}
%token <pins> PINS;
%token <symp> LIB_NAME;

%token <syntax> LIB_MODULE;
%token <syntax> PRIMITIVE;

%type <pin_list> pinlist;

%start library
%%

library:	/*Стартовый символ*/
	|		library L_prim {lib_cnt++;}
	|		library L_mod {lib_cnt++;}
	;

L_prim:	PRIMITIVE LIB_NAME pinlist {$2->pin_list = $3;}
	;
L_mod:	LIB_MODULE LIB_NAME pinlist {$2->pin_list = $3;}
	;

pinlist:	PINS	
			{ 
				char ** tmp = new char *[100]();
				tmp[0] = $1;
				$$ = tmp;
			}
	|		pinlist PINS	
			{ 
				char ** tmp = $1;
				for(int i = 0; i<100;i++)
				{
					if(tmp[i] == NULL)
					{
						tmp[i] = $2;
						break;
					}
				}
				$$ = tmp;
			}
	;

%%

extern void liberror(char * s)
{
	fprintf(stderr, "error: %s\n", s);
}