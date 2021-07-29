#include <iostream>
#include <ctime>

void PrintIntroduction(int Difficulty)
{
    // std::endl also flushes the buffer (equivalent to std::cout << "\n" << std::flush)
    std::cout << "\nYou are hacker and cybersecurity expert from the underworld, you are infiltrating a corporation (level "
              << Difficulty << ") to dig up dirt on the CEO..." << std::endl;
    std::cout << "You must find the correct codes to bypass the firewalls...\n\n";
}

// Returns a random number between 0 and the MaxNumber specified (exclusive)
int GetRandomNumber(int MaxNumber)
{
    return rand() % MaxNumber;
}

int GetCodeValue(int Difficulty)
{
    return GetRandomNumber(3 + (Difficulty * 2)) + 1;
}

bool PlayGame(int Difficulty)
{
    PrintIntroduction(Difficulty);

    int CodeA = GetCodeValue(Difficulty);
    int CodeB = GetCodeValue(Difficulty);
    int CodeC = GetCodeValue(Difficulty);

    int CodeSum = CodeA + CodeB + CodeC;
    int CodeProduct = CodeA * CodeB * CodeC;

    std::cout << "+ There are 3 numbers in the code\n";
    std::cout << "+ The codes add up to: " << CodeSum;
    std::cout << "\n+ The codes multiply to give: " << CodeProduct;

    int GuessA, GuessB, GuessC;

    std::cout << "\n\nEnter the code digit (separated by space): \n";

    // When retreiving a number the cin command will treat spaces as the end of a number.
    // Any remaining input will then be retreived as the next value.
    std::cin >> GuessA >> GuessB >> GuessC;

    int GuessSum = GuessA + GuessB + GuessC;
    int GuessProduct = GuessA * GuessB * GuessC;

    if (GuessSum == CodeSum && GuessProduct == CodeProduct)
    {
        std::cout << "\nYou guessed the code!\n";
        return true;
    }
    else
    {
        std::cout << "\nWrong code!\n";
        return false;
    }
}

int main()
{
    srand(time(NULL)); // set the random seed based on the time of day.

    int LevelDifficulty = 1;
    const int MaxLevel = 10;

    while (LevelDifficulty <= MaxLevel)
    {
        bool bLevelComplete = PlayGame(LevelDifficulty);
        std::cin.clear();  // Clears any errors
        std::cin.ignore(); // Discards the buffer

        if (bLevelComplete)
        {
            ++LevelDifficulty;
        }
    }

    std::cout << "Congradulations, you broke the firewall!\n";

    return 0;
}