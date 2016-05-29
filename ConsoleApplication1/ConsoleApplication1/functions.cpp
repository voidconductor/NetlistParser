/*
Designed by Ефимов В.А [3О-411Б]
2016 год
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "symtab.h"
#include <map>

map <string, struct symbol*> symtab;

map <string, struct lib_ent*> fpga_lib;

struct symbol *lookup(char *sym)
{
	if (symtab.count(sym) == 0)
	{
		struct symbol * sp = new struct symbol;

		sp->name = strdup(sym);
		sp->c_list = new connections;
		sp->el_type = "none";
		sp->host_module = "none";
		sp->size = 1;
		sp->count = 1;
		sp->first_used = 0;

		symtab.insert(pair<string, struct symbol*>(sym, sp));
		return symtab.at(sym);
	}
	else
	{
		symtab[sym]->count++;
		return symtab.at(sym);
	}
}
//Функция поиска по библиотеке
struct lib_ent *lib_search(char *sym)
{
	if (fpga_lib.count(sym) == 0)
	{
		struct lib_ent * sp = new struct lib_ent;

		sp->name = strdup(sym);

		fpga_lib.insert(pair<string, struct lib_ent*>(sym, sp));
		return fpga_lib.at(sym);
	}
	else
	{
		return fpga_lib.at(sym);
	}
}
//Функция, обеспечивающая полную связность элементов.
void rewire(struct symbol * node)
{
	if (node->c_list == NULL)
	{
		//Если элемент не имеет связей, то и восстанавливать нечего.
		return;
	}
	for (int i = 0; i < node->c_list->conn_list.size(); i++)
	{
		struct symbol *tmp = node->c_list->conn_list[i];
		if (node->c_list->subw_ind[i] >= 0)
			tmp->c_list->add(node, node->c_list->pins[i], node->c_list->subw_ind[i]);
		else
			tmp->c_list->add(node, node->c_list->pins[i], -1);
	}
}
//Функция сверки нетлиста и библиотеки

int lib_check()
{
	bool flag; 
	bool flag_total = false;

	for(auto it = symtab.begin(); it != symtab.end(); ++it)
		if ((*it).second->type == mod_type)
		{
			for (auto jt = fpga_lib.begin(); jt != fpga_lib.end(); ++jt)
			{
				flag = true;
				char *tmp = (*it).second->name;
				char *tmp1 = (*jt).second->name;

				if (strcmp(tmp, tmp1) == 0)
				{
					flag = false;
					break;
				}
			}
			if (flag)
			{
				printf("Type %s from this netlist not found in library \n", (*it).second->name);
				flag_total = true;
			}
		}
	if (flag_total)
		return 1;
	else
		return 0;
}

//Функция поиска элементов
//Типы поиска - "module"(1) и "other"(2)
//enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};
vector<struct symbol*> search(char * name)
{
	vector<struct symbol*> found;
	//Дешифрование
	nodetype search_arg; 
	if (!strcmp(name,"wire"))
		search_arg = wire;
	else if (!strcmp(name, "module"))
		search_arg = module;
	else if (!strcmp(name, "input"))
		search_arg = input;
	else if (!strcmp(name, "output"))
		search_arg = output;
	else if (!strcmp(name, "mod_type"))
		search_arg = mod_type;
	else
	{
		for (auto it = symtab.begin(); it != symtab.end(); ++it)
		{
			if ((*it).second->el_type != NULL)
			{
				if (!strcmp((*it).second->el_type, name))
				{
					found.push_back((*it).second);
				}
			}
		}
		return found;
	}
	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if ((*it).second->type == search_arg)
		{
			found.push_back((*it).second);
		}
	}
	return found;
}
//Превращает перечислимый тип в сторки для вывода
char * dechipher(nodetype in)
{
	if (in == wire)
		return "wire";
	if (in == module)
		return "module";
	if (in == input)
		return "input";
	if (in == output)
		return "output";
	if (in == element)
		return "element";
	if (in == mod_type)
		return "mod_type";
	if (in == def_type)
		return "def_type";
	else
		return "error type";
}