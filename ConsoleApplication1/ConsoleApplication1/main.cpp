#include "bison.hpp"
#include "lib_parse.hpp"
#include "symtab.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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
		cout << "Netlist contains wrong elements" << endl;
	else
		cout << "Netlist elements are correnct" << endl;


	cout << endl << "Press any key to finish..." << endl;
	getchar();
}