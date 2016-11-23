#include <regex>
#include <string>

int CheckPatchFileName(char* fileName)
{
	std::string checkName(fileName);
	std::tr1::regex rx("\\d\\d\\d\\d\\dto\\d\\d\\d\\d\\d\\.patch");
	return std::tr1::regex_match(checkName.begin(), checkName.end(), rx);
}