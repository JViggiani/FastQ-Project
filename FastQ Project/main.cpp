#include "pch.h"

#include "Fragment.h"
#include "CmdReadFragment.h"
#include "CmdWriteFragment.h"
#include "CmdMergeSort.h"
#include "FileThread.h"

#include <filesystem>
#include <stdio.h>
#include <time.h>

void fast(const string& aInput)
{
	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aInput.c_str());

	std::multiset<std::unique_ptr<FragmentPair>, FragmentPairComparitor> aFragmentPairSet;

	while (!aFragmentReader.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aFragmentPair;
		aFragmentReader.populateNextFragmentPair(aFragmentPair);
		if (aFragmentPair)
		{
			aFragmentPairSet.insert(std::move(aFragmentPair)); //JOSH can we change this from a move? probably not
		}
	}
	aFragmentReader.getStream().close();

	string aOutputFile = "output_fast.fastq";
	string aOutputFolder = "data";

	CmdWriteFragment aFragmentWriter(aOutputFile, aOutputFolder);

	aFragmentWriter.initialiseFileOutput();

	aFragmentWriter.getStream().open(aOutputFile);

	aFragmentWriter.printDataSetToFile(aFragmentPairSet);

	aFragmentWriter.getStream().close();
}

void lowMemory(const string& aInput)
{
	string aOutputFile = "data/output_memory.fastq";
	string aOutputFolder = "data";

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aInput.c_str());

	CmdWriteFragment aFragmentWriter(aOutputFile, aOutputFolder);
	aFragmentWriter.initialiseFileOutput();

	while (!aFragmentReader.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aFragmentPair;
		aFragmentReader.populateNextFragmentPair(aFragmentPair);
		if (aFragmentPair)
		{
			aFragmentWriter.printAndOrderFragmentPairToFile(aFragmentPair);
		}
	}

	aFragmentReader.getStream().close();

	aFragmentWriter.getStream().flush();
	aFragmentWriter.getStream().close();
}

//JOSH try to run with CUDA
int main()
{
	clock_t clkStart;
	clock_t clkFinish;

	clkStart = clock();
	
	string aInputFolder = "data";
	string aInputFile = "big-test.fastq";
	string aOutputFolder = "data";
	string aOutputFile = "output_mergesort.fastq";
	
	//fast(aInput);
	
	//lowMemory(aInput);
	
	//3 folder manager threads seems to be the optimal solution
	CmdMergeSort aMergeSorter(aInputFolder, aInputFile, aOutputFolder, aOutputFile, 3);
	aMergeSorter.execute();

	clkFinish = clock();
	std::cout << clkFinish - clkStart;

	return 0;
}
