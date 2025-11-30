// AdventOfCode.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "AdventPuzzleSolutions.h"

enum AOCYear
{
    AOCYear2023,
    AOCYear2024,
    AOCYear2025
};

int main()
{
    std::cout << "Advent of Code answers Below: \n";

    constexpr AOCYear YEAR_TO_RUN = AOCYear2024;



    if(YEAR_TO_RUN == AOCYear2023)
    {
        using namespace Year2023;
        AdventDayOne::AdventOfCodeDay1();
        AdventDayTwo::AdventOfCodeDayTwo();
        AdventDayThree::AdventOfCodeDayThree();
        AdventDayFour::AdventOfCodeDayFour();
        AdventDayFive::AdventOfCodeDayFive();
        AdventDaySix::AdventOfCodeDaySix();
        AdventDaySeven::AdventOfCodeDaySeven();
        AdventDayEight::AdventOfCodeDayEight();
    }
    else if(YEAR_TO_RUN == AOCYear2024)
    {
        using namespace Year2024;
        AdventDayOne::AdventOfCodeDay1();
        AdventDayTwo::AdventOfCodeDay2();

        std::cin.get();
    }
    else if(YEAR_TO_RUN == AOCYear2025)
    {
        using namespace Year2025;
        std::cin.get();
    }
    
}