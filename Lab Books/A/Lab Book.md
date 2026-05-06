# Lab A
## Q3 - add code, from Hello World progam

```cpp
#include <iostream>

int main(int argc, char** argv)
{
    std::cout << "Fundamental type sizes (in bytes):\n";

    // Character types
    std::cout << "char:               " << sizeof(char) << "\n";

    // Signed integer types
    std::cout << "signed char:        " << sizeof(signed char) << "\n";
    std::cout << "short:              " << sizeof(short) << "\n";
    std::cout << "int:                " << sizeof(int) << "\n";
    std::cout << "long:               " << sizeof(long) << "\n";
    std::cout << "long long:          " << sizeof(long long) << "\n";

    // Unsigned integer types
    std::cout << "unsigned char:      " << sizeof(unsigned char) << "\n";
    std::cout << "unsigned short:     " << sizeof(unsigned short) << "\n";
    std::cout << "unsigned int:       " << sizeof(unsigned int) << "\n";
    std::cout << "unsigned long:      " << sizeof(unsigned long) << "\n";
    std::cout << "unsigned long long: " << sizeof(unsigned long long) << "\n";

    // Floating‑point types
    std::cout << "float:              " << sizeof(float) << "\n";
    std::cout << "double:             " << sizeof(double) << "\n";
    std::cout << "long double:        " << sizeof(long double) << "\n";

    // Pointer size (depends on 32‑bit vs 64‑bit)
    std::cout << "void*:              " << sizeof(void*) << "\n";

    return 0;
}
```

## Reflection:
- char, short, int, long, and long long all have different sizes.
- sizeof(void*) tells you whether you're on a 32‑bit or 64‑bit system.

# Q4  Hello world program continued

```cpp
    //double x = 10.0;
    //double y = 10.0;
    //if (x == y)
    //    std::cout << "X and Y are identical" << std::endl;

    const double x = 100000.123456789;
    const double a = 200000.123456789;
    double y = (x + a) / x;
    double z = 1.0 + (a / x);
    //double z = x / y;
    if (y == z)
        std::cout << "X and Y are identical" << std::endl;


    return 0;
}
```

## Reflection
- Floating‑point numbers cannot represent most real numbers exactly.
- Operations that should be mathematically identical can produce different results.
- Division by zero only occurs when the denominator is exactly zero, not merely tiny.
- The value of x affects rounding error, but not the possibility of divide‑by‑zero.
- Comparing floating‑point values with == is unreliable; you should use a tolerance (epsilon) instead.

# Q5 - Iteration / similarities between c# & c++
```cpp
#include <iostream> // required for std::cout and std::endl - general input/output stream objects and being able to interact with the console

int main() // changed from static void main(string[] args)
{
	//int factorialNumber = 5;
	//int factorialTotal = 1;

	//for (int i = 2; i <= factorialNumber; i++)
	//{
	//	factorialTotal *= i;
	//}

	//std::cout << "Factorial of " << factorialNumber << " is " << factorialTotal << std::endl;
	//return 0; commit and pushing to github 
}
```
## Reflection
This is very similar to C#'s syntax for the most part, 
with some differences like the use of 'static void main' 
instead of 'static void Main' and 'string[] args' instead of 'string[] args', as well as std:cout, the inclusion of libraries at the top of the file for input/output. But for the most part they are similar. 

# Q6 

```cpp
	int sum = 0;
	int count = 0;

	while (true)
	{	
		std::cout << "Enter a number (0 to stop): ";
		int n;
		std::cin >> n;

		if (n == 0)
			break;

		sum += n;
		count++;
	}

	if (count > 0)
		std::cout << "Average = " << static_cast<double>(sum) / count << std::endl; //static cast<double> is used to 
	// convert sum to a double to ensure floating-point division
	else
		std::cout << "No numbers entered." << std::endl;

	return 0; // fairly similar to C#. 
```
## Reflection

This is almost the same as C#, with the exclusion of ```cpp static-cast<double>``` and the input, output synatx.


