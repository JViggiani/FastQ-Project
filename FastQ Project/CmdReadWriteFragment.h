#pragma once

#include "pch.h"
#include "CmdReadFragment.h"
#include "Fragment.h"
#include "Quality.h"

#include <iostream>
#include <fstream>

class CmdReadWriteFragment : public CmdReadFragment
{
public:
	CmdReadWriteFragment(const string& aFile)
	{
		FILE *file;
		int file_exists;

		file = fopen(aFile.c_str(), "r");
		if (file == NULL)
		{
			file = fopen(aFile.c_str(), "w+b");
		}
		fclose(file);

		file = fopen("data/temp.fastq", "r");
		if (file != NULL)
		{
			remove("data/temp.fastq");
		}

		delete file;
	}
	
	std::ofstream& getStream()
	{
		return _ioStream;
	}

	void printDataSetToFile(const std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor>& aSetToPrint);
	void printDataItemToFile(const std::shared_ptr<FragmentPair>& item);

	void printFragmentPairToFileLowMemory(const std::shared_ptr<FragmentPair>& aFragmentPair);

private:
	std::ofstream _ioStream;
};

