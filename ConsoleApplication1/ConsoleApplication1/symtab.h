/*
Designed by Ефимов В.А [3О-411Б]
2016 год
*/

#pragma once
#include <vector>
#include <map>
#include <string>

using namespace std;
//Типы элементов - обязятально внести всё в функцию "char * dechipher"
enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};
//Структура данных
struct symbol {
	class connections *c_list;		//Связи элемента
	char *name;						//Имя элемента
	char *host_module;				//Модуль, в который входит элемент
	nodetype type = def_type;		//Тип узла
	char *el_type;					//Имя типа элемента из библиотеки
	int size;						//Размер в битах (для проводов)
	int lesser_bit = 0;				//Младший бит (иногда это не 0)
	int count;						//Количество использований
	int first_used;					//Строка, на которой элемент был встречен первый раз
};

struct tmp_conn {					//Временная служебная структура
	struct symbol * sym;
	int index;
};

class connections {					//Описание связей элемента
	public:
		vector<struct symbol*> conn_list;	//Указатели на подключенные элементы
		vector<char*> pins;					//Входы и выходы элемента
		vector<int> subw_ind;				//номер линии, если есть
		void add(struct symbol * conn, char *pin, int ind)
		{
			conn_list.push_back(conn);
			pins.push_back(pin);
			subw_ind.push_back(ind);
		}
};

struct lib_ent {
	char *name;
	char ** pin_list; //Выделяйте 100 на всякий случай, бывает очень много пинов.
};




//Таблица симоволов
extern map <string, struct symbol*> symtab;
//Библиотека
extern map <string, struct lib_ent*> fpga_lib;
//Функция поиска в таблице по имени ( использует хэш-функцию)
struct symbol *lookup(char *);
//Функция, обеспечивающая полную связность элементов.
void rewire(struct symbol *);
//Функция поиска по библиотеке
struct lib_ent *lib_search(char *);
//Счетчик количества символов в библиотеке
extern int lib_cnt;
//Функция сверки нетлиста и библиотеки
int lib_check();
//Функция поиска элементов принадлежащих типу
vector<struct symbol*> search(char *);
//Дешифратор перечислимых типов
char * dechipher(nodetype);
//Обратный транслятор
int netlist_translator(string);
//Общее количество проводов в схеме
extern int total_wires_number;