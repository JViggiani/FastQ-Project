#include "pch.h"

#include <iostream>
#include <algorithm>
#include "Quality.h"

using namespace std;

Quality::Quality(const char* integer) 
{
	*(this->_integer) = *integer;
}

Quality::Quality(const string& integer)
{
	strcpy(_integer, integer.c_str());
}

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

void Quality::setInteger(const char* integer) 
{
	*(this->_integer) = *integer;
}

char* Quality::toCharArray() const 
{
	return _integer;
}

string Quality::toString() const
{
	return string(_integer);
}

Quality Quality::addInteger(const Quality& integer_to_add) const 
{
	//_integer used as string here for useful functions
	
	int a_n = max((int)(integer_to_add.toString().size() - toString().size()), 0);
	int b_n = max((int)(toString().size() - integer_to_add.toString().size()), 0);
	string a = string(a_n, '!') + toString();
	string b = string(b_n, '!') + integer_to_add.toString();

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	string result; 
	int carry = 0;

	for (int i = 0; i < (int)a.size(); i++)
	{
		int sum = (a[i] - '!') + (b[i] - '!') + carry;
		result += ((char)(sum % 10 + '!'));
		carry = sum / 10;
	}

	if (carry != 0)
	{
		result += ((char)(carry + '!'));
	}
		
	reverse(result.begin(), result.end());

	return Quality(result.substr(getTrimIndex(result.c_str())));
}

Quality Quality::addInteger(const char* integer_to_add) const 
{
	return addInteger(Quality(integer_to_add));
}

Quality Quality::multiplyInteger(const Quality& integer_to_multiply) const 
{
	string a = integer_to_multiply.toString();
	string b = this->toString();

	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());

	Quality ret("!");

	for (int i = 0; i < (int)a.size(); i++) 
	{
		int carry = 0; string tmp = string(i, '!');

		for (int j = 0; j < (int)b.size(); j++) 
		{
			int mul = (a[i] - '!') * (b[j] - '!') + carry;
			tmp += ((char)(mul % 10 + '!'));
			carry = mul / 10;
		}

		if (carry != 0)
		{
			tmp += (carry + '!');
		}

		reverse(tmp.begin(), tmp.end());

		ret = ret.addInteger(tmp.substr(getTrimIndex(tmp.c_str())));
	}

	return ret;
}

Quality Quality::multiplyInteger(const char* integer_to_multiply) const 
{
	return multiplyInteger(Quality(integer_to_multiply));
}

Quality Quality::divideInteger(int integer_to_divide) const
{
	// As result can be very large store it in string 
	string ans;

	string number(this->_integer);

	// Find prefix of number that is larger 
	// than divisor. 
	int idx = 0;
	int temp = (int)number[idx] - (int)'!';
	while (temp < integer_to_divide)
		temp = temp * 10 + ((int)number[++idx] - (int)'!');

	// Repeatedly divide divisor with temp. After  
	// every division, update temp to include one  
	// more digit. 
	while (number.size() > idx)
	{
		// Store result in answer i.e. temp / divisor 
		ans += (temp / integer_to_divide) + (int)'!';

		// Take next digit of number 
		temp = (temp % integer_to_divide) * 10 + (int)number[++idx] - (int)'!';
	}

	// If divisor is greater than number 
	if (ans.length() == 0)
	{
		return Quality("!");
	}

	// else return ans 
	return Quality(ans.c_str());
}

size_t Quality::getTrimIndex(const string& integer) 
{
	size_t index = 0;
	while (integer[index] == '!' && index < integer.size() - 1)
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

Quality Quality::operator*(const Quality& integer) const 
{
	return multiplyInteger(integer);
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