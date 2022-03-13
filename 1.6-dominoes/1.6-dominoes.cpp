﻿/*
1.6. Домино (7)
Имеется N костей игры домино. На каждой кости имеется 2 числа (каждое от 0 до 6).
Требуется написать программу, которая  будет определять максимальное число, составленное из цифр на костях цепочки,
составленной по правилам домино из имеющихся костей.
Ввод. Первая строка входного файла содержит целое число  N – количество костей (2 ≤ N ≤ 20).
Следующие N строк содержат два целых числа X и Y (0 ≤ X, Y ≤ 6), разделенные пробелом.
Вывод. В выходной файл необходимо вывести максимальное целое число, цифры которого соответствуют значению костей в цепочке.

Visual Studio 2019 (v142)
Microsoft C++ Compiler (MSVC)
Windows 10 (10.0.19041) x64

Мушкин Андрей ПС-21
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Domino
{
	pair<char, char> val;
	bool excluded;
};

vector<Domino> ReadDominoes(istream& in)
{
	short n;
	char val1, val2;
	vector<Domino> dl;

	in >> n;

	for (short i = 0; i < n; ++i)
	{
		in >> val1 >> val2;
		dl.push_back({ pair<char, char>(val1, val2), false });
	}

	return dl;
}

void AppendDominoToStr(string& str, const Domino& d)
{
	str.push_back(get<0>(d.val));
	str.push_back(get<1>(d.val));
}

void ExcludeDominoAtPos(vector<Domino>& dl, size_t pos, bool exclude)
{
	dl[pos].excluded = exclude;
}

Domino SwapDomino(const Domino& d)
{
	return { pair(get<1>(d.val), get<0>(d.val)), d.excluded };
}

string AddNextDomino(string str, const Domino& d, vector<Domino>& dl)
{
	AppendDominoToStr(str, d);

	string nextStr, maxStr = str;

	char nextDomino = get<1>(d.val);

	short i = -1;
	for (Domino d : dl)
	{
		++i;

		if (d.excluded) continue;

		if (nextDomino == get<0>(d.val))
		{
			ExcludeDominoAtPos(dl, i, true);
			if ((nextStr = AddNextDomino(str, d, dl)).length() > maxStr.length() || 
				((nextStr.length() == maxStr.length()) && nextStr.compare(maxStr) > 0))
			{
				maxStr = nextStr;
			}
			ExcludeDominoAtPos(dl, i, false);
		}
		else if (nextDomino == get<1>(d.val))
		{
			ExcludeDominoAtPos(dl, i, true);
			if ((nextStr = AddNextDomino(str, SwapDomino(d), dl)).length() > maxStr.length() ||
				((nextStr.length() == maxStr.length()) && nextStr.compare(maxStr) > 0))
			{
				maxStr = nextStr;
			}
			ExcludeDominoAtPos(dl, i, false);
		}
	}

	return maxStr;
}

string Start(vector<Domino>& dl)
{
	string resStr, curStr;
	size_t i = 0;
	for (Domino d : dl)
	{
		ExcludeDominoAtPos(dl, i, true);
		curStr = AddNextDomino("", d, dl);
		if ((curStr.length() > resStr.length()) || ((curStr.length() == resStr.length()) && curStr.compare(resStr) > 0))
		{
			resStr = curStr;
		}

		if (get<0>(d.val) != get<1>(d.val))
		{
			curStr = AddNextDomino("", SwapDomino(d), dl);
			if ((curStr.length() > resStr.length()) || (curStr.length() == resStr.length() && curStr.compare(resStr) > 0))
			{
				resStr = curStr;
			}
		}
		ExcludeDominoAtPos(dl, i, false);

		++i;
	}

	return resStr;
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	if (!in.is_open() || !out.is_open())
	{
		cout << "Failed to open files\n";
		return 1;
	}

	vector<Domino> dominoes = ReadDominoes(in);

	out << Start(dominoes) << '\n';

	return 0;
}
