#include "symtab.h"
#include <iostream>
#include <string>
#include <stdlib.h>

void replace_one(struct symbol * initial_element)
{
	cout << "You want to replace element " << initial_element->name;
	cout << " of type" << initial_element->el_type << endl;
	cout << "With another element of type :";

	string new_type = "";
	cin >> new_type;

	struct lib_ent * target_type;

	target_type = fpga_lib[new_type];
	if (target_type == NULL)
	{
		cout << "Type " << new_type << " doesn't exist in a library!" << endl;
		return;
	}
	
	cout << "Do you want to keep old name?" << endl;
	char Y_no;
	cin >> Y_no;

	Y_no = tolower(Y_no);

	if(Y_no == 'y')
	{ 
		cout << "PLACEHOLDER" << endl;
		//Дописать что делать в таком случае
	}
	else
	{
		new_type.clear(); //Используется повторно
		cout << " new element's name:";
		cin >> new_type;

		initial_element->is_deleted = true;

		struct symbol * replacer = lookup(strdup(new_type.c_str()));
		replacer->type = element;
		replacer->el_type = target_type->name;
		replacer->host_module = initial_element->host_module;
		replacer->size = 0;
		replacer->c_list = new connections;

		struct symbol * replacer_type = lookup(target_type->name);
		replacer_type->type = mod_type;
		replacer_type->size = 0;
		replacer_type->host_module = initial_element->host_module;

		for (int i = 0; i < 100; i++)
		{
			if (target_type->pin_list[i] != NULL)
				replacer->c_list->add(nullptr, target_type->pin_list[i], -1);
		}
		
		int regulator = 0;
		for (int i = 0; i < initial_element->c_list->pins.size(); i++)
		{
			cout << "Reconnect " << initial_element->c_list->conn_list[i]->name
				<< " from " << initial_element->c_list->pins[i] << " to:" << endl;
			for (int j = 0; j < replacer->c_list->pins.size(); j++)
			{
				if (replacer->c_list->conn_list[j] == NULL)
					cout << j << ". " << replacer->c_list->pins[j] << endl;
			}
			int chose_one;
			cin >> chose_one;

			if (chose_one >= regulator)
			{
				replacer->c_list->conn_list[chose_one] = initial_element->c_list->conn_list[i];
				replacer->c_list->subw_ind[chose_one] = initial_element->c_list->subw_ind[i];
			}	
		}

		int remainig = abs(((int)initial_element->c_list->pins.size() - (int)replacer->c_list->pins.size()));

		if (remainig)
		{
			cout << remainig << " pins not connected" << endl;
			cout << "Suggested to add external outputs or inputs" << endl;

			for (int i = 0; i < replacer->c_list->pins.size(); i++)
			{
				if (replacer->c_list->conn_list[i] == NULL)
				{
				inoutloop: //GOTO IS HERE
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

						temp_input->type = input;
						temp_input->size = 0;

						replacer->c_list->conn_list[i] = temp_input;

						symtab[replacer->host_module]->c_list->add(temp_input, "external", -1);

						cout << " YEAAAAAAAAAAAAAAh" << endl;
					}
					else if (Iop == 'o')
					{
						new_type.clear();
						cout << "name:" << endl;
						cin >> new_type;
						struct symbol * temp_output = lookup(strdup(new_type.c_str()));

						temp_output->type = output;
						temp_output->size = 0;

						replacer->c_list->conn_list[i] = temp_output;

						symtab[replacer->host_module]->c_list->add(temp_output, "external", -1);
					}
					else
					{
						cout << "try again" << endl;
						goto inoutloop;
					}
				}
			}
		}
		rewire(replacer);
	}
}