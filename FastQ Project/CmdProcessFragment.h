#pragma once
class CmdProcessFragment
{
	virtual bool eof() = 0;

	virtual void open(const string& aFile) = 0;

	virtual void close() = 0;
};

