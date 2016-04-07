#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symtab.h"

struct symbol symtab[NHASH];
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
			return sp;
		}

		if (++sp >= symtab + NHASH) sp = symtab;
	}
	//Проверка заполненности таблицы
	fputs("symbol tbale overflow\n", stderr);
	abort();
}