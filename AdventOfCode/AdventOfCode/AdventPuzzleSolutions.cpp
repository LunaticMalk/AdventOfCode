#include "AdventPuzzleSolutions.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include <assert.h>
#include <map>

#include "HelperFunctions.h"

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
				index = (int)inputString.find(s_numberWords[i], index + 1);
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

		std::cout << "Day 1 value: " << authenticationSum << endl;

		return authenticationSum;
	}

} //namespace AdventDayOne

/***************************************************************
*  Day 2
****************************************************************/
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
		int gameNumber = 0;
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

			inputFile.close();

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
		
		std::cout << "Day 2 - Part One answer: " << rollingSum << " and Part Two: " << rollingPower << endl;
		return rollingSum;
	}
}

/***************************************************************
*  Day 3
****************************************************************/
namespace AdventDayThree
{
	const char SPACE_DOT = '.';
	const char GEAR_SYMBOL = '*';
	
	bool IsSymbol(char val)
	{
		return (val != SPACE_DOT && !isdigit(val));
	}

	int MakeNumber(int index, int length, const std::string& sourceString)
	{
		int value = 0;

		for (int i = 0; i < length; i++)
		{
			const char number = sourceString[index + (length - 1 - i)];
			//start at the last number and work our ways backwards
			value += atoi(&number) * (int)((pow(10, i)));
		}

		return value;
	}

	//assumes the value passed in at startIndex is already a number
	int FindNumberWithValueAtIndex(int startIndex, std::string& line)
	{
		int valueStartIndex = startIndex;
		int length = 1;

		//find the left side
		while (valueStartIndex > 0)
		{
			if (!isdigit(line[valueStartIndex - 1]))
			{
				break;
			}
			--valueStartIndex;
			++length;
		}
		//find the right side
		while ((valueStartIndex + length - 1) < line.size())
		{
			if (!isdigit(line[valueStartIndex + length]))
			{
				break;
			}
			++length;
		}

		return MakeNumber(valueStartIndex, length, line);
	}

	int FindGearRatio(int row, int col, std::vector<std::string>& schematicData)
	{
		std::string& rowString = schematicData[row];
		int rowLength = (int)rowString.length();

		int foundNumberCount = 0;
		const int MAXNUMBERCOUNT = 2;
		int numbers[2] = { 0, 0 };		

		//check next to it
		if (col > 0 && isdigit(rowString[col - 1]))
		{
			numbers[foundNumberCount] = FindNumberWithValueAtIndex(col - 1, rowString);
			foundNumberCount++;			
		}
		if (col < rowLength - 1 && isdigit(rowString[col + 1]))
		{
			numbers[foundNumberCount] = FindNumberWithValueAtIndex(col + 1, rowString);
			foundNumberCount++;
		}

		int startIndex = col > 0 ? col - 1 : col;
		int endIndex = col < rowLength - 1 ? col + 1 : rowLength - 1;

		//at this point we might have two numbers, check before saving
		if (row > 0)
		{
			std::string& aboveRowString = schematicData[row - 1];
			bool foundDigit = false;
			for (int i = startIndex; i <= endIndex; i++)
			{				
				if (isdigit(aboveRowString[i]))
				{
					if (foundDigit) { continue;  }//we already processed this number
					
					if (foundNumberCount >= MAXNUMBERCOUNT)
					{
						return 0; //we have found too many numbers, get out
					}

					numbers[foundNumberCount] = FindNumberWithValueAtIndex(i, aboveRowString);
					foundNumberCount++;
					foundDigit = true;
				}
				else { foundDigit = false; } //reset our flag
			}
		}

		//check below
		if (row < schematicData.size() - 1)
		{
			std::string& belowRowString = schematicData[row + 1];
			bool foundDigit = false;
			for (int i = startIndex; i <= endIndex; i++)
			{
				if (isdigit(belowRowString[i]))
				{
					if (foundDigit) { continue; }//we already processed this number

					if (foundNumberCount >= MAXNUMBERCOUNT)
					{
						return 0; //we have found too many numbers, get out
					}

					numbers[foundNumberCount] = FindNumberWithValueAtIndex(i, belowRowString);
					foundNumberCount++;
					foundDigit = true;
				}
				else { foundDigit = false; } //reset our flag
			}
		}

		if (foundNumberCount == MAXNUMBERCOUNT)
		{
			return numbers[0] * numbers[1];
		}

		return 0;
	}

