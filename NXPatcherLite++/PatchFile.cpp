#include <fstream>
#include <iostream>
#include <string>

int CheckHeader(char* fileName)
{
	std::ifstream f;
	std::string mystring("WzPatch\x1A\x02");
	std::string checkString;
	int result = 0;

	checkString.resize(mystring.length());

	f.open(fileName, std::ios::binary | std::ios::in);
	f.read(&checkString[0], mystring.length());
	result = mystring == checkString;

	f.close();

	return result;	
}