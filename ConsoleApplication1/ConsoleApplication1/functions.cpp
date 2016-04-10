#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtab.h"

struct symbol symtab[NHASH];
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
			return sp;
		}

		if (++sp >= symtab + NHASH) sp = symtab;
	}
	//�������� ������������� �������
	fputs("symbol tbale overflow\n", stderr);
	abort();
}

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