	bool IsPartNumber(int row, int col, int length, std::vector<std::string>& schematicData)
	{
		std::string& rowString = schematicData[row];

		//could cache this globall/statically and save a bit of time but meh
		int rowLength = (int)rowString.length();

		int startIndex = col > 0 ? col - 1 : col;
		int endIndex = col + length >= rowLength ? rowLength - 1 : col + length; //(don't have to add 1, length will extend us past by 1 (if index is 2 and length is 3, 2,3,4 are the number, we want col 5)

		//check all around the number
		//check the sides
		if (IsSymbol(rowString[startIndex]) || IsSymbol(rowString[endIndex]))
		{
			return true;
		}

		//check above
		if (row > 0)
		{			
			std::string& aboveRowString = schematicData[row - 1];
			for (int i = startIndex; i <= endIndex; i++)
			{
				if (IsSymbol(aboveRowString[i]))
				{
					return true;
				}
			}
		}
		
		//check below
		if (row < schematicData.size() - 1)
		{
			std::string& belowRowString = schematicData[row + 1];
			for (int i = startIndex; i <= endIndex; i++)
			{
				if (IsSymbol(belowRowString[i]))
				{
					return true;
				}
			}
		}

		return false;
	}	

	int AdventOfCodeDayThree()
	{
		using namespace std;

		string inputFileName = string(DATA_DIRECTORY) + string("Day3/input.txt");

		vector<string> schematicData;

		ifstream inputFile;
		inputFile.open(inputFileName);

		string line;
		int answerSum = 0;
		int answerGearRatio = 0;
		assert(inputFile.is_open());
		{
			while (getline(inputFile, line))
			{
				schematicData.push_back(line);
			}

			inputFile.close();		
		}

		//Part One
		for (int row = 0; row < schematicData.size(); row++)
		{
			const std::string& rowString = schematicData[row];
			for (int column = 0; column < rowString.length(); column++)
			{
				if (isdigit(rowString[column]))
				{
					int startIndex = column;
					int length = 1;
					while (++column < rowString.length() && isdigit(rowString[column]))
					{
						length++;
					}

					if (IsPartNumber(row, startIndex, length, schematicData))
					{
						int partNumber = MakeNumber(startIndex, length, rowString);
						answerSum += partNumber;
					}

				}
			}
		}

		//Part Two
		for (int row = 0; row < schematicData.size(); row++)
		{
			const std::string& rowString = schematicData[row];
			for (int column = 0; column < rowString.length(); column++)
			{
				if (rowString[column] == GEAR_SYMBOL)
				{
					int gearRatio = FindGearRatio(row, column, schematicData);
					answerGearRatio += gearRatio;
				}
			}
		}


		std::cout << "Day 3 - Part One answer: " << answerSum << " and Part Two: " << answerGearRatio << endl;

		return answerSum;
	}
}

/***************************************************************
*  Day 4
****************************************************************/
namespace AdventDayFour
{
	std::vector<int> scratchCardCounts;
	const int MAX_CARD_COUNT = 198; //we know let's cheat

