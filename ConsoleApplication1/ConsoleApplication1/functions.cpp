/*
Designed by ������ �.� [3�-411�]
2016 ���
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "symtab.h"

struct symbol symtab[NHASH];

struct lib_ent fpga_lib[NHASH];

//������� ��� �����
static unsigned symhash(char *sym)
{
	unsigned int hash = 0;
	unsigned c;

	while (c = *sym++) hash = hash * 9 ^ c;

	return hash;
}
//������ ������ �� �����
struct symbol *lookup(char *sym)
{
	//��� ��� ��������� ���-�������
	struct symbol *sp = &symtab[symhash(sym) % NHASH];
	int scount = NHASH;

	while (--scount >= 0)
	{
		if (sp->name && !strcmp(sp->name, sym))
		{
			sp->count++;
			return sp;
		}
		//���� ������ �� �������� � ����� ������ �� ����������, �������� ��������� ���������
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
	//�������� ������������� �������
	fputs("symbol tbale overflow\n", stderr);
	abort();
}
//������� ������ �� ����������
struct lib_ent *lib_search(char *sym)
{
	//��� ��� ��������� ���-�������
	struct lib_ent *sp = &fpga_lib[symhash(sym) % NHASH];
	int scount = NHASH;

	while (--scount >= 0)
	{
		if (sp->name && !strcmp(sp->name, sym))
		{
			return sp;
		}
		//���� ������ �� �������� � ����� ������ �� ����������, �������� ��������� ���������
		if (!sp->name)
		{
			sp->name = strdup(sym);
			return sp;
		}

		if (++sp >= fpga_lib + NHASH) sp = fpga_lib;
	}
	//�������� ������������� �������
	fputs("symbol tbale overflow\n", stderr);
	abort();
}
//�������, �������������� ������ ��������� ���������.
void rewire(struct symbol * node)
{
	if (node->c_list == NULL)
	{
		//���� ������� �� ����� ������, �� � ��������������� ������.
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
//������� ������ �������� � ����������
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

//������� ������ ���������
//���� ������ - "module"(1) � "other"(2)
//enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};
vector<struct symbol*> search(char * name, int type)
{
	vector<struct symbol*> found;
	if (type == 1)
	{
		for (int i = 0; i < NHASH; i++)
		{
			if (symtab[i].el_type == name)
			{
				found.push_back(&symtab[i]);
			}
		}
		return found;
	}
	else if (type == 2)
	{
		//������������
		nodetype search_arg;
		if (name == "wire")
			search_arg = wire;
		else if (name == "module")
			search_arg = module;
		else if (name == "imput")
			search_arg = input;
		else if (name == "output")
			search_arg = output;
		else if (name == "mod_type")
			search_arg = mod_type;
		for (int i = 0; i < NHASH; i++)
		{
			if (symtab[i].type == search_arg)
			{
				found.push_back(&symtab[i]);
			}
		}
		return found;
	}
	else return found;
}