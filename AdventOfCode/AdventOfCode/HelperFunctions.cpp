#include "HelperFunctions.h"


int Helpers::FindNextInt(const std::string& line, size_t& startIndex)
{
	size_t endIndex = 0;

	while (!isdigit(line[startIndex]))
	{
		startIndex++;
	}

	endIndex = line.find(" ", startIndex);
	std::string number_token = line.substr(startIndex, (endIndex - startIndex));
	int numberValue = atoi(number_token.c_str());
	startIndex = endIndex;

	return numberValue;
}

//returns end index
int Helpers::ParseUntilToken(const std::string& line, size_t startIndex, const char delimiter, std::string& outBuffer)
{
	size_t endIndex = 0;

	endIndex = line.find(delimiter, startIndex);
	outBuffer = line.substr(startIndex, (endIndex - startIndex));

	return endIndex;
}