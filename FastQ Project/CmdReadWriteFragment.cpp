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
	//std::filesystem::copy_file("data/output.fastq", "data/temp.fastq");
	//remove("data/output.fastq");

	_ioStream.open("data/temp.fastq");

	CmdReadFragment aFragmentReader;

	aFragmentReader.getStream().open("data/output_memory.fastq");

	bool hasWroteCurrentFragment = false;

	while (!aFragmentReader.getStream().eof())
	{
		//JOSH problem is here. At the end of the file there are two new lines and so this gets executed at the end
		std::shared_ptr<FragmentPair> aWrittenFragmentPair = aFragmentReader.populateNextFragmentPair();
		
		char c = aFragmentReader.getStream().peek(); //JOSH what is this at the end? newline?

		if (!aWrittenFragmentPair.get()) //JOSH && file is not at the end?? 
		{
			//First item, just write the fragmentpair
			printDataItemToFile(aFragmentPair);
			hasWroteCurrentFragment = true;
			break;
		}

		if ((!hasWroteCurrentFragment && *(aFragmentPair.get()) > *(aWrittenFragmentPair.get())))
		{
			printDataItemToFile(aFragmentPair);
			//Want to only print written from here
			hasWroteCurrentFragment = true;
			printDataItemToFile(aWrittenFragmentPair);
		}
		else
		{
			printDataItemToFile(aWrittenFragmentPair);
			if (!hasWroteCurrentFragment)
			{
				printDataItemToFile(aFragmentPair);
				hasWroteCurrentFragment = true;
			}
		}
	}

	//_ioStream.flush();
	_ioStream.close();
	aFragmentReader.getStream().close();
	int success = remove("data/output_memory.fastq");
	std::filesystem::rename("data/temp.fastq", "data/output_memory.fastq");
}
