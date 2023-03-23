#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <array>

namespace SGET
{
	class IntType
	{
	public:
		IntType(size_t size, bool isSigned);
		IntType(unsigned long long number, size_t size);
		IntType(long long number, size_t size);
		IntType(std::string number, size_t size, bool isSigned);

		friend IntType operator+(const IntType& a, const IntType& b);
		void operator+=(const IntType& a);

		friend IntType operator-(const IntType& a);
		friend IntType operator-(const IntType& a, const IntType& b);
		void operator-=(const IntType& a);

		friend IntType operator*(const IntType& a, const IntType& b);
		void operator*=(const IntType& a);

		std::pair<IntType, IntType> divide(const IntType& a) const;

		friend IntType operator<<(const IntType& a, size_t size);
		void operator<<=(size_t size);
		
		friend IntType operator>>(const IntType& a, size_t size);
		void operator>>=(size_t size);
		
		const uint8_t& operator[](size_t size) const;
		uint8_t& operator[](size_t size);

		size_t size() const;

		static bool isZero(const IntType& a);

		static std::string asString(IntType a, size_t base = 10);

		static int msb(const IntType& a);

	private:
		static IntType digitToIntType(char c, size_t size);
		static char IntTypeToDigit(const IntType& a);
		static const std::array<char, 36> baseDigit;
		std::vector<uint8_t> rawData;
		bool isSigned;
	};
}