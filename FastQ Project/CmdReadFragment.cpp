#include "pch.h"
#include "CmdReadFragment.h"

void CmdReadFragment::populateNextFragment(std::unique_ptr<Fragment>& aFragment)
{
	aFragment.reset(new Fragment());
	int aFragmentLineNum = 1;
	for (string line;
		aFragmentLineNum <= 4 && getline(_iStream, line) && !_iStream.eof();
		++aFragmentLineNum)
	{
		//cout << line << "\n";

		//File may contain blank lines, added for some resilience
		if (line.size() == 0)
		{
			--aFragmentLineNum;
			continue;
		}

		switch (aFragmentLineNum)
		{
		case 1:
			aFragment->_seqId.reset(new char[line.length() + 1]);
			strcpy(aFragment->_seqId.get(), line.c_str());
			break;
		case 2:
			aFragment->_rawSequence.reset(new char[line.length() + 1]);
			strcpy(aFragment->_rawSequence.get(), line.c_str());
			break;
		case 3:
			break;
		case 4:
			aFragment->_qualityValue.reset(new char[line.length() + 1]);
			strcpy(aFragment->_qualityValue.get(), line.c_str());
			break;
		}
	}

	if (!aFragment || !aFragment->_seqId || !aFragment->_rawSequence || !aFragment->_qualityValue)
	{
		//Only want to return complete fragments
		aFragment.reset(nullptr);
	}
}

void CmdReadFragment::populateNextFragmentPair(std::unique_ptr<FragmentPair>& aFragmentPairPtr)
{
	std::unique_ptr<Fragment> aFragment1;
	populateNextFragment(aFragment1);
	std::unique_ptr<Fragment> aFragment2;
	populateNextFragment(aFragment2);

	if (aFragment1 && aFragment2)
	{
		aFragmentPairPtr.reset(new FragmentPair()); //= make_unique<FragmentPair>(new FragmentPair());

		aFragmentPairPtr->_fragment1 = std::move(aFragment1); //JOSH a better way?
		aFragmentPairPtr->_fragment2 = std::move(aFragment2);

		//return aFragmentPair;
	}
	else
	{
		string aFollowingLine;
		getline(_iStream, aFollowingLine);
		if (aFollowingLine == "\n")
		{
			//get rid of new lines
			populateNextFragmentPair(aFragmentPairPtr);
		}
		else
		{
			aFragmentPairPtr.get() == nullptr;
		}
	}

}
