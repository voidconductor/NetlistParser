%{
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include "symtab.h"
	
	extern int yylex();
	extern void yyerror(char *s);
%}

%union {
	struct symbol *symp;
	int size_arr;
	class connections * conn;
	struct tmp_conn * conn_tmp;
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
%type <conn> args
%type <conn> conn_list
%type <conn_tmp> s_name


%start netlist
%%
netlist:	/*Стартовый символ*/
	|		dev_mod
	|		netlist dev_mod
	;

dev_mod:	MODULE NAME args ';' def_list ENDMODULE 
			{
				$2->type = module; 
			}
	;

def_list:	definition
	|		def_list definition
	;

args:		'(' arg_list ')'
	|		'(' conn_list ')'	{$$ = $2;}
	;

arg_list:	NAME
	|		arg_list ',' NAME
	;

wire_list:	NAME ',' NAME
			{
				$1->type = wire; 
				$1->size = 1;
				$3->type = wire; 
				$3->size = 1;
			}
	|		wire_list ',' NAME	
			{ 
				$3->type = wire; 
				$3->size = 1;
			}
	;
conn_list:	'.'NAME '('s_name')'
			{
				connections * t_c = new connections; 
				t_c->add($4->sym, $2->name, $4->index); 
				$$ = t_c;
			}
	|		conn_list ',' '.'NAME '('s_name')' 
			{
				connections * t_c = $1 ; 
				t_c->add($6->sym, $4->name, $6->index); 
				$$ = t_c;
			}
	;
s_name:		NAME	
			{
				tmp_conn * t_c = new struct tmp_conn; 
				t_c->sym = $1; 
				$$ = t_c;
			}
	|		NAME '[' SIZE_A ']' 
			{	tmp_conn * t_c = new struct tmp_conn; 
				t_c->sym = $1; 
				t_c->index = $3; 
				$$ = t_c;
			}
	;
range:			{ $$ = 1;}
	|			'[' SIZE_A ':' SIZE_A ']'	
				{
					if($2>$4)
						$$ = $2;
					else
						$$ = $4;
				}
	;
definition:		INPUT range NAME ';' 
				{ 
					$3->type = input; 
					$3->size = $2;
				}
	|			OUTPUT range NAME ';' 
				{ 
					$3->type = output; 
					$3->size = $2;
				}
	|			WIRE wire_list ';'
	|			WIRE range NAME ';'	
				{
					$3->type = wire; 
					$3->size = $2;
				}
	|			REG range NAME ';'	
				{ 
					$3->type = reg; 
					$3->size = $2;
				}
	|			NAME NAME args ';'	
				{
					$1->type = mod_type;
					$1->size = 0;

					$2->type = element;
					$2->el_type = $1->name;
					$2->size = 0;
					$2->c_list = $3;
					rewire($2);
				}
	;
%%

extern void yyerror(char * s)
{
	fprintf(stderr, "error: %s\n", s);
}