	int GetGameCardScore(const char* line, int& outWinningNumberCount)
	{
		//promised output format! We can cheat a bit
		//Card   1: 59 65 20 66 55 92 43 23 98 70 | 99 81 56 30 88 55 57 11 90 45 53 28 33 20 84 54 24 64 74 98 36 77 61 82 69
		constexpr int HEADER_SPACE = 10;
		constexpr int WINNING_NUMBER_COUNT = 10;
		constexpr int GUESSES_COUNT = 25;
		constexpr int NUMBER_LENGTH = 2;

		int winningNumbers[WINNING_NUMBER_COUNT];
		int guesses[GUESSES_COUNT];
		outWinningNumberCount = 0;

		const char* strIter = line + HEADER_SPACE;
		char numberSubstring[3];
		memset(numberSubstring, 0, 3); //null-terminate
		for (int i = 0; i < WINNING_NUMBER_COUNT; i++)
		{
			strncpy_s(numberSubstring, strIter, NUMBER_LENGTH);			
			int value = atoi(numberSubstring);
			winningNumbers[i] = value;
			strIter += NUMBER_LENGTH + 1; //for the space
		}

		//we should be at the line here
		assert(*strIter == '|');
		strIter += 2;

		for (int i = 0; i < GUESSES_COUNT; i++)
		{
			strncpy_s(numberSubstring, strIter, NUMBER_LENGTH);
			int value = atoi(numberSubstring);
			guesses[i] = value;
			strIter += NUMBER_LENGTH + 1; //for the space
		}

		int score = 0;
		for (int i = 0; i < GUESSES_COUNT; i++)
		{
			for (int j = 0; j < WINNING_NUMBER_COUNT; j++)
			{
				if (guesses[i] == winningNumbers[j])
				{
					score == 0 ? score = 1 : score *= 2;
					outWinningNumberCount++;
				}
			}
		}

		return score;
	}

	int GetPartTwoGameCardScore(int cardIndex, const char* line)
	{
		while (scratchCardCounts.size() <= cardIndex)
		{
			scratchCardCounts.push_back(1);
		}

		int cardsToPlay = scratchCardCounts[cardIndex];

		int winningNumberCount = 0;
		//don't need the score
		GetGameCardScore(line, winningNumberCount);

		//we can't add more than the number of cards we have left
		winningNumberCount = min(MAX_CARD_COUNT - cardIndex, winningNumberCount);

		while (scratchCardCounts.size() <= cardIndex + winningNumberCount)
		{
			scratchCardCounts.push_back(1);
		}

		for (int i = 0; i < winningNumberCount; i++)
		{
			scratchCardCounts[cardIndex + i + 1] += cardsToPlay; //1 for each card "played"
		}

		return winningNumberCount * cardsToPlay;
	}

	int AdventOfCodeDayFour()
	{
		using namespace std;

		string inputFileName = string(DATA_DIRECTORY) + string("Day4/input.txt");

		ifstream inputFile;
		inputFile.open(inputFileName);

		int rollingScore = 0;
		int partTwoCardCount = MAX_CARD_COUNT; //starting with the cards we have
		int lineCount = 1; //starting at card 1
		int winningCardsBuffer = 0; //bleh
		string line;
		assert(inputFile.is_open());
		{
			while (getline(inputFile, line))
			{				
				int gameScore = GetGameCardScore(line.c_str(), winningCardsBuffer);
				rollingScore += gameScore;

				int cardsAdded = GetPartTwoGameCardScore(lineCount, line.c_str());
				partTwoCardCount += cardsAdded;

				lineCount++;
			}
		}

		std::cout << "Day 4 - Part One answer: " << rollingScore << " and Part Two: " << partTwoCardCount << endl;
		return rollingScore;
	}
}

typedef unsigned long ulong; //u short now
typedef long long llong;
/***************************************************************
*  Day 5
****************************************************************/
namespace AdventDayFive
{
	struct RangeEntry
	{
		ulong sourceStart = 0;
		ulong sourceEnd = 0;
		ulong destinationStart = 0;
		ulong destinationEnd = 0;
	};

	struct Range
	{
		ulong start = 0;
		ulong length = 0;
	};

	std::vector<ulong> seedValues;
	std::vector<Range> seedRangeValues;
	std::vector<RangeEntry> seedToSoilMap;
	std::vector<RangeEntry> soilToFertilizerMap;
	std::vector<RangeEntry> fertilizerToWaterMap;
	std::vector<RangeEntry> waterToLightMap;
	std::vector<RangeEntry> lightToTemperatureMap;
	std::vector<RangeEntry> temperatureToHumidityMap;
	std::vector<RangeEntry> humidityToLocationMap;

