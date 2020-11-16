#include "pch.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "PhoneBookLine.h"

using namespace std;

vector<wstring> splitline(wstring line, wstring delimiter)
{
	vector<wstring> substrings;
	size_t prev = 0;
	size_t next;
	size_t delta = delimiter.length();

	while ((next = line.find(delimiter, prev)) != string::npos)
	{
		substrings.push_back(line.substr(prev, next - prev));
		prev = next + delta;
	}
	substrings.push_back(line.substr(prev));

	return substrings;
}

extern __declspec(dllexport) vector<PhoneBookLine*> loadPhonebook(wstring path)
{
	wifstream in(path);
	vector<wstring> substrings;
	wstring line;
	wstring delimiter = wstring(L" ");
	PhoneBookLine* phonebookElement;
	vector<PhoneBookLine*> phonebook;

	if (in.is_open())
	{
		while (getline(in, line))
		{
			try 
			{
				substrings = splitline(line, delimiter);
				phonebookElement = new PhoneBookLine(
					substrings[0],
					substrings[1],
					substrings[2],
					substrings[3],
					substrings[4],
					_wtoi(substrings[5].c_str()),
					_wtoi(substrings[6].c_str()),
					_wtoi(substrings[7].c_str())
				);
				phonebook.push_back(phonebookElement);
			}
			catch(...)
			{

			}
		}

		in.close();

		return phonebook;
	}

}

