/*
Designed by Ефимов В.А [3О-411Б]
2016 год
*/

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

//Очищает данные
int data_wipe()
{
	for (int i = 0; i < NHASH; i++)
	{
		symtab[i].name = NULL;
		delete symtab[i].c_list;
		symtab[i].c_list = NULL;
	}
	return 1;
}

int lib_wipe()
{
	for (int i = 0; i < NHASH; i++)
	{
		fpga_lib[i].name = NULL;
		delete fpga_lib[i].pin_list;
		fpga_lib[i].pin_list = NULL;
	}
	return 1;
}


void main(int argc, char **argv)
{
	//Входные файлы для парсеров
	extern FILE * yyin;
	extern FILE * libin;
	int parse_res;

	cout << "***********************************************" << endl;
	cout << "WireCat 0.1 started" << endl;
	cout << "Hello" << endl;
	cout << "***********************************************" << endl;

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
		cout << "source path: ";
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
		data_wipe();
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
		lib_wipe();
		goto parse_lib_again;	//HERE IT COMES
	}


	//Проверка элементов нетлиста на принадлежность библиотеке
	if (lib_check())
	{
		cout << endl << "Incorrect Netlist-0x11" << endl;
		cout << "Try another source? (Y/N)" << endl;
		char Y_N;
		while (1)
		{
			cin >> Y_N;
			Y_N = tolower(Y_N);
			if (Y_N == 'y')
			{
				data_wipe();
				lib_wipe;
				yyin = NULL;
				libin = NULL;
				goto parse_net_again;
			}
			if (Y_N == 'n')
				break;
		}
	}
	else
		cout << endl << "Netlist is correct, all elements are described in library." << endl;

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