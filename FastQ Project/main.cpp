#include "pch.h"

#include "Fragment.h"
#include "CmdReadFragment.h"
#include "CmdReadWriteFragment.h"

void fast()
{
	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open("data/tiny-test.fastq");

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

	CmdWriteFragment aFragmentWriter(output);

	aFragmentWriter.initialiseFileOutput();

	aFragmentWriter.getStream().open(output);

	aFragmentWriter.printDataSetToFile(aFragmentPairSet);

	aFragmentWriter.getStream().close();
}

void lowMemory()
{
	string aInput = "data/tiny-test.fastq";
	string aOutput = "data/output_memory.fastq";

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aInput);

	CmdWriteFragment aFragmentWriter(aOutput);
	aFragmentWriter.initialiseFileOutput();

	while (!aFragmentReader.getStream().eof())
	{
		std::shared_ptr<FragmentPair> aFragmentPair = aFragmentReader.populateNextFragmentPair();
		if (aFragmentPair)
		{
			aFragmentWriter.printAndOrderFragmentPairToFile(aFragmentPair);
		}
	}

	aFragmentReader.getStream().close();

	aFragmentWriter.getStream().flush();
	aFragmentWriter.getStream().close();
}

int main()
{
	fast();
	
	lowMemory();
}