	void ParseSeedValues(const std::string& line)
	{
		//example line
		//seeds: 3640772818 104094365 1236480411 161072229 376099792 370219099 1590268366 273715765 3224333694 68979978 2070154278 189826014 3855332650 230434913 3033760782 82305885 837883389 177854788 2442602612 571881366
		//we can cheat and know the numbers start at 
		size_t startIndex = 7;
		size_t endIndex = 0;

		while(startIndex != std::string::npos)
		{
			Range rangeData;

			endIndex = line.find(" ", startIndex);
			std::string number_token = line.substr(startIndex, (endIndex - startIndex)); //pretty sure this works even if it it's npos it's just messy
			ulong seedValue = strtoul(number_token.c_str(), nullptr, 10);
			seedValues.push_back(seedValue);
			rangeData.start = seedValue;	
			startIndex = endIndex + 1;

			endIndex = line.find(" ", startIndex);
			number_token = line.substr(startIndex, (endIndex - startIndex));
			seedValue = strtoul(number_token.c_str(), nullptr, 10);
			seedValues.push_back(seedValue);
			rangeData.length = seedValue;
			startIndex = endIndex == std::string::npos ? endIndex : endIndex + 1;

			seedRangeValues.push_back(rangeData);
		}
	}

	void ParseMapValues(std::string& line, std::vector<RangeEntry>& mapVector)
	{
		//example line
		// destination_start source_start range
		//496269031 1203272644 52136246
		//three known numbers hooo
		size_t startIndex = 0;
		size_t endIndex = 0;

		RangeEntry entryData;
		
		endIndex = line.find(" ");
		std::string number_token = line.substr(startIndex, endIndex);
		ulong numberValue = strtoul(number_token.c_str(), nullptr, 10);
		entryData.destinationStart = numberValue;
		startIndex = endIndex + 1;

		endIndex = line.find(" ", startIndex);
		number_token = line.substr(startIndex, (endIndex - startIndex));
		numberValue = strtoul(number_token.c_str(), nullptr, 10);
		entryData.sourceStart = numberValue;
		startIndex = endIndex + 1;

		endIndex = line.find(" ", startIndex);
		number_token = line.substr(startIndex, (endIndex - startIndex));
		numberValue = strtoul(number_token.c_str(), nullptr, 10);
		entryData.sourceEnd = entryData.sourceStart + numberValue;
		entryData.destinationEnd = entryData.destinationStart + numberValue;
		
		mapVector.push_back(entryData);
	}

	ulong TranslateMappedValue(const ulong sourceValue, const std::vector<RangeEntry>& mapVector)
	{
		for (int i = 0; i < mapVector.size(); i++)
		{
			const RangeEntry& entryData = mapVector[i];
			if (sourceValue >= entryData.sourceStart && sourceValue <= entryData.sourceEnd)
			{
				ulong delta = sourceValue - entryData.sourceStart;
				return entryData.destinationStart + delta;
			}
		}

		return sourceValue;
	}

	ulong TranslateMappedValueReverse(const ulong destinationValue, const std::vector<RangeEntry>& mapVector)
	{
		for (int i = 0; i < mapVector.size(); i++)
		{
			const RangeEntry& entryData = mapVector[i];
			if (destinationValue >= entryData.destinationStart && destinationValue <= entryData.destinationEnd)
			{
				ulong delta = destinationValue - entryData.destinationStart;
				return entryData.sourceStart + delta;
			}
		}

		return destinationValue;
	}

	ulong FindLocation(const ulong seedValue)
	{
		ulong value	= TranslateMappedValue(seedValue, seedToSoilMap);
		value = TranslateMappedValue(value, soilToFertilizerMap);
		value = TranslateMappedValue(value, fertilizerToWaterMap);
		value = TranslateMappedValue(value, waterToLightMap);
		value = TranslateMappedValue(value, lightToTemperatureMap);
		value = TranslateMappedValue(value, temperatureToHumidityMap);
		value = TranslateMappedValue(value, humidityToLocationMap);

		return value;
	}

