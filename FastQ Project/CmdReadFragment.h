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

	std::ifstream& getStream()
	{
		return _iStream;
	}

	std::shared_ptr<Fragment> populateNextFragment();

	std::shared_ptr<FragmentPair> populateNextFragmentPair();

private:
	std::ifstream _iStream;
};