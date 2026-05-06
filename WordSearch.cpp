#include "WordSearch.h"

#include <array>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>
#include <vector>

namespace
{
    // Direction vectors for 8 compass directions
    constexpr std::array<int, 8> DIR_DR{ -1, -1,  0,  1,  1,  1,  0, -1 };
    constexpr std::array<int, 8> DIR_DC{ 0,  1,  1,  1,  0, -1, -1, -1 };

    // Shared text fragments to avoid duplication
    constexpr const char* NL2 = "\n\n";
    constexpr const char* MSG_ERR_OPEN = "Error: cannot open ";

    // Helper to collect words that were not matched
    void buildUnmatched(const std::vector<std::string>& wordList,
        const std::set<std::string>& foundSet,
        std::vector<std::string>& unmatchedWords)
    {
        for (const auto& w : wordList)
        {
            if (foundSet.find(w) == foundSet.end())
            {
                unmatchedWords.push_back(w);
            }
        }
    }
}

// Constructor
WordSearch::WordSearch(const std::string& puzzleFile,
    const std::string& dictionaryFile)
    : _puzzleFile(puzzleFile)
    , _dictionaryFile(dictionaryFile)
    , _gridSize(0)
    , _topLeft(nullptr)
    , _trieRoot(nullptr)
    , _usingAdvancedPuzzle(false)
    , _usingAdvancedDictionary(false)
    , _gridCellsVisited(0)
    , _dictEntriesVisited(0)
{
}

// Destructor must not throw
WordSearch::~WordSearch() noexcept
{
    try
    {
        if (_trieRoot != nullptr)
        {
            deleteTrie(_trieRoot);
        }
    }
    catch (...)
    {
        // Destructors must not allow exceptions to escape
    }

    _trieRoot = nullptr;
}

// Safe recursive deletion of trie nodes
void WordSearch::deleteTrie(TrieNode* const node)
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < 26; ++i)
    {
        deleteTrie(node->children[i]);
    }

    delete node;
}

// Create simple grid using vector<vector<char>>
uint32_t WordSearch::createSimplePuzzle(std::chrono::microseconds& duration)
{
    std::cout << "Creating simple puzzle..." << std::endl;

    std::ifstream file(_puzzleFile);
    if (!file.is_open())
    {
        std::cerr << MSG_ERR_OPEN << _puzzleFile << '\n';
        duration = std::chrono::microseconds(0);
        return 0U;
    }

    const auto start = std::chrono::high_resolution_clock::now();

    file >> _gridSize;
    _simpleGrid.assign(_gridSize, std::vector<char>(_gridSize, '\0'));

    for (int r = 0; r < _gridSize; ++r)
    {
        for (int c = 0; c < _gridSize; ++c)
        {
            char temp = '\0';
            file >> temp;
            const char ch = temp;
            _simpleGrid[r][c] = ch;
        }
    }

    _usingAdvancedPuzzle = false;

    const uint32_t byteSize =
        sizeof(std::vector<std::vector<char>>) +
        static_cast<uint32_t>(_gridSize * sizeof(std::vector<char>)) +
        static_cast<uint32_t>(_gridSize * _gridSize * sizeof(char));

    duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start);

    return byteSize;
}

// Create advanced grid using flat vector of GridCell objects
uint32_t WordSearch::createAdvancedPuzzle(std::chrono::microseconds& duration)
{
    std::cout << "Creating advanced puzzle..." << std::endl;

    std::ifstream file(_puzzleFile);
    if (!file.is_open())
    {
        std::cerr << MSG_ERR_OPEN << _puzzleFile << '\n';
        duration = std::chrono::microseconds(0);
        return 0U;
    }

    const auto start = std::chrono::high_resolution_clock::now();

    file >> _gridSize;
    const int total = _gridSize * _gridSize;

    _advancedCells.resize(static_cast<std::size_t>(total));

    for (int r = 0; r < _gridSize; ++r)
    {
        for (int c = 0; c < _gridSize; ++c)
        {
            char temp = '\0';
            file >> temp;
            const char ch = temp;

            GridCell& cell = _advancedCells[r * _gridSize + c];
            cell.letter = ch;
            cell.row = r;
            cell.col = c;
        }
    }

    for (int r = 0; r < _gridSize; ++r)
    {
        for (int c = 0; c < _gridSize; ++c)
        {
            GridCell& cell = _advancedCells[r * _gridSize + c];

            for (int d = 0; d < 8; ++d)
            {
                const int nr = r + DIR_DR[d];
                const int nc = c + DIR_DC[d];

                if (nr >= 0 && nr < _gridSize && nc >= 0 && nc < _gridSize)
                {
                    cell.neighbors[d] = &_advancedCells[nr * _gridSize + nc];
                }
                else
                {
                    cell.neighbors[d] = nullptr;
                }
            }
        }
    }

    _topLeft = &_advancedCells[0];
    _usingAdvancedPuzzle = true;

    const uint32_t byteSize =
        static_cast<uint32_t>(total * sizeof(GridCell));

    duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start);

    return byteSize;
}

