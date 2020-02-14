#pragma once

#include <iostream>
#include "pch.h"

//JOSH go through these and cull the unused functions

class Quality 
{
private:
	char* _integer;
public:
	/* Constructs a Quality representation of the integer given as an argument */
	Quality(unsigned int integer);

	/* Parses the string given as an argument looking for an integer.
	 * Stops as soon as it finds a non-digit character.
	 * Trailing zeros will eventually get removed.
	 */
	Quality(const char* integer);

	Quality(const string& integer);

	/* Constructs a Quality representation of the integer given as an argument
	 * and assigns it to the internal representation of the Quality.
	 */
	//void setInteger(unsigned int integer);

	/* Parses the string given as an argument looking for an integer and assigns it to
	 * the internal representation of the Quality.
	 * Stops as soon as it finds a non-digit character.
	 * Trailing zeros will eventually get removed.
	 */
	void setInteger(const char* integer);

	/* Returns 0 if the value of the internal Quality won't fit in 32 bits.
	 * Otherwise it returns the integer values.
	 */
	//unsigned int getIntValue() const;

	/* Returns the internal Quality as a char array */
	char* toCharArray() const;
	/* Returns the internal Quality as a string */
	string toString() const;

	/* Adds the Quality given as an argument to the internal Quality
	 * and returns the result as a string.
	 */
	Quality addInteger(const Quality& integer_to_add) const;

	/* Adds the integer represented by the string given as an argument to the internal
	 * Quality and returns the result as a string.
	 */
	Quality addInteger(const char* integer_to_add) const;

	/* Multiplies the Quality given as an argument by the internal Quality
	 * and returns the result as a string.
	 */
	Quality multiplyInteger(const Quality& integer_to_multiply) const;

	/* Multiplies the integer represented by the string given as an argument by the internal
	 * Quality and returns the result as a string.
	 */
	Quality multiplyInteger(const char* integer_to_multiply) const;

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