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

//����� ���������� ��������� � ������� ��������, ����� ������ ���� ������� (?), ����� ��� ����� ����� ���������.
#define NHASH 9997
//������� ���������
extern struct symbol symtab[NHASH];
//������� ������ � ������� �� ����� ( ���������� ���-�������)
struct symbol *lookup(char *);
