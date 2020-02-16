// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Fragment.h"
#include "CmdReadFragment.h"
#include "CmdReadWriteFragment.h"

int main()
{
	string aFileName = "data/tiny-test.fastq";
	string output = "data/output.fastq";

	remove(output.c_str());
	remove("data/temp.fastq");

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aFileName);
	CmdReadWriteFragment aFragmentWriter(output);
	//aFragmentWriter.getStream().open(output, std::ofstream::trunc);

	std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor> aFragmentPairSet;

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
