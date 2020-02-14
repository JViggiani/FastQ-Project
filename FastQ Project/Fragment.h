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
		return Quality(_qualityValue);
	}
};

struct FragmentPair
{
	Fragment* _fragment1;
	Fragment* _fragment2;

	bool operator<(const FragmentPair &input) 
	{
		//calculate this average
		Quality thisAverageQuality = (this->_fragment1->calculateQuality() + this->_fragment2->calculateQuality()) / 2;

		//calculate that average
		Quality thatAverageQuality = (input._fragment1->calculateQuality() + input._fragment2->calculateQuality()) / 2;

		return (thisAverageQuality < thatAverageQuality);
	}
};