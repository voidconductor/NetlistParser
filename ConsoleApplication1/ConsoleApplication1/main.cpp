/*
Designed by Ефимов В.А [3О-411Б]
2016 год
*/

//using namespace std;

#include "bison.hpp"
#include "lib_parse.hpp"
#include "symtab.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <ctype.h>
#include <vector>

extern void yyrestart(FILE *input_file);
extern void librestart(FILE *input_file);

//Очищает данные
int data_wipe()
{
	symtab.clear();
	return 1;
}
//Сбрасывает библиотеку
int lib_wipe()
{
	fpga_lib.clear();
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

	if (argv[1] != NULL) //Попытка считать аргументы запуска
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



	if ((argv[1] && argv[2]) != NULL) //Попытка считать аргументы запуска
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

	//Восстановление связей между элементами
	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if ((*it).second->type == element) {
			rewire((*it).second);
		}
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

	cout << "Type \"help\" for list of commands" << endl;
	//Interface main loop:
	while (1)
	{
		cout << ">";
		char com_inp[20];
		cin >> com_inp;
		//Нечувствительность к регистру
		for (int i = 0; i < 20; i++)
		{
			com_inp[i] = tolower(com_inp[i]);
		}
		if (!strcmp(com_inp, "help"))
		{
			cout << "*****************************************************************" << endl;
			cout << endl << "Command list:" << endl;
			cout << " all      - List all netlist elements." << endl;
			cout << " typelist - List all netlist element types." << endl;
			cout << " type     - List all elements of choosen type." << endl;
			cout << " info     - Show element full info." << endl;
			cout << " conn     - Show elements, connected to choosen one." << endl;
			cout << " rpl      - Replace element." << endl;
			cout << " typerpl  - Replace all elements of choosen type." << endl;
			cout << " help     - Show commands." << endl;
			cout << " exit     - Save and Quit" << endl;
		}
		else if (!strcmp(com_inp, "all")) //Вывести информацию обо всех элементах
		{
			cout << "Elements of this netlist:" << endl;
			for (auto it = symtab.begin(); it != symtab.end(); ++it)
			{
				if (((*it).second->type != def_type) && ((*it).second->type != mod_type))
				{
					cout << (*it).second->name << endl;
				}
			}
			cout << "continue..." << endl;
			getchar();
			getchar();
		}
		else if (!strcmp(com_inp, "typelist")) //Список всех типов элементов
		{
			vector<symbol *> search_res;
			search_res = search("mod_type");
			cout << "Elements types: " << endl;
			for (int i = 0; i < search_res.size(); i++)
			{
				cout << search_res[i]->name << endl;
			}
			cout << "continue..." << endl;
			getchar();
			getchar();
		}
		else if (!strcmp(com_inp, "type"))		//Список элементов типа <type>
		{
			char * search_arg = new char[80];
			vector<symbol *> search_res;
			cout << "type name: ";
			cin >> search_arg;
			search_res = search(search_arg);

			if (search_res.size() == 0)
			{
				cout << "No instances found-0x31" << endl;
			}
			else
			{
				cout << "Elements of this type: " << search_arg << endl;
				for (int i = 0; i < search_res.size(); i++)
				{
					cout << search_res[i]->name << endl;
				}
			}
			delete[] search_arg;
			cout << "continue..." << endl;
			getchar();
			getchar();
		}
		else if (!strcmp(com_inp, "info")) //Вывести подробную информацию об элементе
		{
			string search_arg = "";
			struct symbol * tmp_res;
			cout << "element name: ";

			cin >> search_arg;

			tmp_res = symtab[search_arg];
			if (tmp_res == NULL)
			{
				cout << "Element not found-0x33" << endl;
			}
			else
			{
				cout << "-------------------------------------------------------------------" << endl;
				cout << "Object:         " << tmp_res->name << endl;
				cout << "Type:           " << tmp_res->type << endl;
				cout << "Element type:   " << tmp_res->el_type << endl;
				if (tmp_res->c_list != NULL)
				{
					cout << "Connections:" << endl;
					for (int i = 0; i < (tmp_res->c_list->conn_list.size()); i++)
					{
						if (tmp_res->type == element || tmp_res->type == module)
						{
							cout << "                ." << tmp_res->c_list->pins[i] << " to " << tmp_res->c_list->conn_list[i]->name;
							if (tmp_res->c_list->subw_ind[i] >= 0)
								cout << "[" << tmp_res->c_list->subw_ind[i] << "]" << endl;
							else
								cout << endl;
						}
						else
						{
							if (tmp_res->c_list->subw_ind[i] >= 0)
								cout << "                " << tmp_res->c_list->subw_ind[i] << " ";
							else
								cout << "                ";
							cout << "to (." << tmp_res->c_list->pins[i] << ") of (" << tmp_res->c_list->conn_list[i]->name << ")" << endl;
						}
					}
				}
				cout << "Size in bits:	" << tmp_res->size << endl;
				cout << "Host module:	" << tmp_res->host_module << endl;
				cout << "Used times:		" << tmp_res->count << endl;
				cout << "First used:     line #" << tmp_res->first_used << endl;
			}
			cout << "continue..." << endl;
			getchar();
			getchar();
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