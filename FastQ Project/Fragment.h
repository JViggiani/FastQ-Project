#pragma once

#include "Quality.h"

struct Fragment
{
	char *_seqId;
	char *_rawSequence;
	//remember to append a + to line 3
	char *_qualityValue;

	Quality calculateQuality() 
	{
		Quality aQuality(_qualityValue);
		return aQuality;
	}
};

struct FragmentPair
{
	Fragment* _fragment1;
	Fragment* _fragment2;

	Quality calculateAverageQuality() const
	{
		return (this->_fragment1->calculateQuality() + this->_fragment2->calculateQuality()) / 2;
	}

	bool operator<(const FragmentPair input)
	{
		return (this->calculateAverageQuality() < input.calculateAverageQuality());
	}
};

class CmdProcessFragment
{
public:
	CmdProcessFragment(string aFile)
	{
		_inputStream.open(aFile);
	}

	bool eof()
	{
		return _inputStream.eof();
	}

	void close()
	{
		_inputStream.close();
	}

	Fragment* populateNextFragment()
	{
		Fragment* aFragment = new Fragment();
		int aFragmentLineNum = 1;
		for (string line;
			aFragmentLineNum <= 4 && getline(_inputStream, line) && !_inputStream.eof();
			++aFragmentLineNum)
		{
			cout << line << "\n";

			//File may contain blank lines, added for some resilience
			if (line.size() == 0)
			{
				--aFragmentLineNum;
				continue;
			}

			//JOSH need to NULL terminate your char arrays...

			switch (aFragmentLineNum)
			{
			case 1:
				aFragment->_seqId = new char[line.length() + 1];
				strcpy(aFragment->_seqId, line.c_str());
				break;
			case 2:
				aFragment->_rawSequence = new char[line.length() + 1];
				strcpy(aFragment->_rawSequence, line.c_str());
				break;
			case 3:
				break;
			case 4:
				aFragment->_qualityValue = new char[line.length() + 1];
				strcpy(aFragment->_qualityValue, line.c_str());
				break;
			}
		}

		if (aFragment && aFragment->_seqId && aFragment->_rawSequence && aFragment->_qualityValue)
		{
			//Only want to return complete fragments
			return aFragment;
		}
		else return nullptr;
	}

private:
	std::ifstream _inputStream;
};