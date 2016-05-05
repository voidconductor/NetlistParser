#include "bison.hpp"
#include "lib_parse.hpp"
#include "symtab.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>

using namespace std;


void main(int argc, char **argv)
{
	//Входные файлы для парсеров
	extern FILE * yyin;
	extern FILE * libin;


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
	yyparse();// Парсер нетлиста

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
	libparse();//Парсер библиотеки

	//Проверка элементов нетлиста на принадлежность библиотеке
	if (lib_check())
		cout << endl << "Netlist contains wrong elements" << endl;
	else
		cout << endl << "Netlist elements are correct!" << endl;

	//Interface main loop:
	while (1)
	{
		char com_inp[20];
		cout << endl << "Command list:" << endl;
		cout << "1. - \"all\" - List all netlist elements." << endl;
		cout << "2. - \"typelist\" - List all netlist element types." << endl;
		cout << "3. - \"type\" - List all elements of choosen type." << endl;
		cout << "4. - \"info\" - Show element full info." << endl;
		cout << "5. - \"conn\" - Show elements, connected to choosen one." << endl;
		cout << "6. - \"rpl\" - Replace element." << endl;
		cout << "7. - \"typerpl\" - Replace all elements of choosen type." << endl;
		cout << "8. - \"exit\" - Save and Quit" << endl;
		cin >> com_inp;

		for (int i = 0; i < 20; i++)
		{
			com_inp[i] = tolower(com_inp[i]);
		}

		if (!strcmp(com_inp, "all"))
		{
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "typelist"))
		{
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "type"))
		{
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "info"))
		{
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "conn"))
		{
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "rpl"))
		{
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "typerpl"))
		{
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "exit"))
		{
			cout << endl << "Press any key to finish..." << endl;
			getchar();
			getchar();
			return;
		}
		else
		{
			cout << "~~~~~Wrong command!~~~~~" << endl;
		}
	}
}