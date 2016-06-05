#include "symtab.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

void replace_one(struct symbol * initial_element)
{
	cout << "You want to replace element " << initial_element->name;
	cout << " of type" << initial_element->el_type << endl;
	cout << "With another element of type :";

	string new_type = "";
	cin >> new_type;

	struct lib_ent * target_type;
	struct symbol * replacer_type;
	struct symbol * replacer;

	target_type = fpga_lib[new_type];
	if (target_type == NULL)
	{
		cout << "Error-Type " << new_type << " doesn't exist in a library!" << endl;
		return;
	}
	else
	{
		replacer_type = lookup(target_type->name);
		replacer_type->type = mod_type;
		replacer_type->size = 0;
		replacer_type->host_module = initial_element->host_module;
	}
	
	cout << "Do you want to keep old name?" << endl;
	char Y_no;
	cin >> Y_no;

	Y_no = tolower(Y_no);

	//Сохраняем имя
	if(Y_no == 'y')
	{ 
		//Удаление изначального элемента из структуры данных
		initial_element->is_deleted = true;

		//Добавление нового с тем же именем
		replacer = new struct symbol;
		replacer->name = strdup(initial_element->name);
		replacer->type = element;
		replacer->el_type = target_type->name;
		replacer->host_module = initial_element->host_module;
		replacer->size = 1;
		replacer->c_list = new connections;
		replacer->first_used = 0;
		replacer->count = 1;

		symtab[initial_element->name] = replacer;

		string name_buffer;

		name_buffer = name_buffer + initial_element->name + "_old";

		free(initial_element->name);
		initial_element->name = strdup(name_buffer.c_str());

		symtab.insert(pair<string, struct symbol*>(initial_element->name, initial_element));
	}
	//Замена с изменением имени
	else
	{
		new_type.clear(); //Используется повторно
		bool name_flag = true;
		while (name_flag)
		{
			cout << "new element's name:";
			cin >> new_type;

			if (symtab.count(new_type) == 0)
			{
				name_flag = false;
				cout << symtab.count(new_type);
			}
			else
				cout << "Error - name is occupied!" << endl;
		}

		initial_element->is_deleted = true;

		replacer = lookup(strdup(new_type.c_str()));
		replacer->type = element;
		replacer->el_type = target_type->name;
		replacer->host_module = initial_element->host_module;
		replacer->size = 1;
	}
	//Имена пинов помещаются в соответствующую структуру
	for (int i = 0; i < 100; i++)
	{
		if (target_type->pin_list[i] != NULL)
			replacer->c_list->add(nullptr, target_type->pin_list[i], -1);
	}


	for (int i = 0; i < initial_element->c_list->pins.size(); i++)
	{
		bool pin_flag = true;
		bool conn_flag = true;
		while (conn_flag)
		{
			cout << "Reconnect " << initial_element->c_list->conn_list[i]->name
				<< " from " << initial_element->c_list->pins[i] << " to:" << endl;
			//Если не осталось пинов - завершаем переподключение
			pin_flag = false;
			for (int j = 0; j < replacer->c_list->pins.size(); j++)
			{
				if (replacer->c_list->conn_list[j] == NULL)
				{
					cout << j << ". " << replacer->c_list->pins[j] << endl;
					pin_flag = true;
				}			
			}
			if (pin_flag == false)
				break;

			int chose_one;
			cin >> chose_one;

			if (chose_one >= replacer->c_list->pins.size())
			{
				cout << "Error-Out of range" << endl;
			}
			else if (replacer->c_list->conn_list[chose_one] != NULL)
			{
				cout << "Error-This pin is already connected!" << endl;
			}
			else
			{
				replacer->c_list->conn_list[chose_one] = initial_element->c_list->conn_list[i];
				replacer->c_list->subw_ind[chose_one] = initial_element->c_list->subw_ind[i];
				conn_flag = false;
			}
		}
		if (pin_flag == false)
			break;
	}
	

	int remainig = ((int)replacer->c_list->pins.size() - (int)initial_element->c_list->pins.size());

	if (remainig > 0)
	{
		cout << remainig << " pins not connected" << endl;
		cout << "Suggested to add external outputs or inputs" << endl;

		for (int i = 0; i < replacer->c_list->pins.size(); i++)
		{
			if (replacer->c_list->conn_list[i] == NULL)
			{
				bool io_flag = true;
				while (io_flag)
				{
					cout << "Empty pin: " << replacer->c_list->pins[i] << endl;
					cout << "I/O?" << endl;
					char Iop;
					cin >> Iop;

					Iop = tolower(Iop);

					if (Iop == 'i')
					{
						new_type.clear();
						cout << "name:" << endl;
						cin >> new_type;
						struct symbol * temp_input = lookup(strdup(new_type.c_str()));

						if (temp_input->type == def_type)
						{
							temp_input->type = input;
							temp_input->size = 0;
						}

						replacer->c_list->conn_list[i] = temp_input;
						//Если это уже существующий выход или вход
						//Его не нужно добавлять в заголовок модуля
						if (temp_input->first_used == 0)
							symtab[replacer->host_module]->c_list->add(temp_input, "external", -1);
						io_flag = false;
					}
					else if (Iop == 'o')
					{
						new_type.clear();
						cout << "name:" << endl;
						cin >> new_type;
						struct symbol * temp_output = lookup(strdup(new_type.c_str()));

						if (temp_output->type == def_type)
						{
							temp_output->type = output;
							temp_output->size = 0;
						}
						replacer->c_list->conn_list[i] = temp_output;
						//Если это уже существующий выход или вход
						//Его не нужно добавлять в заголовок модуля
						if (temp_output->first_used == 0)
							symtab[replacer->host_module]->c_list->add(temp_output, "external", -1);
						io_flag = false;
					}
					else
					{
						cout << "try again" << endl;
					}
				}
			}
		}
	}
	rewire(replacer);
}


