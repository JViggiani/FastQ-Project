#pragma once

#include "pch.h"

#include <filesystem>
#include <stdio.h>

class CmdMergeSort
{
public:
	CmdMergeSort(const string& aInputFile, const string& aOutput, const int& aNumberOfThreads)
	{
		_inputFile = aInputFile;
		_output = aOutput;
		//record number of merger threads
		_numOfMergerThreads = aNumberOfThreads;

		if (std::filesystem::exists("data/temp"))
		{
			std::filesystem::remove_all("data/temp");
		}
		std::filesystem::remove_all(_output);
	}

	~CmdMergeSort()
	{
		std::filesystem::remove_all("data/temp");
	}

	//Need at least two threads to execute. One to split and one to merge.
	void execute();

	void splitFile();

	void createFolder(int aFolderNum);

	void mergeFiles(const string& aFile1, const string& aFile2, const int& aFolderNum, int& aFileCounter);

	void mergeFolder(int aFolderNum);
private:
	enum FileCountStatus
	{
		Zero = 0,
		Single = 1,
		Multiple = 2
	};

	enum FolderStatus
	{
		Retry = 0,
		NextMerge = 1,
		MoveNextMerge = 2,
		DoneThisThread = 3,
		DoneOtherThread = 4
	};

	enum PreviousFolderStatus
	{
		FinishedPreviousMerges = 0,
		StillMerging = 1,
		FolderDeleted = 2
	};
	
	CmdMergeSort::PreviousFolderStatus checkIfPreviousFoldersPopulated(const int& aCurrentFolderNum);

	bool checkIfNextFoldersPopulated(const int& aCurrentFolderNum);

	void moveAllFilesToNextDir(const int& aCurrentFolderNum);

	//Returns enum indicating whether there are multiple files in the folder, a single file, or if the folder is empty
	FileCountStatus checkFileNumStatus(const int& aCurrentFolderNum);

	FolderStatus checkFolderStatus(const int& aCurrentFolderNum);

	string _inputFile;
	string _output;
	int _numOfMergerThreads;
};