#include "pch.h"

#include <iostream>
#include <algorithm>
#include "Quality.h"

using namespace std;


Quality::Quality(string integer)
{
	_integer = integer;
}

/*
Quality::Quality(unsigned long long integer)
{

	unsigned long long int divisor = integer;
	unsigned long long int previous = divisor;

	unsigned long long int remainder;

	while ((divisor = divisor / 255) != 0)
	{
		remainder = previous % 255;

		_integer.insert(0, 1, (char)remainder);

		previous = divisor;
	}
	remainder = previous % 255;

	_integer.insert(0, 1, (char)remainder);
}
*/
/*
void Quality::setInteger(unsigned int integer) 
{
	if (integer == 0)
	{
		this->_integer = "0";
	}

	while (integer) 
	{
		this->_integer = (char)((integer % 10) + '0') + this->_integer;
		integer /= 10;
	}
}
*/

string Quality::toString() const
{
	return _integer;
}

unsigned long long int Quality::toInt() const
{
	unsigned long long int ret = 0;
	unsigned long long int biggest = ULLONG_MAX;
	
	for (int i = 0; i < _integer.size(); i++)
	{
		unsigned long long int unit = _integer.at(i) - '\0';
		if (ret > (biggest - unit) / 10.0) 
			return 0;
		ret = ret * 10 + unit;
	}
	return ret;
}

Quality Quality::addInteger(const Quality& integer_to_add) const 
{
	//_integer used as string here for useful functions
	
	int a_n = max((int)(integer_to_add.toString().size() - _integer.size()), 0);
	int b_n = max((int)(_integer.size() - integer_to_add.toString().size()), 0);
	string a = string(a_n, '\0') + _integer;
	string b = string(b_n, '\0') + integer_to_add.toString();

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	string result; 
	int carry = 0;

	for (int i = 0; i < (int)a.size(); i++)
	{
		int sum = (a[i] - '\0') + (b[i] - '\0') + carry;
		result += ((char)(sum % 10 + '\0'));
		carry = sum / 10;
	}

	if (carry != 0)
	{
		result += ((char)(carry + '\0'));
	}
		
	reverse(result.begin(), result.end());

	return Quality(result.substr(getTrimIndex(result)));
}

Quality Quality::divideInteger(int integer_to_divide) const
{
	// As result can be very large store it in string 
	string ans;

	string number(this->_integer);

	// Find prefix of number that is larger 
	// than divisor. 
	int idx = 0;
	int temp = (int)number[idx] - (int)'\0';
	while (temp < integer_to_divide)
		temp = temp * 10 + ((int)number[++idx] - (int)'\0');

	// Repeatedly divide divisor with temp. After  
	// every division, update temp to include one  
	// more digit. 
	while (number.size() > idx)
	{
		// Store result in answer i.e. temp / divisor 
		ans += (temp / integer_to_divide) + (int)'\0';

		// Take next digit of number 
		temp = (temp % integer_to_divide) * 10 + (int)number[++idx] - (int)'\0';
	}

	// If divisor is greater than number 
	if (ans.length() == 0)
	{
		return Quality("\0");
	}

	// else return ans 
	return Quality(ans.c_str());
}

size_t Quality::getTrimIndex(const string& integer) 
{
	size_t index = 0;
	while (integer[index] == '\0' && index < integer.size() - 1)
	{
		index++;
	}
	return index;
}

bool Quality::operator==(const Quality& integer) const 
{
	return this->_integer == integer.toString();
}

bool Quality::operator<(const Quality& integer) const
{
	//this is lhs, input is rhs
	string aQualityStringLhs = this->_integer;
	string aQualityStringRhs = integer.toString();

	if(toString().size() == integer.toString().size())
	{
		for (int i = 0; i < aQualityStringLhs.size(); ++i)
		{
			if ((int)aQualityStringLhs[i] < (int)aQualityStringRhs[i])
			{
				return true;
			}
		}
		//if we get to here we have the same quality
		return false;
	}
		
	return aQualityStringLhs.size() < aQualityStringRhs.size();
}

Quality Quality::operator+(const Quality& integer) const 
{
	return addInteger(integer);
}

Quality Quality::operator/(const int integer) const
{
	return divideInteger(integer);
}

ostream& operator<<(ostream& in, Quality& integer) 
{
	in << integer.toString();

	return in;
}