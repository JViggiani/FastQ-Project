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
	CmdWriteFragment(const string& aFileName, const string& aOutputFolder)
	{
		this->_outputFile = aFileName;
		this->_outputFolder = aOutputFolder;
 	}

	CmdWriteFragment() = default;
	
	std::ofstream& getStream()
	{
		return _oStream;
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

	void lockFragmentFile(const string& _outputFolder, const int& aFolderNum, const string& aFile1);
	void unlockFragmentFile(const string& _outputFolder, const int& aFolderNum);

	void printToEndOfFile(CmdReadFragment& aFileToRead);

private:
	string _outputFile;
	string _outputFolder;
	std::ofstream _oStream;
};

