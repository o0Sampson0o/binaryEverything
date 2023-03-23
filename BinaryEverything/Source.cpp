#include <iostream>
#include "SyntaxTree.h";
#include "IntType.h"
#include <vector>
#include <math.h>
bool isNum(const std::string& str);

int main()
{
	SGET::IntType a(0ull, 0ull);
	SGET::IntType b(0ull, 0ull);
	std::string s;
	std::cin >> s;
	if (isNum(s))
	{
		a = SGET::IntType(s, size_t(s.length() * log2(10) + 1), false);
	}
	std::cin >> s;
	if (isNum(s))
	{
		b = SGET::IntType(s, size_t(s.length() * log2(10) + 1), false);
	}
	SGET::IntType c = a * b;
	std::cout << SGET::IntType::asString(c);
}

bool isNum(const std::string& str)
{
	bool periodExist = false;
	bool negExist = false;
	for (char c : str)
	{
		if (c == '.')
		{
			if (periodExist)
			{
				return false;
			}
			periodExist = true;
			continue;
		}

		if (c == '-')
		{
			if (negExist)
			{
				return false;
			}
			negExist = true;
			continue;
		}

		if (c < '0' || c > '9')
		{
			return false;
		}
	}

	if (negExist && str[0] != '-')
	{
		return false;
	}
	return true;
}