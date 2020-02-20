#include "pch.h"

#include "Fragment.h"
#include "CmdReadFragment.h"
#include "CmdWriteFragment.h"

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

	string output = "data/output_fast.fastq";

	CmdWriteFragment aFragmentWriter(output);

	aFragmentWriter.initialiseFileOutput();

	aFragmentWriter.getStream().open(output);

	aFragmentWriter.printDataSetToFile(aFragmentPairSet);

	aFragmentWriter.getStream().close();
}

void lowMemory(const string& aInput)
{
	string aOutput = "data/output_memory.fastq";

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aInput.c_str());

	CmdWriteFragment aFragmentWriter(aOutput);
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

int main()
{
	string aInput = "data/big-test.fastq";
	
	//fast(aInput);
	
	//lowMemory(aInput);
}
