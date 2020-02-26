#include "pch.h"
#include "CmdWriteFragment.h"
#include <filesystem>


//JOSH delete this function and just start opening and closing 
void CmdWriteFragment::initialiseFileOutput()
{
	//delete temp if it exists
	//delete output if it exists
	//open output
	//open temp

	remove(_outputFile.c_str());
	
	_oStream.close();
	_oStream.open(_outputFile.c_str());
	_oStream.close();
}

void CmdWriteFragment::printDataSetToFile(const std::multiset<std::unique_ptr<FragmentPair>, FragmentPairComparitor>& aSetToPrint)
{
	for (auto& item : aSetToPrint)
	{
		printFragmentPairToEndOfFile(item);
	}
}

void CmdWriteFragment::printFragmentPairToEndOfFile(const std::unique_ptr<FragmentPair>& item)
{
	this->_oStream << item->_fragment1->_seqId << "\n";
	this->_oStream << item->_fragment1->_rawSequence << "\n";
	this->_oStream << "+" << "\n";
	this->_oStream << item->_fragment1->_qualityValue << "\n";

	this->_oStream << item->_fragment2->_seqId << "\n";
	this->_oStream << item->_fragment2->_rawSequence << "\n";
	this->_oStream << "+" << "\n";
	this->_oStream << item->_fragment2->_qualityValue << "\n";

	this->_oStream.flush();
}

void CmdWriteFragment::printAndOrderFragmentPairToFile(const std::unique_ptr<FragmentPair>& aFragmentPair)
{
	_oStream.open(_outputFolder + "/temp.fastq");

	CmdReadFragment aFragmentReader;

	aFragmentReader.getStream().open(_outputFile.c_str());

	bool hasWroteCurrentFragment = false;

	while (!aFragmentReader.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aWrittenFragmentPair;
		aFragmentReader.populateNextFragmentPair(aWrittenFragmentPair);
		
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
	
	_oStream.close();
	aFragmentReader.getStream().close();

	remove(_outputFile.c_str());
	std::filesystem::rename(_outputFolder + "/temp.fastq", _outputFile.c_str());
}

void CmdWriteFragment::lockFragmentFile(const string & aOutputFolder, const int & aCurrentFolderNum, const string & aFile1)
{
	_oStream.close();
	_oStream.open(aOutputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum + 1) + "/lockfile", ios::trunc);
	_oStream << aFile1;
	_oStream.flush();
	_oStream.close();
}

void CmdWriteFragment::unlockFragmentFile(const string & aOutputFolder, const int & aCurrentFolderNum)
{
	_oStream.close();
	_oStream.open(aOutputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum + 1) + "/lockfile", ios::trunc);
	_oStream.flush();
	_oStream.close();
}

void CmdWriteFragment::printToEndOfFile(CmdReadFragment & aFileToRead)
{
	while (aFileToRead.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aPair;
		aFileToRead.populateNextFragmentPair(aPair);
		this->printFragmentPairToEndOfFile(aPair);
	}
}
