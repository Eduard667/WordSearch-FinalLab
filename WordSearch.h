#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// Performance metrics for output
// Layout: microseconds (8B) x3, then uint32_t (4B) x2 — two uint32_t pack into
// one 8-byte slot with no internal padding.
struct PerformanceResult
{
    // Order optimised for memory layout (largest → smallest)
    std::chrono::microseconds timeToCreatePuzzle{};
    std::chrono::microseconds timeToCreateDictionary{};
    std::chrono::microseconds timeToSolve{};

    uint32_t puzzleSize = 0;
    uint32_t dictionarySize = 0;
};

// One cell in the advanced grid
// Layout: array<ptr,8> (64B), int (4B), int (4B), char (1B + 3B padding)
struct GridCell
{
    // Non-owning pointers to neighbours — largest member first
    std::array<GridCell*, 8> neighbors{};

    int row = 0;
    int col = 0;

    char letter = '\0';
};

// Trie node for advanced dictionary
// Layout: array<ptr,26> (208B), string (32B), bool (1B + 7B padding)
struct TrieNode
{
    // Non-owning pointers to children — largest member first
    std::array<TrieNode*, 26> children{};

    std::string word;

    bool isWord = false;
};

// A matched word and its starting position
// Layout: string (32B), int (4B), int (4B) — no padding between ints
struct MatchedWord
{
    std::string word;

    int col = 0;
    int row = 0;
};

// Custom deleter for the Trie — recursively frees all nodes.
// Using a named functor makes ownership unambiguous to both humans and
// static-analysis tools (satisfies MRM.33).
struct TrieDeleter
{
    void operator()(TrieNode* node) const noexcept;
};

// WordSearch class (final because destructor is non-virtual)
class WordSearch final
{
public:
    WordSearch(const std::string& puzzleFile,
        const std::string& dictionaryFile);

    ~WordSearch() noexcept = default;  // unique_ptr<TrieNode,TrieDeleter> handles cleanup

    // Rule of 5: disable copying (owns heap memory via unique_ptr)
    WordSearch(const WordSearch&) = delete;
    WordSearch& operator=(const WordSearch&) = delete;

    // Allow moving
    WordSearch(WordSearch&&) noexcept = default;
    WordSearch& operator=(WordSearch&&) noexcept = default;

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

    // --- 24-byte (vector-sized) members ---
    // Simple grid
    std::vector<std::vector<char>> _simpleGrid;

    // Advanced grid
    std::vector<GridCell> _advancedCells;

    // Simple dictionary
    std::vector<std::string> _simpleDictionary;

    // Master word list
    std::vector<std::string> _wordList;

    // Solve results
    std::vector<MatchedWord> _matchedWords;
    std::vector<std::string> _unmatchedWords;

    // --- 8-byte (pointer/uint64-sized) members ---
    // Advanced dictionary (Trie) — unique_ptr makes ownership explicit
    std::unique_ptr<TrieNode, TrieDeleter> _trieRoot;

    // Non-owning pointer into _advancedCells
    GridCell* _topLeft = nullptr;

    // Counters
    uint64_t _gridCellsVisited = 0;
    uint64_t _dictEntriesVisited = 0;

    // --- 4-byte members ---
    int _gridSize = 0;

    // --- 1-byte members ---
    // Active structure flags
    bool _usingAdvancedPuzzle = false;
    bool _usingAdvancedDictionary = false;

    // Internal helpers
    void solveSimpleSimple();
    void solveSimpleAdvanced();
    void solveAdvancedSimple();
    void solveAdvancedAdvanced();
};