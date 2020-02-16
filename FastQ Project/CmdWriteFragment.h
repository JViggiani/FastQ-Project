#pragma once

#include "pch.h"
#include "CmdProcessFragment.h"
#include "Fragment.h"
#include "Quality.h"

#include <iostream>
#include <fstream>

class CmdWriteFragment : public CmdProcessFragment
{
public:
	CmdWriteFragment(const string& aFile)
	{
		_outputStream.open(aFile);
	}

	bool eof()
	{
		return _outputStream.eof();
	}

	//Opens a file
	/*
		Creates file if one doesn't exist with this name
		Deletes content before writing if file does exist 
	*/
	void open(const string& aFile)
	{
		_outputStream.open(aFile, std::ofstream::trunc);
	}

	void close()
	{
		_outputStream.flush();
		_outputStream.close();
	}

	void printDataToFile(const std::multiset<std::shared_ptr<FragmentPair>, FragmentPairComparitor>& aSetToPrint);

private:
	std::ofstream _outputStream;
};

