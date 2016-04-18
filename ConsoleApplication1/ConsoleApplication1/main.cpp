#include "bison.hpp"
#include "lib_parse.hpp"
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
	if (node->c_list != NULL)
	{
		fprintf(result, "Connections:\n");
		for (int i = 0; i < (node->c_list->conn_list.size()); i++)
		{
			if (node->type == element)
			{
				fprintf(result, "                .%s to %s", node->c_list->pins[i], node->c_list->conn_list[i]->name);
				if (node->c_list->subw_ind[i] >= 0)
					fprintf(result, "[%i]\n", node->c_list->subw_ind[i]);
				else
					fprintf(result, "\n");
			}
			else
			{
				if (node->c_list->subw_ind[i] >= 0)
					fprintf(result, "                [%i] ", node->c_list->subw_ind[i]);
				else
					fprintf(result,"                ");
				fprintf(result, "to (.%s) of (%s)\n", node->c_list->pins[i], node->c_list->conn_list[i]->name);
			}
		}
	}
	fprintf(result, "Size in bits:	%i\n", node->size);
	fprintf(result, "Host module:	%s\n", node->host_module);
	fprintf(result, "Used times:		%i\n", node->count);
	fprintf(result, "First used:     line #%i\n", node->first_used);
}

void main(int argc, char **argv)
{
	extern FILE * yyin;
	extern FILE * libin;
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

	yyparse();// Ïàðñåð íåòëèñòà
	if ((argv[1] && argv[2]) == NULL)
	{
		char tmp[256];
		cout << "Input library name" << endl;
		cin >> tmp;
		libin = fopen(tmp, "r+");
		if (libin == NULL)
		{
			cout << "Cannot open library file" << endl;
			exit(1);
		}
	}
	else
	{
		libin = fopen(argv[2], "r+");
		if (libin == NULL)
		{
			cout << "Cannot open library file" << endl;
			exit(1);
		}
	}
	libparse();//Ïàðñåð áèáëèîòåêè
	//====================================================================
	//ÎÒËÀÄÊÀ ÁÈÁËÈÎÒÅÊÈ
	cout << "Number of libriry items " << lib_cnt << endl;

	
	for (int i = 0; i < NHASH; i++)
	{
		if (fpga_lib[i].name != NULL)
		{
			cout << fpga_lib[i].name << " (";
			
			for (int j = 0; j < 100; j++)
			{
				if (fpga_lib[i].pin_list[j] == NULL)
					break;
				else
					cout << fpga_lib[i].pin_list[j] << " ";
			}
			
			cout << ")" << endl << endl;
		}
	}

	//===============================================================
	//ÎÒËÀÄÊÀ ÁÈÁËÈÎÒÅÊÈ

	//ÎÒËÀÄÊÀ ÍÅÒËÈÑÒÀ
	//===============================================================
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