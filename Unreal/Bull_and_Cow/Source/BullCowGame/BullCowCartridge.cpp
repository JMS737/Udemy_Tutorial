// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
	Super::BeginPlay();
	const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
	FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath, [](const FString &Word)
													{ return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word); });

	// Isograms = GetValidWords(Words);
	// TArray<FString> ValidWords = GetValidWords(Words, 3);

	InitGame();

	// PrintLine(TEXT("The number of possible words is %i"), Words.Num());
	// PrintLine(TEXT("The number of valid words is %i"), ValidWords.Num());

	// int32 randomNumber = FMath::RandRange(0, 10);
	// PrintLine(TEXT("Random Number: %i"), randomNumber);

	// for (int32 i = 0; i < ValidWords.Num(); i++)
	// {
	// 	PrintLine(TEXT("%s"), *ValidWords[i]);
	// }
}

// TArray<FString> UBullCowCartridge::GetValid;Words(const TArray<FString> &Words, int32 Limit) const
// {
// 	TArray<FString> ValidWords;

// 	int32 Count = 0;
// 	for (FString Word : Words)
// 	{
// 		if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
// 		{
// 			ValidWords.Emplace(Word);
// 			++Count;

// 			if (Limit > 0 && Count >= Limit)
// 			{
// 				break;
// 			}
// 		}
// 	}

// 	return ValidWords;
// }

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{

	if (bGameOver)
	{
		ClearScreen();
		InitGame();
		return;
	}

	ProcessGuess(Input);
}

void UBullCowCartridge::InitGame()
{
	HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
	Lives = HiddenWord.Len();
	bGameOver = false;

	Welcome();

	// Use FString::Printf("format (e.g. %s, %i)", *myStr, myInt) to format strings. This is build into the PrintLine method.
	PrintLine(FString::Printf(TEXT("The HiddenWord is: %s"), *HiddenWord));

	const TCHAR HW[] = TEXT("cakes");
}

void UBullCowCartridge::EndGame()
{
	bGameOver = true;

	PrintLine(TEXT("\nPress enter to play again..."));
}

void UBullCowCartridge::Welcome()
{
	// Welcome the Player
	PrintLine(TEXT("Hi there! Welcome to Bull Cow?!"));
	PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len()); // TODO: Calculate length
	PrintLine(TEXT("You have %i lives."), Lives);
	PrintLine(TEXT("Press enter to continue..."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
	/* Check Input is Valid
		- Does it's length match
		- Is it a isogram?
		- Iterate through the characters
			- Do any characters match
			- Are any characters in the correct position
	*/

	if (Guess == HiddenWord)
	{
		PrintLine(TEXT("You guessed correctly!"));
		EndGame();
		return;
	}

	if (Guess.Len() < HiddenWord.Len())
	{
		PrintLine(TEXT("Code was not long enough.\nIt should be %i characters."), HiddenWord.Len());
		PrintLine(TEXT("Sorry, try guessing again! You have %i lives remaining"), Lives);
		return;
	}

	if (HiddenWord.Len() != Guess.Len())
	{
		PrintLine(TEXT("The hidden word is %i characters long, try again..."), HiddenWord.Len());
		PrintLine(TEXT("Sorry, try guessing again! You have %i lives remaining"), Lives);
		return;
	}

	// Check if Isogram
	if (!IsIsogram(Guess))
	{
		PrintLine(TEXT("No repeating letters, guess again"));
		return;
	}

	--Lives;

	if (Lives == 0)
	{
		ClearScreen();
		PrintLine(TEXT("You ran out of lives!"));
		PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
		EndGame();
		return;
	}

	FBullCowCount Count = GetBullCows(Guess);

	PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls, Count.Cows);

	PrintLine(TEXT("Guess again! You have %i lives left..."), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString &Guess)
{
	for (int32 i = 0; i < Guess.Len(); i++)
	{
		for (int32 j = i + 1; j < Guess.Len(); j++)
		{
			if (Guess[i] == Guess[j])
			{
				return false;
			}
		}
	}

	return true;
}

// Use the '&' to pass by reference.
// void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
	FBullCowCount Count;

	for (int32 i = 0; i < Guess.Len(); i++)
	{
		if (Guess[i] == HiddenWord[i])
		{
			Count.Bulls++;
			continue;
		}

		for (int32 j = 0; j < Guess.Len(); j++)
		{
			if (Guess[i] == HiddenWord[j])
			{
				Count.Cows++;
				break;
			}
		}
	}

	return Count;
}