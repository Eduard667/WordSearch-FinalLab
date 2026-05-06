# Linker error & What is needed in the .h and .cpp files

1. .h files tell the compiler what to expect, such as methods, constructors etc.
2. .cpp files tell the compiler how it is implemented / how it is used
3. .cpp file must have "#include file.h"

+-------------------------------------------+
|                .h File                    |
|-------------------------------------------|
|  • Declares class                         |
|  • Declares methods                       |
|  • Declares constructors/destructors      |
|  • No logic                               |
+-------------------------------------------+
                    |
                    |  included by
                    v
+-------------------------------------------+
|                .cpp File                  |
|-------------------------------------------|
|  • Implements methods                     |
|  • Contains logic                         |
|  • Uses #include "MyClass.h"              |
+-------------------------------------------+


# Reading into Grid Class

```cpp
void Grid::LoadGrid(const char filename[])
{
	std::ifstream fin; // Create an object of type ifstream called fin - which is used to open our file and read in from our file
	fin.open(filename);
	for (x = 0; x <= 8; x++) // For each row
	{
		for (y = 0; y <= 8; y++) // and for each column
		{
			fin >> m_grid[x][y]; // read in and add to our m_grid 2D array_
		}
	}

	fin.close(); // close the file

}
```
# Saving / Writing to a file with ifstream and ofstream

```cpp
void Grid::SaveGrid(const char filename[])
{
	std::ofstream fout; // create an object of type ofstream called fout (file out) 
	fout.open(filename); // open the file
	for (int x = 0; x <= 8; x++) // loop through the grid
	{
		for (int y = 0; y <= 8; y++)
		{
			fout << m_grid[x][y] << " "; // write the grid separated by spaces
		}
		fout << std::endl; // add a new line after each row

	}
	fout.close(); // close the file 
}
```

# Basics of Pointers
Here we confirm the address of *p pointed to by p is indeed "a"
I say this because 10 in hexadecimal is 0A. 
<img width="416" height="119" alt="image" src="https://github.com/user-attachments/assets/5e282a62-f5bc-44db-a83d-689166d3413f" />
<img width="295" height="85" alt="image" src="https://github.com/user-attachments/assets/a3d2413a-e0f0-47ec-ae59-bb6ac56108bf" />
Here I have added the code to update the value of the address pointer p is pointing to:
<img width="180" height="37" alt="image" src="https://github.com/user-attachments/assets/c43a0817-ad83-4aeb-9a74-931220184474" />
<img width="180" height="37" alt="image" src="https://github.com/user-attachments/assets/c43a0817-ad83-4aeb-9a74-931220184474" />


---
# Lab F
# 1. Code refactored header file for Grid.h
```cpp
#pragma once
#include <iostream>
#include <fstream>

template <class T>
class Grid
{
public:
    Grid();
    ~Grid();

    void LoadGrid(const char filename[]);
    void SaveGrid(const char filename[]);

private:
    T m_grid[9][9];
};


// Method Implementations


template <class T>
Grid<T>::Grid() {}

template <class T>
Grid<T>::~Grid() {}

template <class T>
void Grid<T>::LoadGrid(const char filename[])
{
    std::ifstream fin(filename);
    for (int x = 0; x < 9; x++)
        for (int y = 0; y < 9; y++)
            fin >> m_grid[x][y];
}

template <class T>
void Grid<T>::SaveGrid(const char filename[])
{
    std::ofstream fout(filename);
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
            fout << m_grid[x][y] << " ";
        fout << "\n";
    }
}

// operator<<
template <class T>
std::ostream& operator<<(std::ostream& os, const Grid<T>& g)
{
    for (int x = 0; x < 9; x++)
    {
        for (int y = 0; y < 9; y++)
            os << g.m_grid[x][y] << " ";
        os << "\n";
    }
    return os;
}

// operator>>
template <class T>
std::istream& operator>>(std::istream& is, Grid<T>& g)
{
    for (int x = 0; x < 9; x++)
        for (int y = 0; y < 9; y++)
            is >> g.m_grid[x][y];

    return is;
}
```

# Template Grid