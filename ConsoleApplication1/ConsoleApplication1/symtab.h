#pragma once

enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};

struct symbol {
	char *name;
	char *host_module;
	nodetype type; //Тип узла
	char *el_type; //Имя типа элемента из библиотеки
	char *connections;
	int size;
	int count;
};

#define NHASH 9997
extern struct symbol symtab[NHASH];

struct symbol *lookup(char *);
