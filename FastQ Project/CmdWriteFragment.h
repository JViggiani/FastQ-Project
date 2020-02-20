#pragma once

#include "pch.h"
#include "CmdReadFragment.h"
#include "Fragment.h"
#include "Quality.h"

#include <iostream>
#include <fstream>

class CmdWriteFragment : public CmdReadFragment
{
public:
	CmdWriteFragment(const string& aFileName)
	{
		this->_aOutputFileName = aFileName;
	}
	
	std::ofstream& getStream()
	{
		return _ioStream;
	}

	//! Deletes old data from prior run outputs with the provided file name
	void initialiseFileOutput();

	//! Prints a multiset of fragment pairs to the currently opened file
	void printDataSetToFile(const std::multiset<std::unique_ptr<FragmentPair>, FragmentPairComparitor>& aSetToPrint);
	//! Prints an individual fragment pair to the end of the currently opened file
	void printFragmentPairToEndOfFile(const std::unique_ptr<FragmentPair>& item);
	//! Prints an individual fragment pair in the correct ordered place to the currently opened file
	/*
		O(n^2) time complexity: Bubble sort algorithm used
	*/
	void printAndOrderFragmentPairToFile(const std::unique_ptr<FragmentPair>& aFragmentPair);

private:
	string _aOutputFileName;
	std::ofstream _ioStream;
};