	bool FindSeed(const ulong locationValue)
	{
		ulong value = TranslateMappedValueReverse(locationValue, humidityToLocationMap);
		value = TranslateMappedValueReverse(value, temperatureToHumidityMap);
		value = TranslateMappedValueReverse(value, lightToTemperatureMap);
		value = TranslateMappedValueReverse(value, waterToLightMap);
		value = TranslateMappedValueReverse(value, fertilizerToWaterMap);
		value = TranslateMappedValueReverse(value, soilToFertilizerMap);
		value = TranslateMappedValueReverse(value, seedToSoilMap);

		//find it in the seed vector if it's there
		for (int i = 0; i < seedRangeValues.size(); i++)
		{
			Range& entryData = seedRangeValues[i];
			if (value >= entryData.start && value <= (entryData.start + entryData.length)) //should store the end but listen I fucked up
			{
				return true;
			}
		}

		return false;
	}

	int AdventOfCodeDayFive()
	{
		using namespace std;

		string inputFileName = string(DATA_DIRECTORY) + string("Day5/input.txt");

		ifstream inputFile;
		inputFile.open(inputFileName);

		ulong lowestLocation = MAXULONG32;
		ulong lowestLocationPartTwo = 0;

		string line;
		assert(inputFile.is_open());
		{
			getline(inputFile, line);

			//line 1 is seeds
			ParseSeedValues(line);

			//GONNA DO THIS DUMB LET'S GO
			//find the next map
			{
				while (getline(inputFile, line))
				{
					if (strstr(line.c_str(), "seed-to-soil") != 0)
					{
						break;
					}
				}

				while (getline(inputFile, line))
				{
					if (isdigit(line[0]))
					{
						ParseMapValues(line, seedToSoilMap);
					}
					else { break; }
				}
			}
			{
				while (getline(inputFile, line))
				{
					if (strstr(line.c_str(), "soil-to-fertilizer") != 0)
					{
						break;
					}
				}

				while (getline(inputFile, line))
				{
					if (isdigit(line[0]))
					{
						ParseMapValues(line, soilToFertilizerMap);
					}
					else { break; }
				}
			}
			{
				while (getline(inputFile, line))
				{
					if (strstr(line.c_str(), "fertilizer-to-water") != 0)
					{
						break;
					}
				}

				while (getline(inputFile, line))
				{
					if (isdigit(line[0]))
					{
						ParseMapValues(line, fertilizerToWaterMap);
					}
					else { break; }
				}
			}
			{
				while (getline(inputFile, line))
				{
					if (strstr(line.c_str(), "water-to-light") != 0)
					{
						break;
					}
				}

				while (getline(inputFile, line))
				{
					if (isdigit(line[0]))
					{
						ParseMapValues(line, waterToLightMap);
					}
					else { break; }
				}
			}
			{
				while (getline(inputFile, line))
				{
					if (strstr(line.c_str(), "light-to-temperature") != 0)
					{
						break;
					}
				}

				while (getline(inputFile, line))
				{
					if (isdigit(line[0]))
					{
						ParseMapValues(line, lightToTemperatureMap);
					}
					else { break; }
				}
			}
			{
				while (getline(inputFile, line))
				{
					if (strstr(line.c_str(), "temperature-to-humidity") != 0)
					{
						break;
					}
				}

				while (getline(inputFile, line))
				{
					if (isdigit(line[0]))
					{
						ParseMapValues(line, temperatureToHumidityMap);
					}
					else { break; }
				}
			}
			{
				while (getline(inputFile, line))
				{
					if (strstr(line.c_str(), "humidity-to-location") != 0)
					{
						break;
					}
				}

				while (getline(inputFile, line))
				{
					if (isdigit(line[0]))
					{
						ParseMapValues(line, humidityToLocationMap);
					}
					else { break; }
				}
			}
			inputFile.close();

			//Ok file is parsed, let's operate
			for (int i = 0; i < seedValues.size(); i++)
			{
				ulong locationSpot = FindLocation(seedValues[i]);
				if (locationSpot < lowestLocation)
				{
					lowestLocation = locationSpot;
				}
			}

			//wheee
			while (lowestLocationPartTwo != MAXULONG32 && !FindSeed(lowestLocationPartTwo))
			{
				lowestLocationPartTwo++;
			}
						
		} //assert file is open


		std::cout << "Day 5 - Part One answer: " << lowestLocation << " and Part Two: " << lowestLocationPartTwo << endl;
		return lowestLocation;
	}
}

