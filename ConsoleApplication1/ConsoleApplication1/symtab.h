#pragma once
#include <vector>

using namespace std;
//Типы элементов
enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};
//Структура данных
struct symbol {
	vector<vector<symbol> > kek;
	char *name;
	char *host_module; //Модуль, в который входит элемент
	nodetype type; //Тип узла
	char *el_type; //Имя типа элемента из библиотеки
	char *connections;
	int size;
	int count;
};
//Общее количество элементов в таблице символов, число должно быть простым (?), иначе хэш будет плохо считаться.
#define NHASH 9997
//Таблица симоволов
extern struct symbol symtab[NHASH];
//Функция поиска в таблице по имени ( использует хэш-функцию)
struct symbol *lookup(char *);
