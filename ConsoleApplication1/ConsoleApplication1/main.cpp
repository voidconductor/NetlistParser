#include "bison.hpp"
#include "symtab.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

void printnode(struct symbol *node, FILE *result)
{
	fprintf(result, "-------------------------------------------------------------------\n");
	fprintf(result, "Object:			%s\n", node->name);
	fprintf(result, "Type:			%d\n", node->type);
	fprintf(result, "Element type:	%s\n", node->el_type);
	fprintf(result, "Connections:	%s\n", node->connections);
	fprintf(result, "Size in bits:	%i\n", node->size);
	fprintf(result, "Host module:	%s\n", node->host_module);
	fprintf(result, "Used times:		%i\n", node->count);
}

void main(int argc, char **argv)
{
	extern FILE * yyin;
	FILE *result;
	char *mode = "w";

	int total_names = 0;
	int printed_names = 0;
	int used_indexes[NHASH];

	if (argv[1] == NULL)
	{
		char tmp[256];
		cout << "Input source name" << endl;
		cin >> tmp;
		yyin = fopen(tmp, "r+");
		if (yyin == NULL)
		{
			cout << "Cannot open source file" << endl;
			exit(1);
		}
	}
	else
	{
		yyin = fopen(argv[1], "r+");
		if (yyin == NULL)
		{
			cout << "Cannot open source file" << endl;
			exit(1);
		}
	}

	yyparse();

	result = fopen("result.txt", mode);
	if (result == NULL)
	{
		cout << "Cannot open a file" << endl;
		exit(1);
	}
	for (int i = 0; i < NHASH; i++)
	{
		if ((symtab[i].name != NULL) && (symtab[i].type != def_type))
		{
			used_indexes[total_names] = i;
			total_names++;
		}
	}

	fprintf(result, "MODULES:\n");
	for (int i = 0; i < total_names; i++)
	{
		if (symtab[used_indexes[i]].type == module)
		{
			printnode(&symtab[used_indexes[i]], result);
			printed_names++;
		}
	}
	fprintf(result, "###################################################################\n");
	fprintf(result, "WIRES:\n");
	for (int i = 0; i < total_names; i++)
	{
		if (symtab[used_indexes[i]].type == wire)
		{
			printnode(&symtab[used_indexes[i]], result);
			printed_names++;
		}
	}
	fprintf(result, "###################################################################\n");
	fprintf(result, "INPUTS:\n");
	for (int i = 0; i < total_names; i++)
	{
		if (symtab[used_indexes[i]].type == input)
		{
			printnode(&symtab[used_indexes[i]], result);
			printed_names++;
		}
	}
	fprintf(result, "###################################################################\n");
	fprintf(result, "OUTPUTS:\n");
	for (int i = 0; i < total_names; i++)
	{
		if (symtab[used_indexes[i]].type == output)
		{
			printnode(&symtab[used_indexes[i]], result);
			printed_names++;
		}
	}
	fprintf(result, "###################################################################\n");
	fprintf(result, "REGISTERS:\n");
	for (int i = 0; i < total_names; i++)
	{
		if (symtab[used_indexes[i]].type == reg)
		{
			printnode(&symtab[used_indexes[i]], result);
			printed_names++;
		}
	}
	fprintf(result, "###################################################################\n");
	fprintf(result, "MODULE TYPES:\n");
	for (int i = 0; i < total_names; i++)
	{
		if (symtab[used_indexes[i]].type == mod_type)
		{
			printnode(&symtab[used_indexes[i]], result);
			printed_names++;
		}
	}
	fprintf(result, "###################################################################\n");
	fprintf(result, "DECLARATIONS:\n");
	for (int i = 0; i < total_names; i++)
	{
		if ((symtab[used_indexes[i]].type != mod_type) &&
			(symtab[used_indexes[i]].type != reg) &&
			(symtab[used_indexes[i]].type != output) &&
			(symtab[used_indexes[i]].type != input) &&
			(symtab[used_indexes[i]].type != wire) &&
			(symtab[used_indexes[i]].type != module))
		{
			printnode(&symtab[used_indexes[i]], result);
			printed_names++;
		}
	}
	fprintf(result, "###################################################################\n");
	printf("Result is written to file \"result.txt\"\n");
	printf("Total names: %i, printed names: %i\n", total_names, printed_names);
	fclose(result);

	cout << endl << "Press any key to finish..." << endl;
	getchar();
	getchar();
}