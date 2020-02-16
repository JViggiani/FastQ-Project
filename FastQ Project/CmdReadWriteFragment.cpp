#include "pch.h"
#include "CmdReadWriteFragment.h"
#include <filesystem>

void CmdReadWriteFragment::printDataSetToFile(const std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor>& aSetToPrint)
{
	for (auto item : aSetToPrint)
	{
		printDataItemToFile(item);
	}
}

void CmdReadWriteFragment::printDataItemToFile(const std::shared_ptr<FragmentPair>& item)
{
	this->_ioStream << item->_fragment1->_seqId << "\n";
	this->_ioStream << item->_fragment1->_rawSequence << "\n";
	this->_ioStream << "+" << "\n";
	this->_ioStream << item->_fragment1->_qualityValue << "\n";

	this->_ioStream << item->_fragment2->_seqId << "\n";
	this->_ioStream << item->_fragment2->_rawSequence << "\n";
	this->_ioStream << "+" << "\n";
	this->_ioStream << item->_fragment2->_qualityValue << "\n";

	this->_ioStream.flush();
}

void CmdReadWriteFragment::printFragmentPairToFileLowMemory(const std::shared_ptr<FragmentPair>& aFragmentPair)
{
	std::filesystem::copy_file("data/output.fastq", "data/temp.fastq");
	remove("data/output.fastq");

	CmdReadFragment aFragmentReader;

	aFragmentReader.getStream().open("data/temp.fastq");

	_ioStream.open("data/output.fastq");

	//copy output to temp file, deleting contents (truncate mode)
	while (!aFragmentReader.getStream().eof())
	{
		std::shared_ptr<FragmentPair> aWrittenFragmentPair = aFragmentReader.populateNextFragmentPair();
		if (!aWrittenFragmentPair.get())
		{
			printDataItemToFile(aFragmentPair);
		}
		else
		{
			if (aFragmentPair.get() < aWrittenFragmentPair.get())
			{
				printDataItemToFile(aFragmentPair);
			}
			printDataItemToFile(aWrittenFragmentPair);
		}
	}

	_ioStream.flush();
	_ioStream.close();
	aFragmentReader.getStream().close();
	int success = remove("data/temp.fastq");
}