/***************************************************************
*  Day 6
****************************************************************/
namespace AdventDaySix
{
	struct RecordTimes
	{
		int time = 0;
		int distance = 0;
	};

	int FindNextNumber(const std::string& line, size_t& startIndex)
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

	int DetermineWinningPossibilitiesForRace(const llong endTime, const llong recordDistance)
	{
		int possibleHoldTime = 1; //0 never wins, get out of here (nor does full time)
		int winningTimesCount = 0;

		for (; possibleHoldTime < endTime; possibleHoldTime++)
		{
			if (possibleHoldTime * (endTime - possibleHoldTime) >= recordDistance)
			{
				++winningTimesCount;
			}
		}

		return winningTimesCount;
	}

	int DetermineWinningPossibilitiesForRace(const RecordTimes& raceTimes)
	{
		return DetermineWinningPossibilitiesForRace((llong)raceTimes.time, (llong)raceTimes.distance);
	}

	int AdventOfCodeDaySix()
	{
		using namespace std;

		string inputFileName = string(DATA_DIRECTORY) + string("Day6/input.txt");

		ifstream inputFile;
		inputFile.open(inputFileName);

		RecordTimes time1;
		RecordTimes time2;
		RecordTimes time3;
		RecordTimes time4;

		int partOneAnswer = 0;
		int partTwoAnswer = 0;

		string line;
		assert(inputFile.is_open());
		{
			//file is only two lines long, let's make this straightforward
			getline(inputFile, line);
			size_t startIndex = 0;
			
			//startIndex is updated inside the function
			time1.time = FindNextNumber(line, startIndex);
			time2.time = FindNextNumber(line, startIndex);
			time3.time = FindNextNumber(line, startIndex);
			time4.time = FindNextNumber(line, startIndex);

			startIndex = 0;
			getline(inputFile, line);
			time1.distance = FindNextNumber(line, startIndex);
			time2.distance = FindNextNumber(line, startIndex);
			time3.distance = FindNextNumber(line, startIndex);
			time4.distance = FindNextNumber(line, startIndex);
		}
		inputFile.close();

		int race1Possibilities = DetermineWinningPossibilitiesForRace(time1);
		int race2Possibilities = DetermineWinningPossibilitiesForRace(time2);
		int race3Possibilities = DetermineWinningPossibilitiesForRace(time3);
		int race4Possibilities = DetermineWinningPossibilitiesForRace(time4);

		partOneAnswer = race1Possibilities * race2Possibilities * race3Possibilities * race4Possibilities;

		//re-read the file for part 2...
		inputFile.open(inputFileName);

		llong bigRaceTime = 0;
		llong bigRaceDistance = 0;
		assert(inputFile.is_open());
		{
			getline(inputFile, line);
			int lineIndex = 0;
			string numberString;
			
			while(lineIndex < line.size())
			{
				if(isdigit(line[lineIndex]))
				{
					numberString.push_back(line[lineIndex]);
				}
				lineIndex++;
			}

			bigRaceTime = atoll(numberString.c_str());

			getline(inputFile, line);
			lineIndex = 0;
			numberString.clear();

			while (lineIndex < line.size())
			{
				if (isdigit(line[lineIndex]))
				{
					numberString.push_back(line[lineIndex]);
				}
				lineIndex++;
			}

			bigRaceDistance = atoll(numberString.c_str());
		}
		inputFile.close();

		partTwoAnswer = DetermineWinningPossibilitiesForRace(bigRaceTime, bigRaceDistance);

		std::cout << "Day 6 - Part One answer: " << partOneAnswer << " and Part Two: " << partTwoAnswer << endl;
		return 0;
	}
}

