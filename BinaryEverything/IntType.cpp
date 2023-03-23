#include "IntType.h"

using namespace SGET;

const std::array<char, 36> IntType::baseDigit = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

IntType::IntType(size_t size, bool isSigned)
	: rawData(size, false), isSigned(isSigned)
{}

IntType::IntType(unsigned long long number, size_t size)
	: rawData(std::vector<uint8_t>(size, false)), isSigned(false)
{
	for (size_t i = 0; i < size; i++)
	{
		bool bit = number & 0b1;
		rawData[size - i - 1] = bit;
		number >>= 1;
	}
}

IntType::IntType(long long number, size_t size)
	: rawData(size, false), isSigned(true)
{
	for (size_t i = 0; i < size; i++)
	{
		bool bit = number & 0b1;
		rawData[size - i - 1] = bit;
		number >>= 1;
	}
}

IntType::IntType(std::string number, size_t size, bool isSigned)
	: rawData(size, false), isSigned(isSigned)
{
	bool isNeg = number[0] == '-';

	size_t minSize = std::min(size, number.size() - (isNeg ? 1 : 0));
	IntType ten(size, false);
	ten[ten.size() - 2] = 0b1;
	ten[ten.size() - 4] = 0b1;

	IntType base(size, false);
	base[base.size() - 1] = 0b1;
	
	for (size_t i = 0; i < minSize; i++)
	{
		IntType digit = digitToIntType(number[number.size() - i - 1], size);
		*this += base * digit;
		base *= ten;
	}
	if (isNeg)
	{
		*this = -*this;
	}
}

void SGET::IntType::operator+=(const IntType& a)
{
	*this = *this + a;
}

void SGET::IntType::operator-=(const IntType& a)
{
	*this = *this - a;
}

void SGET::IntType::operator*=(const IntType& a)
{
	*this = *this * a;
}

std::pair<IntType, IntType> SGET::IntType::divide(const IntType& a) const
{
	bool negate = false;
	IntType Q(this->size(), true); //Divident
	IntType M(a.size(), true); //Divisor
	IntType A(this->size(), true);

	auto SLAQ = [&A, &Q]()
	{
		for (size_t i = 1; i < A.size(); i++)
		{
			A[i - 1] = A[i];
		}
		A[A.size() - 1] = Q[0];
		for (size_t i = 1; i < Q.size(); i++)
		{
			Q[i - 1] = Q[i];
		}
		Q[Q.size() - 1] = 0;
	};

	for (size_t i = 0; i < Q.size(); i++)
	{
		Q[i] = (*this)[i];
	}

	for (size_t i = 0; i < M.size(); i++)
	{
		M[i] = a[i];
	}

	if (M[0] ^ Q[0])
	{
		negate = true;
	}

	if (M[0])
	{
		M = -M;
	}
	if (Q[0])
	{
		Q = -Q;
	}

	for (size_t i = 0; i < this->size(); i++)
	{
		SLAQ();
		if (A[0])
		{
			A += M;
		}
		else
		{
			A -= M;
		}
		Q[Q.size() - 1] = A[0] ^ 0b1;
	}
	if (A[0])
	{
		A += M;
	}
	if (negate)
	{
		Q = -Q;
		A = -A;
	}

	return std::make_pair(Q, A);
}

void SGET::IntType::operator<<=(size_t size)
{
	*this = *this << 1;
}

void SGET::IntType::operator>>=(size_t size)
{
	*this = *this >> size;
}

const uint8_t& SGET::IntType::operator[](size_t size) const
{
	return rawData[size];
}

uint8_t& SGET::IntType::operator[](size_t size)
{
	return rawData[size];
}

size_t SGET::IntType::size() const
{
	return rawData.size();
}

bool SGET::IntType::isZero(const IntType& a)
{
	for (size_t i = 0; i < a.size(); i++)
	{
		if (a[i] != 0) return false;
	}
	return true;
}

IntType SGET::IntType::digitToIntType(char c, size_t size)
{
	IntType ans(size, false);
	switch (c)
	{
	case '0':
		break;
	case '1':
		ans[ans.size() - 1] = 0b1;
		break;
	case '2':
		ans[ans.size() - 2] = 0b1;
		break;
	case '3':
		ans[ans.size() - 1] = 0b1;
		ans[ans.size() - 2] = 0b1;
		break;
	case '4':
		ans[ans.size() - 3] = 0b1;
		break;
	case '5':
		ans[ans.size() - 1] = 0b1;
		ans[ans.size() - 3] = 0b1;
		break;
	case '6':
		ans[ans.size() - 2] = 0b1;
		ans[ans.size() - 3] = 0b1;
		break;
	case '7':
		ans[ans.size() - 1] = 0b1;
		ans[ans.size() - 2] = 0b1;
		ans[ans.size() - 3] = 0b1;
		break;
	case '8':
		ans[ans.size() - 4] = 0b1;
		break;
	case '9':
		ans[ans.size() - 1] = 0b1;
		ans[ans.size() - 4] = 0b1;
		break;
	default:
		__debugbreak();
	}
	return ans;
}