// Create simple dictionary using vector<string>
uint32_t WordSearch::createSimpleDictionary(std::chrono::microseconds& duration)
{
    std::cout << "Creating simple dictionary..." << std::endl;

    std::ifstream file(_dictionaryFile);
    if (!file.is_open())
    {
        std::cerr << MSG_ERR_OPEN << _dictionaryFile << '\n';
        duration = std::chrono::microseconds(0);
        return 0U;
    }

    const auto start = std::chrono::high_resolution_clock::now();

    std::string word;
    while (file >> word)
    {
        _simpleDictionary.push_back(word);
        _wordList.push_back(word);
    }

    _usingAdvancedDictionary = false;

    uint32_t byteSize = sizeof(std::vector<std::string>);
    for (const auto& w : _simpleDictionary)
    {
        byteSize += static_cast<uint32_t>(sizeof(std::string) + w.size());
    }

    duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start);

    return byteSize;
}

// Create advanced dictionary using Trie
uint32_t WordSearch::createAdvancedDictionary(std::chrono::microseconds& duration)
{
    std::cout << "Creating advanced dictionary..." << std::endl;

    std::ifstream file(_dictionaryFile);
    if (!file.is_open())
    {
        std::cerr << MSG_ERR_OPEN << _dictionaryFile << '\n';
        duration = std::chrono::microseconds(0);
        return 0U;
    }

    const auto start = std::chrono::high_resolution_clock::now();

    _trieRoot = new TrieNode();
    uint32_t nodeCount = 1U;

    std::string word;
    while (file >> word)
    {
        _wordList.push_back(word);

        TrieNode* node = _trieRoot;
        for (const char ch : word)
        {
            const int idx = ch - 'A';
            if (idx < 0 || idx >= 26)
            {
                node = nullptr;
                break;
            }

            if (node->children[idx] == nullptr)
            {
                node->children[idx] = new TrieNode();
                ++nodeCount;
            }

            node = node->children[idx];
        }

        if (node != nullptr)
        {
            node->isWord = true;
            node->word = word;
        }
    }

    _usingAdvancedDictionary = true;

    const uint32_t byteSize =
        nodeCount * static_cast<uint32_t>(sizeof(TrieNode));

    duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start);

    return byteSize;
}

// Solve using simple grid and simple dictionary
void WordSearch::solveSimpleSimple()
{
    std::set<std::string> foundSet;

    for (const auto& word : _simpleDictionary)
    {
        const int wlen = static_cast<int>(word.size());
        bool found = false;

        for (int r = 0; r < _gridSize && !found; ++r)
        {
            for (int c = 0; c < _gridSize && !found; ++c)
            {
                for (int d = 0; d < 8 && !found; ++d)
                {
                    ++_dictEntriesVisited;
                    bool match = true;

                    for (int k = 0; k < wlen; ++k)
                    {
                        const int nr = r + k * DIR_DR[d];
                        const int nc = c + k * DIR_DC[d];
                        ++_gridCellsVisited;

                        if (nr < 0 || nr >= _gridSize ||
                            nc < 0 || nc >= _gridSize ||
                            _simpleGrid[nr][nc] != word[k])
                        {
                            match = false;
                            break;
                        }
                    }

                    if (match)
                    {
                        _matchedWords.push_back({ c, r, word });
                        foundSet.insert(word);
                        found = true;
                    }
                }
            }
        }

        if (!found)
        {
            _unmatchedWords.push_back(word);
        }
    }
}

// Solve using simple grid and Trie dictionary
void WordSearch::solveSimpleAdvanced()
{
    std::set<std::string> foundSet;

    for (int r = 0; r < _gridSize; ++r)
    {
        for (int c = 0; c < _gridSize; ++c)
        {
            for (int d = 0; d < 8; ++d)
            {
                TrieNode* node = _trieRoot;
                int cr = r;
                int cc = c;

                while (cr >= 0 && cr < _gridSize &&
                    cc >= 0 && cc < _gridSize)
                {
                    ++_gridCellsVisited;
                    ++_dictEntriesVisited;

                    const int idx = _simpleGrid[cr][cc] - 'A';
                    if (idx < 0 || idx >= 26)
                    {
                        break;
                    }

                    node = node->children[idx];
                    if (node == nullptr)
                    {
                        break;
                    }

                    if (node->isWord &&
                        foundSet.find(node->word) == foundSet.end())
                    {
                        _matchedWords.push_back({ c, r, node->word });
                        foundSet.insert(node->word);
                    }

                    cr += DIR_DR[d];
                    cc += DIR_DC[d];
                }
            }
        }
    }

    buildUnmatched(_wordList, foundSet, _unmatchedWords);
}

