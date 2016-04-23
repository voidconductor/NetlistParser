#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtab.h"

struct symbol symtab[NHASH];

struct lib_ent fpga_lib[NHASH];

//Считает хэш имени
static unsigned symhash(char *sym)
{
	unsigned int hash = 0;
	unsigned c;

	while (c = *sym++) hash = hash * 9 ^ c;

	return hash;
}
//Фнкция поиска по имени
struct symbol *lookup(char *sym)
{
	//Вот тут вывзываем хэш-функцию
	struct symbol *sp = &symtab[symhash(sym) % NHASH];
	int scount = NHASH;

	while (--scount >= 0)
	{
		if (sp->name && !strcmp(sp->name, sym))
		{
			sp->count++;
			return sp;
		}
		//Если записи об элементе с таким именем не существует, задаются дефолтные параметры
		if (!sp->name)
		{
			sp->name = strdup(sym);
			sp->type = def_type;
			sp->el_type = "none";
			sp->host_module = "none";
			sp->size = 1;
			sp->count = 1;
			sp->first_used = 0;
			return sp;
		}

		if (++sp >= symtab + NHASH) sp = symtab;
	}
	//Проверка заполненности таблицы
	fputs("symbol tbale overflow\n", stderr);
	abort();
}
//Функция поиска по библиотеке
struct lib_ent *lib_search(char *sym)
{
	//Вот тут вывзываем хэш-функцию
	struct lib_ent *sp = &fpga_lib[symhash(sym) % NHASH];
	int scount = NHASH;

	while (--scount >= 0)
	{
		if (sp->name && !strcmp(sp->name, sym))
		{
			return sp;
		}
		//Если записи об элементе с таким именем не существует, задаются дефолтные параметры
		if (!sp->name)
		{
			sp->name = strdup(sym);
			return sp;
		}

		if (++sp >= fpga_lib + NHASH) sp = fpga_lib;
	}
	//Проверка заполненности таблицы
	fputs("symbol tbale overflow\n", stderr);
	abort();
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
		if(tmp->c_list == NULL)
			tmp->c_list = new connections;
		if (node->c_list->subw_ind[i] >= 0)
			tmp->c_list->add(node, node->c_list->pins[i], node->c_list->subw_ind[i]);
		else
			tmp->c_list->add(node, node->c_list->pins[i], -1);
	}
}
//Функция сверки нетлиста и библиотеки
int lib_check()
{
	int used_lib[NHASH];
	int used_netlist[NHASH];

	int net_names = 0;
	int lib_names = 0;

	bool flag; 
	bool flag_total = false;

	for (int i = 0; i < NHASH; i++)
	{
		if ((symtab[i].name != NULL) && (symtab[i].type == mod_type))
		{
			used_netlist[net_names] = i;
			net_names++;
		}
	}

	for (int i = 0; i < NHASH; i++)
	{
		if ((fpga_lib[i].name != NULL))
		{
			used_lib[lib_names] = i;
			lib_names++;
		}
	}

	for (int i = 0; i < net_names; i++)
	{
		for (int j = 0; j < lib_names; j++)
		{
			flag = true;
			char *tmp = symtab[used_netlist[i]].name;
			char *tmp1 = fpga_lib[used_lib[j]].name;
			if (strcmp(tmp, tmp1) == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag)
		{
			printf("Type %s from this netlist not found in library \n", symtab[used_netlist[i]].name);
			flag_total = true;
		}
	}

	if (flag_total)
		return 1;
	else
		return 0;
}