char SGET::IntType::IntTypeToDigit(const IntType& a)
{
	size_t num = 0;
	size_t base = 1;
	for (size_t i = 0; i < a.size(); i++)
	{
		num += base * a[a.size() - i - 1];
		base *= 2;
	}
	return baseDigit[num];
}

std::string SGET::IntType::asString(IntType a, size_t base)
{
	std::string ans = "";
	IntType baseIntType((long long)base, 8);
	while (!isZero(a))
	{
		auto [Quotient, Remainder] = a.divide(baseIntType);
		ans += IntTypeToDigit(Remainder);
		a = Quotient;
	}
	if (ans == "")
	{
		return "0";
	}
	else
	{
		std::reverse(ans.begin(), ans.end());
		return ans;
	}
}

int SGET::IntType::msb(const IntType& a)
{
	for (size_t i = 0; i < a.size(); i++)
	{
		if (a[i] == 0b1)
		{
			return a.size() - i - 1;
		}
	}
	return -1;
}

IntType SGET::operator+(const IntType& a, const IntType& b)
{
	size_t maxSize = std::max(a.size(), b.size());
	SGET::IntType ans(maxSize, true);
	bool carry = false;
	for (size_t i = 0; i < maxSize; i++)
	{
		uint8_t currentAValue = ((a.size() > i) ? a[a.size() - i - 1] : (a.isSigned ? a[0] : 0b0));
		uint8_t currentBValue = ((b.size() > i) ? b[b.size() - i - 1] : (b.isSigned ? b[0] : 0b0));
		ans[ans.size() - i - 1] = carry ^ currentAValue ^ currentBValue;
		carry = carry & currentAValue | (carry ^ currentAValue) & currentBValue;
	}
	return ans;
}

IntType SGET::operator-(const IntType& a)
{
	IntType ans(a.size(), true);
	for (size_t i = 0; i < a.size(); i++)
	{
		ans[i] = a[i] ^ 0b1; // ans = ~a
	}
	IntType one(a.size(), true);
	one[one.size() - 1] = 1;
	ans += one;
	return ans;
}

IntType SGET::operator-(const IntType& a, const IntType& b)
{
	return a + (-b);
}

IntType SGET::operator*(const IntType& a, const IntType& b)
{
	size_t totalSize = a.size() + b.size();
	IntType M(totalSize, a.isSigned);
	IntType Q(totalSize, b.isSigned);
	uint8_t q0 = 0b0;
	IntType A(totalSize, true);

	for (size_t i = 0; i < totalSize; i++)
	{
		M[totalSize - i - 1] = a.size() > i ? a[a.size() - i - 1] : (a.isSigned ? a[0] : 0b0);
		Q[totalSize - i - 1] = b.size() > i ? b[b.size() - i - 1] : (b.isSigned ? b[0] : 0b0);
	}

	for (size_t n = 0; n < totalSize; n++)
	{
		if (Q[totalSize - 1] == 1 && q0 == 0)
		{
			A -= M;
		}
		else if (Q[totalSize - 1] == 0 && q0 == 1)
		{
			A += M;
		}
		q0 = Q[totalSize - 1];
		Q >>= 1;
		Q[0] = A[totalSize - 1];
		A >>= 1;                       // (AQq0 >> 1)
		A[0] = A[1];
	}

	IntType ans(IntType::msb(a) + 1 + IntType::msb(b) + 2, a.isSigned || b.isSigned);
	for (size_t i = 0; i < ans.size(); i++)
	{
		ans[ans.size() - i - 1] = Q[Q.size() - i - 1];
	}
	return ans;
}

IntType SGET::operator<<(const IntType& a, size_t size)
{
	SGET::IntType ans = IntType(a.size(), false);
	for (size_t i = 0; i + size < a.size(); i++)
	{
		ans[i] = a[i + size];
	}

	return ans;
}

IntType SGET::operator>>(const IntType& a, size_t size)
{
	SGET::IntType ans = IntType(a.size(), false);
	for (size_t i = size; i < a.size(); i++)
	{
		ans[i] = a.rawData[i - size];
	}
	return ans;
}
