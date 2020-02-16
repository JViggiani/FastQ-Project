#pragma once

#include "pch.h"
#include "Fragment.h"
#include "Quality.h"

#include "CmdProcessFragment.h"

#include <iostream>
#include <fstream>

class CmdReadFragment : public CmdProcessFragment
{
public:
	CmdReadFragment(const string& aFile)
	{
		_inputStream.open(aFile);
	}

	bool eof()
	{
		return _inputStream.eof();
	}

	void open(const string& aFile)
	{
		_inputStream.open(aFile);
	}

	void close()
	{
		_inputStream.close();
	}

	std::shared_ptr<Fragment> populateNextFragment();

	std::shared_ptr<FragmentPair> populateNextFragmentPair();

private:
	std::ifstream _inputStream;
};