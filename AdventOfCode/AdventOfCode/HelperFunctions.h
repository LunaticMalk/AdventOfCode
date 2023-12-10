#pragma once

#include <string>

namespace Helpers
{
	//also sets startIndex to the end index of the number
	int FindNextInt(const std::string& line, size_t& startIndex);
	int ParseUntilToken(const std::string& line, size_t startIndex, const char delimiter, std::string& outBuffer);
}