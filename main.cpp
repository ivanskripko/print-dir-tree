#include <iostream>
#include <Windows.h>
#include <cstring>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

struct Directories 
{
	string fileName;
	int level; // ������� �����������
	bool directory; // ���������� ��� ����
	Directories (string name, int lev, bool dir) : fileName(name), level(lev), directory(dir) {}
};

int findDirectories(string path, vector<Directories>& v, int level);
void printDirectories(vector<Directories>& v);

int main() 
{
	setlocale(LC_ALL, "Russian");

	vector<Directories> dirs;

	string path;
	cout << "Enter the name of the derictory. For example: G:\\\\Downloads" << endl; 
	cin >> path;

	findDirectories(path, dirs, 0); 
	printDirectories(dirs);

	cin.get();
	cin.get();
	return 0;
}

int findDirectories (string path, vector<Directories>& v, int level)
{
	WIN32_FIND_DATAA findData;
	HANDLE hf;
	vector<Directories> filesV; //��������� ������, ���� ����� ���������� �����, ����� ������� ���� ����������, ����� �����

	// � ����, ������� ���� ������������ ��������� /*, �������� G://Downloads -> G://Downloads/*
	// ����� ������������ ������ ���� ������� ����������/����, ������� ������������� �����
	path += "\\*"; 
	
	hf = FindFirstFileA(path.c_str(), &findData);

	// ������� * �� ����, �.�. ����� ��� ������������ ���� ���� ��� ������ ��������� ���������� ������ ����
	path.erase(path.size() - 1, 1);	

	if (hf == INVALID_HANDLE_VALUE)	{
		cout << "Cannot find file" << endl;
		return -1;
	}

	do {
		if(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { 
			if (strcmp(findData.cFileName, ".") && strcmp(findData.cFileName, "..")) {
				v.push_back(Directories(findData.cFileName, level, true));
				findDirectories(path + findData.cFileName, v, level + 1);
			}
		} else {
			filesV.push_back(Directories(findData.cFileName, level, false));
		}					
	} while (FindNextFileA(hf, &findData)); 

	v.insert(v.end(), filesV.begin(), filesV.end());

	FindClose(hf);
	return 0;
}

void printDirectories (vector<Directories>& v) {
	cout << setfill('-');
	const int width = 5;
	for (vector<Directories>::iterator it = v.begin(); it != v.end(); ++it) {
		if (it->directory == true) {
			cout << setw( (it->level + 1) * width ) << "[DIR]  " << it->fileName << endl;
		} else {
			cout << setw( (it->level + 1) * width ) << "[FILE] " << it->fileName << endl;
		}
	}
	cout << setfill(' ');
}