/***************************************************************
*  Day 7
****************************************************************/
namespace AdventDaySeven
{
	constexpr int MAX_HAND_SIZE = 5;

	enum HandType
	{
		HANDTYPE_UNKNOWN = -1,
		HANDTYPE_FIRST = 0,
		HANDTYPE_HIGHCARD = HANDTYPE_FIRST,
		HANDTYPE_ONEPAIR,
		HANDTYPE_TWOPAIR,
		HANDTYPE_THREEOFAKIND,
		HANDTYPE_FULLHOUSE,
		HANDTYPE_FOUROFAKIND,
		HANDTYPE_FIVEOFAKIND,
		HANDTYPE_MAX = HANDTYPE_FIVEOFAKIND
	};

	struct CamelCardHand
	{
		char hand[MAX_HAND_SIZE + 1];
		int bid = 0;
		HandType type = HANDTYPE_UNKNOWN;

		//might not use this
		int rank = -1;
	};

	std::vector<CamelCardHand> handVector;

	std::vector<CamelCardHand*> FiveOfAKinds;
	std::vector<CamelCardHand*> FourOfAKinds;
	std::vector<CamelCardHand*> FullHouses;
	std::vector<CamelCardHand*> ThreeOfAKinds;
	std::vector<CamelCardHand*> TwoPairs;
	std::vector<CamelCardHand*> OnePairs;
	std::vector<CamelCardHand*> HighCards;

	constexpr int CARD_TYPE_COUNT = 13;

	const std::map<char, int> CardValueOrder =
	{
		{ '2', 1 },
		{ '3', 2 },
		{ '4', 3 },
		{ '5', 4 },
		{ '6', 5 },
		{ '7', 6 },
		{ '8', 7 },
		{ '9', 8 },
		{ 'T', 9 },
		{ 'J', 10 },
		{ 'Q', 11 },
		{ 'K', 12 },
		{ 'A', 13 },
	};

	void ParseHands()
	{
		struct CardInfo
		{
			CardInfo(char value)
			{
				cardValue = value;
				count = 1;
			}

			char cardValue;
			int count = 0;
		};

		std::vector<CardInfo> cardFoundInfo;

		for (int i = 0; i < handVector.size(); i++)
		{
			CamelCardHand& hand = handVector[i];

			//find out the card info, looking for pairs
			for (int cardIndex = 0; cardIndex < MAX_HAND_SIZE; cardIndex++)
			{
				char cardvalue = hand.hand[cardIndex];
				bool seen = false;
				for (unsigned int foundCardIndex = 0; foundCardIndex < cardFoundInfo.size(); foundCardIndex++)
				{
					if (cardvalue == cardFoundInfo[foundCardIndex].cardValue)
					{
						cardFoundInfo[foundCardIndex].count++;
						seen = true;
					}
				}

				if (!seen)
				{
					cardFoundInfo.push_back(CardInfo(cardvalue));
				}
			}

			//determine the hand type
			if (cardFoundInfo.size() == 1)
			{
				assert(cardFoundInfo[0].count == 5);

				hand.type = HANDTYPE_FIVEOFAKIND;
				FiveOfAKinds.push_back(&hand);
			}
			else if (cardFoundInfo.size() == 2)
			{
				if (cardFoundInfo[0].count == 3 || cardFoundInfo[0].count == 2)
				{
					hand.type = HANDTYPE_FULLHOUSE;
					FullHouses.push_back(&hand);
				}
				else if (cardFoundInfo[0].count == 4 || cardFoundInfo[1].count == 4)
				{
					hand.type = HANDTYPE_FOUROFAKIND;
					FourOfAKinds.push_back(&hand);
				}
			}
			else
			{
				int pairCount = 0;
				int highestMatch = 0;
				for (int cardFoundIndex = 0; cardFoundIndex < cardFoundInfo.size(); cardFoundIndex++)
				{
					if (cardFoundInfo[cardFoundIndex].count > highestMatch)
					{
						highestMatch = cardFoundInfo[cardFoundIndex].count;
					}
					if (cardFoundInfo[cardFoundIndex].count == 2)
					{
						pairCount++;
					}
				}

				if (highestMatch == 3)
				{
					hand.type = HANDTYPE_THREEOFAKIND;
					ThreeOfAKinds.push_back(&hand);
				}
				else if (pairCount == 2)
				{
					hand.type = HANDTYPE_TWOPAIR;
					TwoPairs.push_back(&hand);
				}
				else if (pairCount == 1)
				{
					hand.type = HANDTYPE_ONEPAIR;
					OnePairs.push_back(&hand);
				}
				else
				{
					assert(pairCount == 0 && highestMatch == 1);
					hand.type = HANDTYPE_HIGHCARD;
					HighCards.push_back(&hand);
				}
			}
			cardFoundInfo.clear();
		}
	}

