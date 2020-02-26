#pragma once

#include "pch.h"

#include <filesystem>
#include <stdio.h>

class CmdMergeSort
{
public:
	CmdMergeSort(const string& aInputFolder, const string& aInputFile, const string& aOutputFolder, const string& aOutputFile, const int& aNumberOfThreads)
	{
		//police string format - we want to remove the slash between file and folder. We will handle this internally
		
		if (&aInputFolder.back() == "/")
		{
			_inputFolder = string(aInputFolder.begin(), aInputFolder.end() - 1);
		}
		else
		{
			_inputFolder = aInputFolder;
		}

		if (&aInputFile.front() == "/")
		{
			_inputFile = string(aInputFile.begin() + 1, aInputFile.end());
		}
		else
		{
			_inputFile = aInputFile;
		}

		if (&aOutputFolder.back() == "/")
		{
			_outputFolder = string(aOutputFolder.begin(), aOutputFolder.end() - 1);
		}
		else
		{
			_outputFolder = aOutputFolder;
		}

		if (&aOutputFile.front() == "/")
		{
			_outputFile = string(aOutputFile.begin() + 1, aOutputFile.end());
		}
		else
		{
			_outputFile = aOutputFile;
		}
		
		//record number of merger threads
		_numOfMergerThreads = aNumberOfThreads;

		if (std::filesystem::exists(_outputFolder + "/temp"))
		{
			std::filesystem::remove_all(_outputFolder + "/temp");
		}
	}

	~CmdMergeSort()
	{
		std::filesystem::remove_all(_outputFolder + "/temp");
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

	string _inputFolder;
	string _inputFile;
	string _outputFolder;
	string _outputFile;
	int _numOfMergerThreads;
};