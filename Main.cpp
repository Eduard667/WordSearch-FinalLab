#include <chrono>

#include "WordSearch.h"

// by far the best version yet lolol

int main(int, char**) {

	const std::string puzzle("grid.txt");
	const std::string dictionary("dictionary.txt");

	{
		PerformanceResult data;
		WordSearch ws(puzzle, dictionary);

		data.puzzleSize = ws.createSimplePuzzle(data.timeToCreatePuzzle);
		data.dictionarySize = ws.createSimpleDictionary(data.timeToCreateDictionary);
		ws.solvePuzzle(data.timeToSolve);
		ws.outputResults("simple_puzzle_simple_dictionary.txt", data);
	}


	{
		PerformanceResult data;
		WordSearch ws(puzzle, dictionary);

		data.puzzleSize = ws.createAdvancedPuzzle(data.timeToCreatePuzzle);
		data.dictionarySize = ws.createSimpleDictionary(data.timeToCreateDictionary);
		ws.solvePuzzle(data.timeToSolve);
		ws.outputResults("advanced_puzzle_simple_dictionary.txt", data);
	}


	{
		PerformanceResult data;
		WordSearch ws(puzzle, dictionary);

		data.puzzleSize = ws.createSimplePuzzle(data.timeToCreatePuzzle);
		data.dictionarySize = ws.createAdvancedDictionary(data.timeToCreateDictionary);
		ws.solvePuzzle(data.timeToSolve);
		ws.outputResults("simple_puzzle_advanced_dictionary.txt", data);
	}


	{
		PerformanceResult data;
		WordSearch ws(puzzle, dictionary);

		data.puzzleSize = ws.createAdvancedPuzzle(data.timeToCreatePuzzle);
		data.dictionarySize = ws.createAdvancedDictionary(data.timeToCreateDictionary);
		ws.solvePuzzle(data.timeToSolve);
		ws.outputResults("advanced_puzzle_advanced_dictionary.txt", data);
	}

	return 0;
}