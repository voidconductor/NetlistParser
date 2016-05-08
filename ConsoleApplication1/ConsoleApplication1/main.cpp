#include "bison.hpp"
#include "lib_parse.hpp"
#include "symtab.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <ctype.h>

extern void yyrestart(FILE *input_file);
extern void librestart(FILE *input_file);

using namespace std;


void main(int argc, char **argv)
{
	//Входные файлы для парсеров
	extern FILE * yyin;
	extern FILE * libin;
	int parse_res;

	if (argv[1] != NULL)
	{
		yyin = fopen(argv[1], "r+");
		if (yyin == NULL)
		{
			cout << "Cannot open source file!" << endl;
		}
	}
parse_net_again:	//warning, a wild GOTO appears
	while (yyin == NULL)
	{
		char tmp[256];
		cout << "soure path: ";
		cin >> tmp;
		yyin = fopen(tmp, "r+");
		if (yyin == NULL)
		{
			cout << "Cannot open source file!" << endl;
		}
	}
	yyrestart(yyin);	//Здесь происходит смена входного файла, парсер сбрасывает буферы
	parse_res = yyparse();// Парсер нетлиста
	if (parse_res != 0)
	{
		yyin = NULL;
		goto parse_net_again;	//HERE IT COMES
	}



	if ((argv[1] && argv[2]) != NULL)
	{
		libin = fopen(argv[2], "r+");
		if (libin == NULL)
		{
			cout << "Cannot open library file!" << endl;
		}
	}
parse_lib_again:	//warning, a wild GOTO appears
	while (libin == NULL)
	{
		char tmp[256];
		cout << "library path: ";
		cin >> tmp;
		libin = fopen(tmp, "r+");
		if (libin == NULL)
		{
			cout << "Cannot open library file!" << endl;
		}
	}
	librestart(libin);	//Здесь происходит смена входного файла, парсер сбрасывает буферы
	parse_res = libparse();//Парсер библиотеки
	if (parse_res == 1)
	{
		libin = NULL;
		goto parse_lib_again;	//HERE IT COMES
	}


	//Проверка элементов нетлиста на принадлежность библиотеке
	if (lib_check())
	{
		cout << endl << "Incorrect Netlist-0x11" << endl;
		goto parse_net_again;
	}
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
			//Вывести информацию обо всех элементах
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "typelist"))
		{
			//Список всех типов элементов
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "type"))
		{
			//Список элементов типа <type>
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "info"))
		{
			//Вывести подробную информацию об элементе
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "conn"))
		{
			//Показать связи элемента
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "rpl"))
		{
			//Замена конкретного элемента
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "typerpl"))
		{
			//Заменяем все элементы выбранного типа
			cout << "placeholder" << endl;
		}
		else if (!strcmp(com_inp, "exit"))
		{
			//Здесь запускается обратный транслятор и запись в файл
			cout << endl << "Press any key to finish..." << endl;
			getchar();
			getchar();
			return;
		}
		else
		{
			cout << "Wrong command-0x21" << endl;
		}
	}
}