// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Fragment.h"
#include "CmdReadFragment.h"
#include "CmdWriteFragment.h"

int main()
{
	string aFileName = "data/big-test.fastq";

	CmdReadFragment aFragmentReader(aFileName);

	std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor> aFragmentPairSet;
	
	while (!aFragmentReader.eof())
	{
		std::shared_ptr<FragmentPair> aFragmentPair = aFragmentReader.populateNextFragmentPair();
		if (aFragmentPair)
		{
			aFragmentPairSet.insert(aFragmentPair);
		}
	}
	
	aFragmentReader.close();

	/*
	cout << "Multiset constructed. Size: " << aFragmentPairSet.size() << "\n";
	for (std::multiset<FragmentPair*>::const_iterator i = aFragmentPairSet.begin(), end(aFragmentPairSet.end()); i != end; ++i)
	{
		cout << "Average quality: " << (**i).calculateAverageQuality().toInt() << "\n";
	}
	*/

	string output = "data/output.fastq";

	CmdWriteFragment aFragmentWriter(output);

	aFragmentWriter.printDataToFile(aFragmentPairSet);

	aFragmentWriter.close();
}