	void SortHandType(std::vector<CamelCardHand*>& handTypeVector)
	{
		//sort by first card
		//Ascending
		std::sort(handTypeVector.begin(), handTypeVector.end(), [](CamelCardHand* a, CamelCardHand*b) 
			{
				for (int i = 0; i < MAX_HAND_SIZE; i++)
				{
					const char& characterA = a->hand[i];
					const char& characterB = b->hand[i];
					int valueA = CardValueOrder.at(characterA);
					int valueB = CardValueOrder.at(characterB);

					if (valueA < valueB)
					{
						return true;
					}
					else if (valueA > valueB)
					{
						return false;
					}
					//or keep going
				}
				
				//they match
				return true;
			}
		);
	}

	int AdventOfCodeDaySeven()
	{
		using namespace std;

		string inputFileName = string(DATA_DIRECTORY) + string("Day7/input.txt");

		ifstream inputFile;
		inputFile.open(inputFileName);

		long partOneAnswer = 0;
		long partTwoAnswer = 0;

		handVector.reserve(1000); //file is 1000 long

		string line;
		assert(inputFile.is_open());
		{
			while(getline(inputFile, line))
			{
				CamelCardHand hand;
				strncpy_s(hand.hand, line.c_str(), 5);

				size_t numberStart = 6;
				hand.bid = Helpers::FindNextInt(line, numberStart);

				handVector.push_back(hand);
			}
		}
		inputFile.close();

		ParseHands();
		SortHandType(FiveOfAKinds);
		SortHandType(FourOfAKinds);
		SortHandType(FullHouses);
		SortHandType(ThreeOfAKinds);
		SortHandType(TwoPairs);
		SortHandType(OnePairs);
		SortHandType(HighCards);

		int ranking = 1;
		auto AssignRanksAndSumBids = [](std::vector<CamelCardHand*> inVector, int& currentRanking)
		{
			long total = 0;
			for (size_t i = 0; i < inVector.size(); i++)
			{
				inVector[i]->rank = currentRanking;
				total += (currentRanking * inVector[i]->bid);
				currentRanking++;
			}

			return total;
		};

		partOneAnswer += AssignRanksAndSumBids(HighCards, ranking);
		partOneAnswer += AssignRanksAndSumBids(OnePairs, ranking);
		partOneAnswer += AssignRanksAndSumBids(TwoPairs, ranking);
		partOneAnswer += AssignRanksAndSumBids(ThreeOfAKinds, ranking);
		partOneAnswer += AssignRanksAndSumBids(FullHouses, ranking);
		partOneAnswer += AssignRanksAndSumBids(FourOfAKinds, ranking);
		partOneAnswer += AssignRanksAndSumBids(FiveOfAKinds, ranking);
		
		std::cout << "Day 7 - Part One answer: " << partOneAnswer << " and Part Two: " << partTwoAnswer << endl;
		return 0;
	}
}