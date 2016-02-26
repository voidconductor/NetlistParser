#pragma once
struct symbol {
	char *name;
	char *host_module;
	char *type;
	char *connections;
	int size;
	int count;
};

#define NHASH 9997
extern struct symbol symtab[NHASH];

struct symbol *lookup(char *);
