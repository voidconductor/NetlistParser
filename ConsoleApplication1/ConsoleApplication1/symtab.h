/*
Designed by ������ �.� [3�-411�]
2016 ���
*/

#pragma once
#include <vector>
#include <map>
#include <string>

using namespace std;
//���� ��������� - ����������� ������ �� � ������� "char * dechipher"
enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};

class connections {					//�������� ������ ��������
public:
	vector<struct symbol*> conn_list;	//��������� �� ������������ ��������
	vector<char*> pins;					//����� � ������ ��������
	vector<int> subw_ind;				//����� �����, ���� ����
	void add(struct symbol * conn, char *pin, int ind)
	{
		conn_list.push_back(conn);
		pins.push_back(pin);
		subw_ind.push_back(ind);
	}
};

//��������� ������
struct symbol {
	class connections *c_list;		//����� ��������
	char *name;						//��� ��������
	char *host_module;				//������, � ������� ������ �������
	nodetype type = def_type;		//��� ����
	char *el_type;					//��� ���� �������� �� ����������
	int size;						//������ � ����� (��� ��������)
	int lesser_bit = 0;				//������� ��� (������ ��� �� 0)
	int count;						//���������� �������������
	int first_used;					//������, �� ������� ������� ��� �������� ������ ���
	bool is_deleted = false;		//���� ����������, ��� ������� ��� ������� �� �� ������ ������������

	~symbol()
	{
		delete c_list;
		free(name);
	}
};

struct tmp_conn {					//��������� ��������� ���������
	struct symbol * sym;
	int index;
};

struct lib_ent {
	char *name;
	char ** pin_list; //��������� 100 �� ������ ������, ������ ����� ����� �����.

	~lib_ent()
	{
		free(name);
		delete[] pin_list;
	}
};




//������� ���������
extern map <string, struct symbol*> symtab;
//����������
extern map <string, struct lib_ent*> fpga_lib;
//������� ������ � ������� �� ����� ( ���������� ���-�������)
struct symbol *lookup(char *);
//�������, �������������� ������ ��������� ���������.
void rewire(struct symbol *);
//������� ������ �� ����������
struct lib_ent *lib_search(char *);
//������� ���������� �������� � ����������
extern int lib_cnt;
//������� ������ �������� � ����������
int lib_check();
//������� ������ ��������� ������������� ����
vector<struct symbol*> search(char *);
//���������� ������������ �����
char * dechipher(nodetype);
//�������� ����������
int netlist_translator(string);
//����� ���������� �������� � �����
extern int total_wires_number;
//�������� ����� ��������
int print_connections(struct symbol *);
//������ ������ ��������
void replace_one(struct symbol *);
//������ ���� ��������� ������������ ����
void replace_type(char *);