#include "pch.h"
#include "CmdWriteFragment.h"

void CmdWriteFragment::printDataToFile(const std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor>& aSetToPrint)
{
	for (auto item : aSetToPrint)
	{
		this->_outputStream << item->_fragment1->_seqId << "\n";
		this->_outputStream << item->_fragment1->_rawSequence << "\n";
		this->_outputStream << "+" << "\n";
		this->_outputStream << item->_fragment1->_qualityValue << "\n";

		this->_outputStream << item->_fragment2->_seqId << "\n";
		this->_outputStream << item->_fragment2->_rawSequence << "\n";
		this->_outputStream << "+" << "\n";
		this->_outputStream << item->_fragment2->_qualityValue << "\n";
	}
}