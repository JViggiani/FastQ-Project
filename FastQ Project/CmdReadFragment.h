#pragma once

#include "pch.h"
#include "Fragment.h"
#include "Quality.h"

#include "CmdProcessFragment.h"

#include <iostream>
#include <fstream>

class CmdReadFragment : public CmdProcessFragment
{
public:

	std::ifstream& getStream()
	{
		return _iStream;
	}

	bool isFragmentFileLocked(const string& _outputFolder, const int& aFolderNum, const string& aFile1);

	void populateNextFragment(std::unique_ptr<Fragment>& aFragmentPtr);

	void populateNextFragmentPair(std::unique_ptr<FragmentPair>& aFragmentPairPtr);

private:
	std::ifstream _iStream;
};