void replace_type(char * initial_type)
{
	vector<symbol *> old_list;
	vector<symbol *> new_list;
	old_list = search(initial_type);
	if (old_list.size() == 0)
	{
		cout << "Error-No elements of type " << initial_type << " found!" << endl;
		return;
	}
	cout << "You want to replace all the elements of type " << initial_type << endl;
	cout << "With elements of another type:";

	string new_type = "";
	cin >> new_type;

	struct lib_ent * target_type;
	struct symbol * replacer_type;

	target_type = fpga_lib[new_type];
	if (target_type == NULL)
	{
		cout << "Error-Type " << new_type << " doesn't exist in a library!" << endl;
		return;
	}
	else
	{
		replacer_type = lookup(target_type->name);
		replacer_type->type = mod_type;
		replacer_type->size = 0;
		replacer_type->host_module = old_list[0]->host_module;
	}
	//На выбор можно либо оставить старые имена элементов, либо назначить групповое имя
	//Имена будут отличаться суффиксом
	bool group_flag = false;
	cout << "Keep initial names? (Y/N)" << endl;
	{
		char y_n;
		cin >> y_n;
		y_n = tolower(y_n);

		if (y_n == 'n')
		{
			group_flag = true;
		}
	}
	string group_name;
	if (group_flag)
	{
		while (1)
		{
			cout << "Group name?" << endl;
			cout << ":";
			cin >> group_name;

			if (symtab.count(group_name) == 0)
				break;
			else
				cout << "Error-Name is ocupied!" << endl;
		}
	}
	//Создание новых элементов
	for (int i = 0; i < old_list.size(); i++)
	{
		if (group_flag)
		{
			old_list[i]->is_deleted = true;

			string tmp_name = group_name + "_" + to_string(i);

			new_list.push_back(lookup(strdup(tmp_name.c_str())));
			new_list[i]->type = element;
			new_list[i]->el_type = target_type->name;
			new_list[i]->host_module = old_list[i]->host_module;
			new_list[i]->size = 1;
		}
		else
		{
			old_list[i]->is_deleted = true;

			new_list.push_back(new struct symbol);
			new_list[i]->name = strdup(old_list[i]->name);
			new_list[i]->type = element;
			new_list[i]->el_type = target_type->name;
			new_list[i]->host_module = old_list[i]->host_module;
			new_list[i]->size = 1;
			new_list[i]->c_list = new connections;
			new_list[i]->first_used = 0;
			new_list[i]->count = 0;

			symtab[old_list[i]->name] = new_list[i];

			string name_buffer;

			name_buffer = name_buffer + old_list[i]->name + "_old";

			free(old_list[i]->name);
			old_list[i]->name = strdup(name_buffer.c_str());

			symtab.insert(pair<string, struct symbol*>(old_list[i]->name, old_list[i]));
		}
	}
	for (int it = 0; it < new_list.size(); it++)
	{
		for (int i = 0; i < 100; i++)
		{
			if (target_type->pin_list[i] != NULL)
				new_list[it]->c_list->add(nullptr, target_type->pin_list[i], -1);
		}
	}

	for (int i = 0; i < old_list[0]->c_list->pins.size(); i++)
	{
		bool pin_flag = true;
		bool conn_flag = true;
		while (conn_flag)
		{
			cout << "Reconnect the wires from " << old_list[0]->c_list->pins[i] << " pins to:" << endl;
			//Если не осталось пинов - завершаем переподключение
			pin_flag = false;
			for (int j = 0; j < new_list[0]->c_list->pins.size(); j++)
			{
				if (new_list[0]->c_list->conn_list[j] == NULL)
				{
					cout << j << ". " << new_list[0]->c_list->pins[j] << endl;
					pin_flag = true;
				}
			}
			if (pin_flag == false)
				break;

			int chose_one;
			cin >> chose_one;

			if (chose_one >= new_list[0]->c_list->pins.size())
			{
				cout << "Error-Out of range" << endl;
			}
			else if (new_list[0]->c_list->conn_list[chose_one] != NULL)
			{
				cout << "Error-This pin is already connected!" << endl;
			}
			else
			{
				for (int it = 0; it < new_list.size(); it++)
				{
					new_list[it]->c_list->conn_list[chose_one] = old_list[it]->c_list->conn_list[i];
					new_list[it]->c_list->subw_ind[chose_one] = old_list[it]->c_list->subw_ind[i];
				}
				conn_flag = false;
			}
		}
		if (pin_flag == false)
			break;
	}
	int remainig = ((int)new_list[0]->c_list->pins.size() - (int)old_list[0]->c_list->pins.size());

	if (remainig > 0)
	{
		cout << remainig << " pins not connected" << endl;
		cout << "Suggested to add external outputs or inputs" << endl;

		for (int i = 0; i < new_list[0]->c_list->pins.size(); i++)
		{
			if (new_list[0]->c_list->conn_list[i] == NULL)
			{
				bool io_flag = true;
				while (io_flag)
				{
					cout << "Empty pin: " << new_list[0]->c_list->pins[i] << endl;
					cout << "I/O?" << endl;
					char Iop;
					cin >> Iop;

					Iop = tolower(Iop);

					if (Iop == 'i')
					{
						new_type.clear();
						cout << "name:" << endl;
						cin >> new_type;
						struct symbol * temp_input = lookup(strdup(new_type.c_str()));

						if (temp_input->type == def_type)
						{
							temp_input->type = input;
							temp_input->size = 0;
						}

						for (int it = 0; it < new_list.size(); it++)
						{
							new_list[it]->c_list->conn_list[i] = temp_input;
						}
						//Если это уже существующий выход или вход
						//Его не нужно добавлять в заголовок модуля
						if (temp_input->first_used == 0)
							symtab[new_list[0]->host_module]->c_list->add(temp_input, "external", -1);
						io_flag = false;
					}
					else if (Iop == 'o')
					{
						new_type.clear();
						cout << "name:" << endl;
						cin >> new_type;
						struct symbol * temp_output = lookup(strdup(new_type.c_str()));

						if (temp_output->type == def_type)
						{
							temp_output->type = output;
							temp_output->size = 0;
						}

						for (int it = 0; it < new_list.size(); it++)
						{
							new_list[it]->c_list->conn_list[i] = temp_output;
						}
						//Если это уже существующий выход или вход
						//Его не нужно добавлять в заголовок модуля
						if (temp_output->first_used == 0)
							symtab[new_list[0]->host_module]->c_list->add(temp_output, "external", -1);
						io_flag = false;
					}
					else
					{
						cout << "try again" << endl;
					}
				}
			}
		}
	}
	for (int it = 0; it < new_list.size(); it++)
	{
		rewire(new_list[it]);
	}
}
