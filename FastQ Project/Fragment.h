#pragma once

#include "Quality.h"

struct Fragment
{
	//char *_seqId;
	std::unique_ptr<char> _seqId;
	std::unique_ptr<char> _rawSequence;
	//remember to append a + to line 3
	std::unique_ptr<char> _qualityValue;

	Quality calculateQuality() 
	{
		Quality aQuality(_qualityValue.get());
		return aQuality;
	}
};

struct FragmentPair
{
	std::unique_ptr<Fragment> _fragment1;
	std::unique_ptr<Fragment> _fragment2;

	Quality calculateAverageQuality() const
	{
		return (this->_fragment1->calculateQuality() + this->_fragment2->calculateQuality()) / 2;
	}

	bool operator<(const FragmentPair& input)
	{
		return (this->calculateAverageQuality() < input.calculateAverageQuality());
	}

	bool operator==(const FragmentPair& input)
	{
		return (this->calculateAverageQuality() == input.calculateAverageQuality());
	}

	bool operator>(const FragmentPair& input)
	{
		return (this->calculateAverageQuality() > input.calculateAverageQuality());
	}
};

struct FragmentPairComparitor
{
	bool operator()(const unique_ptr<FragmentPair>& a, const unique_ptr<FragmentPair>& b)
	{
		return *(a.get()) > *(b.get());
	}
	//auto comparitor = [](FragmentPair* a, FragmentPair* b) { return *a > *b; };
};