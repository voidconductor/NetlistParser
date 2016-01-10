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
conn_list:	'.'NAME '('s_name')'	{sprintf(tmp, "%s to %s",$2->name,$4); $$ = strdup(tmp);}
	|		conn_list ',' '.'NAME '('s_name')' {sprintf(tmp,"%s, %s to %s", $1, $4->name, $6); $$ = strdup(tmp);}
	;
s_name:		NAME	{$$ = $1->name;}
	|		NAME '[' SIZE_A ']' {sprintf(tmp, "%s[%i]",$1->name,$3); $$ = strdup(tmp);}
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
	|			WIRE range NAME ';'	{$3->type = "wire"; $3->size = $2;}
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

void printnode(struct symbol *node, FILE *result)
{         
	fprintf(result,"-------------------------------------------------------------------\n");
	fprintf(result,"Object:			%s\n", node->name);
	fprintf(result,"Type:			%s\n", node->type);
	fprintf(result,"Connections:	%s\n", node->connections);
	fprintf(result,"Size in bits:	%i\n", node->size);
	fprintf(result,"Used times:		%i\n", node->count);
}

void main(int argc, char **argv)
{
	yyparse();
	FILE *result;
	char *mode = "w";

	int total_names = 0;
	int printed_names= 0;
	int used_indexes[NHASH];
	char * to_comp;

	result = fopen("result.txt", mode);
	if(result == NULL)
	{
		printf("Cannot open a file\n");
		exit(1);
	}
	for(int i = 0; i < NHASH; i++)
	{
		if((symtab[i].name != NULL) && (symtab[i].type != "pin"))
		{
			used_indexes[total_names] = i;
			total_names++;
		}
	}
	
	fprintf(result,"MODULES:\n");
	for(int i = 0; i < total_names; i++)
	{
		if(symtab[used_indexes[i]].type == "module")
		{
			printnode(&symtab[used_indexes[i]],result);
			printed_names++;
		}
	}				
	fprintf(result,"###################################################################\n");
	fprintf(result,"WIRES:\n");
	for(int i = 0; i < total_names; i++)
	{
		if(symtab[used_indexes[i]].type == "wire")
		{
			printnode(&symtab[used_indexes[i]],result);
			printed_names++;
		}
	}
	fprintf(result,"###################################################################\n");
	fprintf(result,"INPUTS:\n");
	for(int i = 0; i < total_names; i++)
	{
		if(symtab[used_indexes[i]].type == "input")
		{
			printnode(&symtab[used_indexes[i]],result);
			printed_names++;
		}
	}
	fprintf(result,"###################################################################\n");
	fprintf(result,"OUTPUTS:\n");
	for(int i = 0; i < total_names; i++)
	{
		if(symtab[used_indexes[i]].type == "output")
		{
			printnode(&symtab[used_indexes[i]],result);
			printed_names++;
		}
	}
	fprintf(result,"###################################################################\n");
	fprintf(result,"REGISTERS:\n");
	for(int i = 0; i < total_names; i++)
	{
		if(symtab[used_indexes[i]].type == "reg")
		{
			printnode(&symtab[used_indexes[i]],result);
			printed_names++;
		}
	}
	fprintf(result,"###################################################################\n");
	fprintf(result,"MODULE TYPES:\n");
	for(int i = 0; i < total_names; i++)
	{
		if(symtab[used_indexes[i]].type == "Module type")
		{
			printnode(&symtab[used_indexes[i]],result);
			printed_names++;
		}
	}
	fprintf(result,"###################################################################\n");
	fprintf(result,"DECLARATIONS:\n");
	for(int i = 0; i < total_names; i++)
	{
		if((symtab[used_indexes[i]].type != "Module type") &&
		   (symtab[used_indexes[i]].type != "reg") &&
		   (symtab[used_indexes[i]].type != "output") &&
		   (symtab[used_indexes[i]].type != "input") &&
		   (symtab[used_indexes[i]].type != "wire") &&
		   (symtab[used_indexes[i]].type != "module"))
		{
			printnode(&symtab[used_indexes[i]],result);
			printed_names++;
		}
	}
	fprintf(result,"###################################################################\n");
	printf("Result is written to file \"result.txt\"\n");
	printf("Total names: %i, printed names: %i\n", total_names, printed_names);
	fclose(result);
}
extern void yyerror(char * s)
{
	fprintf(stderr, "error: %s\n", s);
}