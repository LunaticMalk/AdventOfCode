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

	int AdventOfCodeDay1()
	{
		using namespace std;

		UnitTestFindAllNumberWords();

		string inputFileName = string(DATA_DIRECTORY) + string("Day1/input.txt");

		ifstream inputFile;
		inputFile.open(inputFileName);

		int authenticationSum = 0;
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

				numberValuesVector.clear();
			}
		}

		inputFile.close();

		cout << "Day 1 value: " << authenticationSum << endl;

		return authenticationSum;
	}

} //namespace AdventDayOne

namespace AdventDayTwo
{
	const int s_maxRedCubes = 12;
	const int s_maxGreenCubes = 13;
	const int s_maxBlueCubes = 14;

	const char* s_redName = "red";
	const char* s_blueName = "blue";
	const char* s_greenName = "green";

	struct GameStats
	{
		int gameNumber;
		int maxBlue = 0;
		int maxGreen = 0;
		int maxRed = 0;
	};

	void ParseGameLine(std::string& line, GameStats& outStats)
	{
		//Example: Game 1: 12 blue, 15 red, 2 green; 17 red, 8 green, 5 blue; 8 red, 17 blue; 9 green, 1 blue, 4 red
		
		//Get the Game
		size_t token_end_pos = line.find(":");
		std::string token = line.substr(0, token_end_pos); //start, length

		size_t pos = line.find(" ") + 1;
		std::string gameToken = line.substr(pos, (token_end_pos - pos));

		outStats.gameNumber = atoi(gameToken.c_str());

		pos = token_end_pos + 1;

		while (token_end_pos != std::string::npos)
		{
			token_end_pos = line.find(";", pos); //possibe bug: how will this handle the string end?
			token = line.substr(pos, (token_end_pos - pos));

			//now I have the dice counts in "token" (12 blue, 15 red, 2 green)
			size_t inner_end_pos;
			size_t inner_start_pos = 1; //+1 cause they put spaces after commas
			do
			{
				//find a number, find a color
				inner_end_pos = token.find(" ", inner_start_pos);
				std::string number_token = token.substr(inner_start_pos, (inner_end_pos - inner_start_pos));

				int diceNumber = atoi(number_token.c_str());

				inner_start_pos = inner_end_pos + 1;
				inner_end_pos = token.find(",", inner_start_pos);
				if (inner_end_pos == std::string::npos)
				{
					inner_end_pos = token.length();
				}

				std::string color_token = token.substr(inner_start_pos, (inner_end_pos - inner_start_pos));
				if (strcmp(color_token.c_str(), s_redName) == 0)
				{
					if (diceNumber > outStats.maxRed)
					{
						outStats.maxRed = diceNumber;
					}
				}
				else if (strcmp(color_token.c_str(), s_greenName) == 0)
				{
					if (diceNumber > outStats.maxGreen)
					{
						outStats.maxGreen = diceNumber;
					}
				}
				else if (strcmp(color_token.c_str(), s_blueName) == 0)
				{
					if (diceNumber > outStats.maxBlue)
					{
						outStats.maxBlue = diceNumber;
					}
				}

				inner_start_pos = inner_end_pos + 2; //comma, space
 
			} while (inner_end_pos != std::string::npos && inner_end_pos != token.length());
			
			pos = token_end_pos + 1; //semicolon
		}
	}

	int AdventOfCodeDayTwo()
	{
		using namespace std;

		//UnitTestFindAllNumberWords();

		string inputFileName = string(DATA_DIRECTORY) + string("Day2/input.txt");

		ifstream inputFile;
		inputFile.open(inputFileName);

		int rollingSum = 0;
		int rollingPower = 0;
		string line;
		assert(inputFile.is_open());
		{
			vector<GameStats> statsVector;
			
			while (getline(inputFile, line))
			{
				GameStats gameLineStat;
				ParseGameLine(line, gameLineStat);

				statsVector.push_back(gameLineStat);
			}

			for (size_t i = 0; i < statsVector.size(); i++)
			{
				GameStats& stats = statsVector[i];

				if (stats.maxBlue <= s_maxBlueCubes && stats.maxGreen <= s_maxGreenCubes && stats.maxRed <= s_maxRedCubes)
				{
					rollingSum += stats.gameNumber;
				}

				int power = stats.maxBlue * stats.maxGreen * stats.maxRed;
				rollingPower += power;
			}
		}
		
		return rollingSum;
	}
}