// Solve using advanced grid and simple dictionary
void WordSearch::solveAdvancedSimple()
{
    std::set<std::string> foundSet;

    for (const auto& word : _simpleDictionary)
    {
        const int wlen = static_cast<int>(word.size());
        bool found = false;

        for (GridCell* row = _topLeft;
            row != nullptr && !found;
            row = row->neighbors[4])
        {
            for (GridCell* cell = row;
                cell != nullptr && !found;
                cell = cell->neighbors[2])
            {
                for (int d = 0; d < 8 && !found; ++d)
                {
                    ++_dictEntriesVisited;
                    bool match = true;
                    GridCell* cur = cell;

                    for (int k = 0; k < wlen; ++k)
                    {
                        if (cur == nullptr)
                        {
                            match = false;
                            break;
                        }

                        ++_gridCellsVisited;

                        if (cur->letter != word[k])
                        {
                            match = false;
                            break;
                        }

                        if (k < wlen - 1)
                        {
                            cur = cur->neighbors[d];
                        }
                    }

                    if (match)
                    {
                        _matchedWords.push_back({ cell->col, cell->row, word });
                        foundSet.insert(word);
                        found = true;
                    }
                }
            }
        }

        if (!found)
        {
            _unmatchedWords.push_back(word);
        }
    }
}

// Solve using advanced grid and Trie dictionary
void WordSearch::solveAdvancedAdvanced()
{
    std::set<std::string> foundSet;

    for (GridCell* row = _topLeft; row != nullptr; row = row->neighbors[4])
    {
        for (GridCell* cell = row; cell != nullptr; cell = cell->neighbors[2])
        {
            for (int d = 0; d < 8; ++d)
            {
                TrieNode* node = _trieRoot;
                GridCell* cur = cell;

                while (cur != nullptr)
                {
                    ++_gridCellsVisited;
                    ++_dictEntriesVisited;

                    const int idx = cur->letter - 'A';
                    if (idx < 0 || idx >= 26)
                    {
                        break;
                    }

                    node = node->children[idx];
                    if (node == nullptr)
                    {
                        break;
                    }

                    if (node->isWord &&
                        foundSet.find(node->word) == foundSet.end())
                    {
                        _matchedWords.push_back({ cell->col, cell->row, node->word });
                        foundSet.insert(node->word);
                    }

                    cur = cur->neighbors[d];
                }
            }
        }
    }

    buildUnmatched(_wordList, foundSet, _unmatchedWords);
}

// Dispatch to correct solver
void WordSearch::solvePuzzle(std::chrono::microseconds& duration)
{
    std::cout << "Solving puzzle..." << std::endl;

    _matchedWords.clear();
    _unmatchedWords.clear();
    _gridCellsVisited = 0;
    _dictEntriesVisited = 0;

    const auto start = std::chrono::high_resolution_clock::now();

    if (!_usingAdvancedPuzzle && !_usingAdvancedDictionary)
    {
        solveSimpleSimple();
    }
    else if (!_usingAdvancedPuzzle && _usingAdvancedDictionary)
    {
        solveSimpleAdvanced();
    }
    else if (_usingAdvancedPuzzle && !_usingAdvancedDictionary)
    {
        solveAdvancedSimple();
    }
    else
    {
        solveAdvancedAdvanced();
    }

    duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start);
}

// Write results to output file
void WordSearch::outputResults(const std::string& outputFile,
    const PerformanceResult& data)
{
    std::cout << "Outputting results to " << outputFile << "\n" << std::endl;

    std::ofstream out(outputFile);
    if (!out.is_open())
    {
        std::cerr << MSG_ERR_OPEN << outputFile << '\n';
        return;
    }

    out << "Number of words matched: " << _matchedWords.size() << NL2;

    out << "Words matched in grid:\n";
    for (const auto& mw : _matchedWords)
    {
        out << mw.col << " " << mw.row << " " << mw.word << "\n";
    }
    out << NL2;

    out << "Words unmatched in grid:\n";
    for (const auto& uw : _unmatchedWords)
    {
        out << uw << "\n";
    }
    out << NL2;

    out << "Number of grid cells visited: " << _gridCellsVisited << NL2;
    out << "Number of dictionary entries visited: " << _dictEntriesVisited << NL2;

    const double gridTime =
        static_cast<double>(data.timeToCreatePuzzle.count()) / 1e6;
    const double dictTime =
        static_cast<double>(data.timeToCreateDictionary.count()) / 1e6;
    const double solveTime =
        static_cast<double>(data.timeToSolve.count()) / 1e6;

    out << std::fixed << std::setprecision(6);
    out << "Time to populate grid: " << gridTime << NL2;
    out << "Time to populate dictionary: " << dictTime << NL2;
    out << "Time to solve puzzle: " << solveTime << NL2;

    out << "Size of the grid data structure: " << data.puzzleSize << NL2;
    out << "Size of the dictionary data structure: " << data.dictionarySize << "\n";
}