#include "pch.h"
#include "CmdReadFragment.h"

std::shared_ptr<Fragment> CmdReadFragment::populateNextFragment()
{
	std::shared_ptr<Fragment> aFragment(new Fragment());
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
			aFragment->_seqId = std::shared_ptr<char>(new char[line.length() + 1]);
			strcpy(aFragment->_seqId.get(), line.c_str());
			break;
		case 2:
			aFragment->_rawSequence = std::shared_ptr<char>(new char[line.length() + 1]);
			strcpy(aFragment->_rawSequence.get(), line.c_str());
			break;
		case 3:
			break;
		case 4:
			aFragment->_qualityValue = std::shared_ptr<char>(new char[line.length() + 1]);
			strcpy(aFragment->_qualityValue.get(), line.c_str());
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

std::shared_ptr<FragmentPair> CmdReadFragment::populateNextFragmentPair()
{
	std::shared_ptr<Fragment> aFragment1 = this->populateNextFragment();
	std::shared_ptr<Fragment> aFragment2 = this->populateNextFragment();

	if (aFragment1 && aFragment2)
	{
		std::shared_ptr<FragmentPair> aFragmentPair(new FragmentPair());

		aFragmentPair->_fragment1 = std::move(aFragment1); //JOSH a better way?
		aFragmentPair->_fragment2 = std::move(aFragment2);

		return aFragmentPair;
	}
	else
	{
		string aFollowingLine;
		getline(_iStream, aFollowingLine);
		if (aFollowingLine == "\n")
		{
			//get rid of new lines
			return populateNextFragmentPair();
		}
		else return nullptr;
	}

}
