#pragma once

#include <iostream>
#include "pch.h"

//JOSH go through these and cull the unused functions

class Quality 
{
private:
	string _integer;
public:

	Quality(string integer);

	/* Returns the internal Quality as a string */
	string toString() const;
	/* Returns the internal Quality as an int. Returns 0 if it is too big to return */
	unsigned long long int toInt() const;

	/* Adds the Quality given as an argument to the internal Quality
	 * and returns the result as a string.
	 */
	Quality addInteger(const Quality& integer_to_add) const;

	/* Multiplies the Quality given as an argument by the internal Quality
	 * and returns the result as a string.
	 */
	Quality multiplyInteger(const Quality& integer_to_multiply) const;

	/* Multiplies the integer represented by the string given as an argument by the internal
	 * Quality and returns the result as a string.
	 */
	Quality multiplyInteger(const string& integer_to_multiply) const;

	Quality divideInteger(int integer_to_divide) const;

	/* Returns the index of the first non-zero digit in the string given as an argument.
	 * This function is used to trim trailing zeros from a string representation of an integer.
	 * A trimmed version of the string is a substring that starts at the index returned by this
	 * function.
	 */
	static size_t getTrimIndex(const string& integer);

	bool operator==(const Quality& integer) const;

	bool operator<(const Quality& integer) const;

	Quality operator+(const Quality& integer) const;

	Quality operator*(const Quality& integer) const;

	Quality operator/(const int integer) const;

	friend ostream& operator<<(ostream& in, Quality& integer);
};