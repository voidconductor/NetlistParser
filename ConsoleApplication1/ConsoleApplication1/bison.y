%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "symtab.h"
	
	extern int yylex();
	extern void yyerror(char *s);

	char tmp[256];
%}

%union {
	struct symbol *symp;
	int size_arr;
	char * arguments;
}
%token <symp> NAME;
%token <size_arr> SIZE_A;

%token <syntax>	MODULE;
%token <syntax> ENDMODULE;
%token <syntax> INPUT;
%token <syntax> OUTPUT;
%token <syntax> WIRE;
%token <syntax> REG;

%type <size_arr> range
%type <arguments> args
%type <arguments> conn_list
%type <arguments> s_name


%start netlist
%%
netlist:	/*Стартовый символ*/
	|		dev_mod
	|		netlist dev_mod
	;

dev_mod:	MODULE NAME args ';' def_list ENDMODULE	{
														printf("module %s found\n", $2->name);
														$2->type = "module";
													}
	;

def_list:	definition
	|		def_list definition
	;

args:		 '(' arg_list ')'
	|		 '(' conn_list ')'	{$$ = $2;}
	;

arg_list:	NAME
	|		arg_list ',' NAME
	;

wire_list:	NAME ',' NAME   {$1->type = "wire"; $1->size = 1; 
							 $3->type = "wire"; $3->size = 1;}
	|		wire_list ',' NAME	{ $3->type = "wire"; $3->size = 1;}
	;
conn_list:	'.'NAME '('s_name')'	{sprintf(tmp,"%s to %s", $2->name,$4);$$ = tmp;}
	|		conn_list ',' '.'NAME '('s_name')' {sprintf(tmp,"%s, %s to %s", $1, $4->name, $6); $$ = tmp;}
	;
s_name:		NAME	{$$ = $1->name;}
	|		NAME '[' SIZE_A ']' {sprintf(tmp, "%s[%i]",$1->name,$3); $$ = tmp;}
	;
range:			{ $$ = 1;}
	|			'[' SIZE_A ':' SIZE_A ']'	{
												if($2>$4)
													$$ = $2;
												else
													$$ = $4;
											}
	;
definition:		INPUT range NAME ';' { $3->type = "input"; $3->size = $2;}
	|			OUTPUT range NAME ';' { $3->type = "output"; $3->size = $2;}
	|			WIRE wire_list ';'
	|			WIRE range NAME ';'	{$3->type = "output"; $3->size = $2;}
	|			REG range NAME ';'	{ $3->type = "reg"; $3->size = $2;}
	|			NAME NAME args ';'	{
										$1->type = "Module type";
										$1->size = 0;

										$2->type = $1->name;
										$2->size = 0;
										$2->connections = $3;
									}
	;
%%

void main(int argc, char **argv)
{
	yyparse();
	for(int i = 0; i < NHASH; i++)
	{
		if(symtab[i].name != NULL)
		{
			printf("Object: %s\n", symtab[i].name);
			printf("Type: %s\n", symtab[i].type);
			printf("Connections: %s\n", symtab[i].connections);
			printf("Size in bits: %i\n", symtab[i].size);
			printf("Used times: %i\n", symtab[i].count);
			printf("=======================================\n");
		}
	}
}
extern void yyerror(char * s)
{
	fprintf(stderr, "error: %s\n", s);
}