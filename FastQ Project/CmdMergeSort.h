#pragma once

#include "pch.h"

#include <filesystem>
#include <stdio.h>

class CmdMergeSort
{
public:
	CmdMergeSort(const string& aInputFile)
	{
		_aInputFile = aInputFile;
	}

	//Need at least two threads to execute. One to split and one to merge.
	void execute(const int & aNumberOfThreads);

	void splitFile();

	void createFolder(int aFolderNum);

	void mergeFiles(const string& aFile1, const string& aFile2, const int& aFolderNum, int& aFileCounter);

	void mergeFolder(int aFolderNum);
private:
	string _aInputFile;
};