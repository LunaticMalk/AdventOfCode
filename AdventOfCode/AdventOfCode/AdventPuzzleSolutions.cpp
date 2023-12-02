#include "AdventPuzzleSolutions.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include <assert.h>

#define ARRAY_ELEMENT_COUNT(arr) (sizeof(arr)/sizeof(arr[0]))

const char* DATA_DIRECTORY = "../Data/";

/***************************************************************
*  Day 1
****************************************************************/
namespace AdventDayOne
{
	static const char* s_numberWords[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	static const int32_t s_numberWordsSize = ARRAY_ELEMENT_COUNT(s_numberWords);

	struct IndexValuePair
	{
		int index;
		int value;
	};

	void FindAllNumberWords(const std::string& inputString, std::vector<IndexValuePair>& outputVector)
	{
		for (int i = 0; i < s_numberWordsSize; i++)
		{
			//find ALL references to the word, not just the first
			//start at -1 so the first iteration starts at "0" with the find, but future iterations
			//will start at the index after the last one (could do +size or something but eh)
			int index = -1;
			do
			{
				index = inputString.find(s_numberWords[i], index + 1);
				if (index != std::string::npos)
				{
					IndexValuePair newPair = { index, i + 1 };
					outputVector.push_back(newPair);
				}
			} while (index != std::string::npos);
		}

	}

	void FindAllNumbers(const std::string& inputString, std::vector<IndexValuePair>& outputVector)
	{
		for (unsigned int i = 0; i < inputString.length(); i++)
		{
			char character = inputString[i];
			if (isdigit(character))
			{
				IndexValuePair newPair = { static_cast<int>(i), atoi(&character) };
				outputVector.push_back(newPair);
			}
		}
	}

	void UnitTestFindAllNumberWords()
	{
		std::vector<IndexValuePair> testVector;
		std::string testString = "xtwone3four";

		FindAllNumbers(testString, testVector);
		FindAllNumberWords(testString, testVector);

		std::sort(testVector.begin(), testVector.end(),
			[](const IndexValuePair& a, const IndexValuePair& b)
			{
				return a.index < b.index;
			});

		assert(testVector.size() == 4);
		assert(testVector[0].value == 2);
		assert(testVector[1].value == 1);
		assert(testVector[2].value == 3);
		assert(testVector[3].value == 4);

		testVector.clear();
		std::string testString2 = "eightwothree";

		FindAllNumbers(testString2, testVector);
		FindAllNumberWords(testString2, testVector);

		std::sort(testVector.begin(), testVector.end(),
			[](const IndexValuePair& a, const IndexValuePair& b)
			{
				return a.index < b.index;
			});

		assert(testVector.size() == 3);
		assert(testVector[0].value == 8);
		assert(testVector[1].value == 2);
		assert(testVector[2].value == 3);

		testVector.clear();
		std::string testString3 = "eighttwoeight";

		FindAllNumbers(testString3, testVector);
		FindAllNumberWords(testString3, testVector);

		std::sort(testVector.begin(), testVector.end(),
			[](const IndexValuePair& a, const IndexValuePair& b)
			{
				return a.index < b.index;
			});

		assert(testVector.size() == 3);
		assert(testVector[0].value == 8);
		assert(testVector[1].value == 2);
		assert(testVector[2].value == 8);
	}

	void VerifyValue(int lineCount, int combinedValue)
	{
		switch (lineCount)
		{
		case 1:
			assert(combinedValue == 99);
			break;
		case 20:
			assert(combinedValue == 44);
			break;
		case 60:
			assert(combinedValue == 19);
			break;
		case 100:
			assert(combinedValue == 46);
			break;
		case 150:
			assert(combinedValue == 84);
			break;
		case 196:
			assert(combinedValue == 52);
			break;
		case 586:
			assert(combinedValue == 88);
			break;
		case 679:
			assert(combinedValue == 55);
			break;
		}
	}

	int AdventOfCodeDay1()
	{
		using namespace std;

		UnitTestFindAllNumberWords();

		string inputFileName = string(DATA_DIRECTORY) + string("Day1/input.txt");

		ifstream inputFile;
		inputFile.open(inputFileName);

		int authenticationSum = 0;
		int lineCount = 0;
		string line;
		assert(inputFile.is_open());
		{
			vector<IndexValuePair> numberValuesVector;

			while (getline(inputFile, line))
			{
				FindAllNumbers(line, numberValuesVector);
				FindAllNumberWords(line, numberValuesVector);

				std::sort(numberValuesVector.begin(), numberValuesVector.end(),
					[](const IndexValuePair& a, const IndexValuePair& b)
					{
						return a.index < b.index;
					});

				//line gathered, check for numbers
				int firstNumber = numberValuesVector.begin()->value;
				int lastNumber = numberValuesVector.back().value;

				int combinedValue = (firstNumber * 10) + (lastNumber);
				authenticationSum += combinedValue;

				lineCount++;
				VerifyValue(lineCount, combinedValue);

				numberValuesVector.clear();
			}
		}

		inputFile.close();

		cout << "Day 1 value: " << authenticationSum << " total lines: " << lineCount << endl;

		return authenticationSum;
	}
}
