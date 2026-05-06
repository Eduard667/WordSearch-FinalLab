# 1. Auxiliary Functions: Operator<< and Operator>>

```cpp
void Grid::LoadGrid(const char filename[])
{
	std::ifstream fin(filename);
	for (int x = 0; x < 9; x++)
		for (int y = 0; y < 9; y++)
			fin >> m_grid[x][y];
}

void Grid::SaveGrid(const char filename[])
{
	std::ofstream fout(filename);
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
			fout << m_grid[x][y] << " ";
		fout << "\n";
	}
}

std::ostream& operator<<(std::ostream& os, const Grid& g)
{
	for (int x = 0; x < 9; x++)
	{
		for (int y = 0; y < 9; y++)
			os << g.m_grid[x][y] << " ";
		os << "\n";
	}
	return os;
}

std::istream& operator>>(std::istream& is, Grid& g)
{
	for (int x = 0; x < 9; x++)
		for (int y = 0; y < 9; y++)
			is >> g.m_grid[x][y];

	return is;
}
```
Reflection: 
Reason why we use Aux Functions:
- Original functions only work with files 
- Aux functions work with any input or output stream
-	They dont deal with files at all. 
	- Just read from or write to whatever stream theyre given.
	- This make our code more flexible.


# 2. Fractions

This is code for the Fraction.cpp where i implement the methods, which define the arithmetic operations to test out the Source.cpp
```cpp 
#include "Fraction.h"

Fraction::Fraction() : m_num(0), m_den(1) {}

Fraction::Fraction(int num, int den) : m_num(num), m_den(den) {}

Fraction Fraction::Add(const Fraction& rhs) const
{
    return Fraction(m_num * rhs.m_den + rhs.m_num * m_den,
        m_den * rhs.m_den);
}

Fraction Fraction::Subtract(const Fraction& rhs) const
{
    return Fraction(m_num * rhs.m_den - rhs.m_num * m_den,
        m_den * rhs.m_den);
}

Fraction Fraction::Multiply(int scale) const
{
    return Fraction(m_num * scale, m_den);
}

Fraction Fraction::Divide(int scale) const
{
    return Fraction(m_num, m_den * scale);
}

int Fraction::Num() const { return m_num; }
int Fraction::Den() const { return m_den; }

void Fraction::Num(int num) { m_num = num; }
void Fraction::Den(int den) { m_den = den; }

void Fraction::Write(std::ostream& sout) const
{
    sout << m_num << "/" << m_den;
}

void Fraction::Read(std::istream& sin) 
{
    sin >> m_num >> m_den;
}
```