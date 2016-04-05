#pragma once
#include <vector>

using namespace std;
//���� ���������
enum nodetype {wire, reg, module, input, output, element, mod_type, def_type};
//��������� ������
struct symbol {
	vector<vector<symbol> > kek;
	char *name;
	char *host_module; //������, � ������� ������ �������
	nodetype type; //��� ����
	char *el_type; //��� ���� �������� �� ����������
	char *connections;
	int size;
	int count;
};
//����� ���������� ��������� � ������� ��������, ����� ������ ���� ������� (?), ����� ��� ����� ����� ���������.
#define NHASH 9997
//������� ���������
extern struct symbol symtab[NHASH];
//������� ������ � ������� �� ����� ( ���������� ���-�������)
struct symbol *lookup(char *);
