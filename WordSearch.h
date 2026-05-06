#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

// Performance metrics for output
struct PerformanceResult
{
    uint32_t puzzleSize = 0;
    uint32_t dictionarySize = 0;

    // Order optimised for memory layout (largest → smallest)
    std::chrono::microseconds timeToCreatePuzzle{};
    std::chrono::microseconds timeToCreateDictionary{};
    std::chrono::microseconds timeToSolve{};
};

// One cell in the advanced grid
// Memory layout optimised: char → ints → pointer array
struct GridCell
{
    char letter = '\0';
    int row = 0;
    int col = 0;

    // Non-owning pointers to neighbours
    std::array<GridCell*, 8> neighbors{};
};

// Trie node for advanced dictionary
// Memory layout optimised: bool → string → pointer array
struct TrieNode
{
    bool isWord = false;
    std::string word;

    // Non-owning pointers to children
    std::array<TrieNode*, 26> children{};
};

// A matched word and its starting position
struct MatchedWord
{
    int col = 0;
    int row = 0;
    std::string word;
};

// WordSearch class (final because destructor is non-virtual)
class WordSearch final
{
public:
    WordSearch(const std::string& puzzleFile,
        const std::string& dictionaryFile);

    ~WordSearch() noexcept;

    // Rule of 5: disable copying (owns heap memory)
    WordSearch(const WordSearch&) = delete;
    WordSearch& operator=(const WordSearch&) = delete;

    // Allow moving if needed
    WordSearch(WordSearch&&) noexcept = default;
    WordSearch& operator=(WordSearch&&); //noexcept = default;

    uint32_t createSimplePuzzle(std::chrono::microseconds& duration);
    uint32_t createAdvancedPuzzle(std::chrono::microseconds& duration);
    uint32_t createSimpleDictionary(std::chrono::microseconds& duration);
    uint32_t createAdvancedDictionary(std::chrono::microseconds& duration);

    void solvePuzzle(std::chrono::microseconds& duration);
    void outputResults(const std::string& outputFile,
        const PerformanceResult& data);

private:
    // File paths
    const std::string _puzzleFile;
    const std::string _dictionaryFile;

    // Simple grid
    int _gridSize = 0;
    std::vector<std::vector<char>> _simpleGrid;

    // Advanced grid
    std::vector<GridCell> _advancedCells;
    GridCell* _topLeft = nullptr;   // Non-owning pointer

    // Simple dictionary
    std::vector<std::string> _simpleDictionary;

    // Advanced dictionary (Trie)
    TrieNode* _trieRoot = nullptr;  // Owning pointer

    // Master word list
    std::vector<std::string> _wordList;

    // Active structure flags
    bool _usingAdvancedPuzzle = false;
    bool _usingAdvancedDictionary = false;

    // Solve results
    std::vector<MatchedWord> _matchedWords;
    std::vector<std::string> _unmatchedWords;

    // Counters
    uint64_t _gridCellsVisited = 0;
    uint64_t _dictEntriesVisited = 0;

    // Internal helpers
    void solveSimpleSimple();
    void solveSimpleAdvanced();
    void solveAdvancedSimple();
    void solveAdvancedAdvanced();

    // Safe recursive deletion
    void deleteTrie(TrieNode* const node);
};