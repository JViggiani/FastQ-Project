// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Fragment.h"
#include "CmdReadFragment.h"
#include "CmdReadWriteFragment.h"

void fast()
{
	//string aFileName = "data/big-test.fastq";

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open("data/big-test.fastq");

	std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor> aFragmentPairSet;

	while (!aFragmentReader.getStream().eof())
	{
		std::shared_ptr<FragmentPair> aFragmentPair = aFragmentReader.populateNextFragmentPair();
		if (aFragmentPair)
		{
			aFragmentPairSet.insert(aFragmentPair);
		}
	}
	aFragmentReader.getStream().close();

	string output = "data/output_fast.fastq";

	CmdReadWriteFragment aFragmentWriter(output);

	aFragmentWriter.getStream().open(output);

	aFragmentWriter.printDataSetToFile(aFragmentPairSet);

	aFragmentWriter.getStream().close();
}

void lowMemory()
{
	string aFileName = "data/big-test.fastq";
	string output = "data/output_memory.fastq";

	remove(output.c_str());
	remove("data/temp.fastq");

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aFileName);
	CmdReadWriteFragment aFragmentWriter(output);
	//aFragmentWriter.getStream().open(output);

	while (!aFragmentReader.getStream().eof())
	{
		std::shared_ptr<FragmentPair> aFragmentPair = aFragmentReader.populateNextFragmentPair();
		if (aFragmentPair)
		{
			aFragmentWriter.printFragmentPairToFileLowMemory(aFragmentPair);
		}
	}

	aFragmentReader.getStream().close();

	aFragmentWriter.getStream().flush();
	aFragmentWriter.getStream().close();
}

int main()
{
	fast();
	
	//lowMemory();
}
