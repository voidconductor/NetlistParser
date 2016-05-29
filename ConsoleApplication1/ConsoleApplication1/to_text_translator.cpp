#include <fstream>
#include <string.h>
#include "symtab.h"

using namespace std;

int netlist_translator(string file_name)
{
	fstream out_file;
	out_file.open(file_name, fstream::out);

	//Вместо комментария
	for (int i = 0; i < 7; i++)
		out_file << endl;
	//Печать модуля
	int prev_position = out_file.tellp(); //Переменная для подсчета выведенных символов
	out_file << "module ";

	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if ((*it).second->type == module)
		{
			out_file << (*it).second->name << " ( ";
			int mod_colmn = (int)out_file.tellp() - prev_position;
			for (int i = 0; i < ((*it).second->c_list->conn_list.size()); i++)
			{
				if (((i % 6) == 0) && (i != 0))
				{
					out_file << endl;
					for (int j = 0; j < mod_colmn; j++)
						out_file << " ";
				}
				if (i < (*it).second->c_list->conn_list.size()-1)
					out_file << (*it).second->c_list->conn_list[i]->name << ", ";
				else
					out_file << (*it).second->c_list->conn_list[i]->name << " ) ;" << endl;
			}
		}
	}
	out_file << endl;
	//Печать входов
	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if ((*it).second->type == input)
		{
			out_file << "    " << "input ";
			if ((*it).second->size > 1)
				out_file << "[" << (*it).second->size << ":" << (*it).second->lesser_bit <<
				"]";
			out_file << (*it).second->name << " ;" << endl;
		}
	}
	//Печать выходов
	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if ((*it).second->type == output)
		{
			out_file << "    " << "output ";
			if ((*it).second->size > 1)
				out_file << "[" << (*it).second->size << ":" << (*it).second->lesser_bit <<
				"]";
			out_file << (*it).second->name <<  " ;" << endl;
		}
	}
	//Пустая строка перед проводами
	out_file << endl;
	//Вывод параллельных проводов
	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if (((*it).second->type == wire) && ((*it).second->size > 1))
		{
			out_file << "    " << "wire ";
			out_file << "[" << (*it).second->size << ":" << (*it).second->lesser_bit <<
				"]" << (*it).second->name << ";" << endl;
			total_wires_number--;
		}
	}
	//Вывод остальных проводов
	int wires_on_line = 0;
	out_file << "    " << "wire ";
	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if (((*it).second->type == wire) && ((*it).second->size <= 1))
		{	
			out_file << (*it).second->name;
			if (total_wires_number == 1)
				out_file << ";" << endl;
			else
				out_file << ", ";
			wires_on_line++;	//Парсер подсчитывает общее количество проводов
			if ((wires_on_line % 6) == 0)
				out_file << endl << "    " << "     ";
			total_wires_number--;
		}
	}
	//Три пустых строки перед элементами
	out_file << endl << endl << endl;
	//Вывод элементов
	prev_position = out_file.tellp();
	for (auto it = symtab.begin(); it != symtab.end(); ++it)
	{
		if ((*it).second->type == element)
		{
			out_file << "    ";
			if ((*it).second->is_deleted)
				out_file << "//";
			out_file << (*it).second->el_type << " ";
			out_file << (*it).second->name;

			out_file << " (";
			for (int i = 0; i < ((*it).second->c_list->conn_list.size()); i++)
			{
				out_file << "." << (*it).second->c_list->pins[i] << " ";
				out_file << "(" << (*it).second->c_list->conn_list[i]->name;
				if ((*it).second->c_list->subw_ind[i] >= 0)
					out_file << "[" << (*it).second->c_list->subw_ind[i] << "]";
				out_file << ")";
				//Проверка, если выведено много символов - переход на новую строку
				if ((int)out_file.tellp() - prev_position > 70)
				{
					out_file << endl << "        ";
					prev_position = out_file.tellp();
					if ((*it).second->is_deleted)
						out_file << "//";
				}
				if ((i + 1) < (*it).second->c_list->conn_list.size())
					out_file << ", ";
			}

			out_file << ") ;" << endl;
			prev_position = out_file.tellp();
		}
	}
	out_file << "endmodule" << endl << endl;
	out_file.close();
	return 0;
}
