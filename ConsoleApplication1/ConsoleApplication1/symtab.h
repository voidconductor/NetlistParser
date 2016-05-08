/*
Designed by ������ �.� [3�-411�]
2016 ���
*/

#pragma once
#include <vector>

using namespace std;
//���� ���������
enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};
//��������� ������
struct symbol {
	class connections *c_list;		//����� ��������
	char *name;						//��� ��������
	char *host_module;				//������, � ������� ������ �������
	nodetype type;					//��� ����
	char *el_type;					//��� ���� �������� �� ����������
	int size;						//������ � ����� (��� ��������)
	int count;						//���������� �������������
	int first_used;					//������, �� ������� ������� ��� �������� ������ ���
};

struct tmp_conn {					//��������� ��������� ���������
	struct symbol * sym;
	int index;
};

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

struct lib_ent {
	char *name;
	char ** pin_list; //��������� 100 �� ������ ������, ������ ����� ����� �����.
};



//����� ���������� ��������� � ������� ��������, ����� ������ ���� ������� (?), ����� ��� ����� ����� ���������.
#define NHASH 9997
//������� ���������
extern struct symbol symtab[NHASH];
//����������
extern struct lib_ent fpga_lib[NHASH];
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
vector<struct symbol*> search(char *, int);