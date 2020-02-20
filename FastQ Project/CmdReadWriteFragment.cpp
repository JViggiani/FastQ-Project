#include "pch.h"
#include "CmdReadWriteFragment.h"
#include <filesystem>

void CmdWriteFragment::initialiseFileOutput()
{
	//delete temp if it exists
	//delete output if it exists
	//open output
	//open temp

	remove(_aOutputFileName.c_str());
	remove("data/temp.fastq");

	_ioStream.close();
	_ioStream.open("data/temp.fastq");
	
	_ioStream.close();
	_ioStream.open(_aOutputFileName.c_str());
	_ioStream.close();
}

void CmdWriteFragment::printDataSetToFile(const std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor>& aSetToPrint)
{
	for (auto item : aSetToPrint)
	{
		printFragmentPairToEndOfFile(item);
	}
}

void CmdWriteFragment::printFragmentPairToEndOfFile(const std::shared_ptr<FragmentPair>& item)
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

void CmdWriteFragment::printAndOrderFragmentPairToFile(const std::shared_ptr<FragmentPair>& aFragmentPair)
{
	_ioStream.open("data/temp.fastq");

	CmdReadFragment aFragmentReader;

	aFragmentReader.getStream().open(_aOutputFileName.c_str());

	bool hasWroteCurrentFragment = false;

	while (!aFragmentReader.getStream().eof())
	{
		std::shared_ptr<FragmentPair> aWrittenFragmentPair = aFragmentReader.populateNextFragmentPair();
		
		char c = aFragmentReader.getStream().peek(); 

		if (!aWrittenFragmentPair.get()) 
		{
			//First item, just write the fragmentpair
			printFragmentPairToEndOfFile(aFragmentPair);
			hasWroteCurrentFragment = true;
			break;
		}

		if ((!hasWroteCurrentFragment && *(aFragmentPair.get()) > *(aWrittenFragmentPair.get())))
		{
			printFragmentPairToEndOfFile(aFragmentPair);
			//Want to only print written from here
			hasWroteCurrentFragment = true;
			printFragmentPairToEndOfFile(aWrittenFragmentPair);
		}
		else
		{
			printFragmentPairToEndOfFile(aWrittenFragmentPair);
			if (!hasWroteCurrentFragment)
			{
				printFragmentPairToEndOfFile(aFragmentPair);
				hasWroteCurrentFragment = true;
			}
		}
	}
	
	_ioStream.close();
	aFragmentReader.getStream().close();

	remove(_aOutputFileName.c_str());
	std::filesystem::rename("data/temp.fastq", _aOutputFileName.c_str());
}
