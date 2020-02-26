#include "pch.h"

#include "CmdMergeSort.h"
#include "CmdReadFragment.h"
#include "CmdWriteFragment.h"
#include "FileThread.h"

#include <functional>

#include <filesystem>
#include <stdio.h>

void CmdMergeSort::splitFile()
{
	std::filesystem::create_directory(_outputFolder + "/temp/MERGEME/1");

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open((_inputFolder + "/" + _inputFile).c_str());

	CmdWriteFragment aFragmentWriter;

	int aFileCounter = 1;
	while (!aFragmentReader.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aFragmentPair;
		aFragmentReader.populateNextFragmentPair(aFragmentPair);
		if (aFragmentPair)
		{
			string aFileName = "1_" + std::to_string(aFileCounter);

			aFragmentWriter.lockFragmentFile(_outputFolder, 0, aFileName);
			aFragmentWriter.getStream().open((_outputFolder + "/temp/MERGEME/1/" + aFileName).c_str(), ios::trunc);

			aFragmentWriter.printFragmentPairToEndOfFile(aFragmentPair);
			aFragmentWriter.getStream().flush();
			aFragmentWriter.getStream().close();
			aFragmentWriter.unlockFragmentFile(_outputFolder, 0);
		}
		++aFileCounter;
	}
	mergeFolder(_numOfMergerThreads);
}

void CmdMergeSort::createFolder(int aFolderNum)
{
	// Check if current folder exists
	if (!filesystem::is_directory(_outputFolder + "/temp/MERGEME/" + std::to_string(aFolderNum))
		|| !filesystem::exists(_outputFolder + "/temp/MERGEME/" + std::to_string(aFolderNum)))
	{
		// create current folder
		filesystem::create_directory(_outputFolder + "/temp/MERGEME/" + std::to_string(aFolderNum)); 
	}
}

void CmdMergeSort::mergeFiles(const string& aFile1, const string& aFile2, const int& aFolderNum, int& aFileCounter)
{
	//Set up readers
	CmdReadFragment aCurrentFileReader;
	CmdReadFragment aPreviousFileReader;
	
	string aCurrentFullPath = _outputFolder + "/temp/MERGEME/" + std::to_string(aFolderNum) + "/" + aFile1;
	string aPreviousFullPath = _outputFolder + "/temp/MERGEME/" + std::to_string(aFolderNum) + "/" + aFile2;

	if (aCurrentFileReader.isFragmentFileLocked(_outputFolder, aFolderNum, aFile1) || aPreviousFileReader.isFragmentFileLocked(_outputFolder, aFolderNum, aFile2))
	{
		//One of the files is currently being written to, need to return for a retry
		return;
	}

	aCurrentFileReader.getStream().open(aCurrentFullPath);
	aPreviousFileReader.getStream().open(aPreviousFullPath);

	//Set up writer
	CmdWriteFragment aFragmentWriter;

	string aWrittenFileName = std::to_string(aFolderNum + 1) + "_" + std::to_string(aFileCounter);
	string aWrittenFullPath = _outputFolder + "/temp/MERGEME/" + std::to_string(aFolderNum + 1) + "/" + aWrittenFileName;

	aFragmentWriter.lockFragmentFile(_outputFolder, aFolderNum, aWrittenFileName);
	aFragmentWriter.getStream().open(aWrittenFullPath.c_str(), ios::trunc);

	//Begin comparing the two read file elements, print to written file
	bool isPrintedCurrent = true;
	bool isPrintedPrevious = true;

	std::unique_ptr<FragmentPair> aCurrentPair;
	std::unique_ptr<FragmentPair> aPreviousPair;

	while (!aCurrentFileReader.getStream().eof() || !aPreviousFileReader.getStream().eof())
	{
		//We want to keep the previous iteration in memory if it wasn't printed for comparison to the next sorted fragment in the other file
		if (isPrintedCurrent)
		{
			aCurrentPair.reset();
			aCurrentFileReader.populateNextFragmentPair(aCurrentPair);
		}
		if (isPrintedPrevious)
		{
			aPreviousPair.reset();
			aPreviousFileReader.populateNextFragmentPair(aPreviousPair);
		}

		//We got fragment pairs, ie the readers have not yet reached the end of either file
		if (aCurrentPair && aPreviousPair)
		{
			//Want to write the greatest fragment pair first
			if (*aCurrentPair.get() > *aPreviousPair.get())
			{
				aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
				isPrintedCurrent = true;
				isPrintedPrevious = false;
			}
			else if (*aCurrentPair.get() < *aPreviousPair.get())
			{
				aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
				isPrintedCurrent = false;
				isPrintedPrevious = true;
			}
			else
			{
				//equal, write both
				aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
				aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
				isPrintedCurrent = true;
				isPrintedPrevious = true;
			}
		}
		else if (!aCurrentPair && aPreviousPair)
		{
			aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
			//aFragmentWriter.printToEndOfFile(aPreviousFileReader); JOSH am I really faster? not really \todo
			isPrintedCurrent = false;
			isPrintedPrevious = true;
			//break;
		}
		else if (aCurrentPair && !aPreviousPair)
		{
			aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
			//aFragmentWriter.printToEndOfFile(aCurrentFileReader);
			isPrintedCurrent = true;
			isPrintedPrevious = false;
			//break;
		}
		else
		{
			//stream.eof() goes one after end of file, force break
			break;
		}
		aFragmentWriter.getStream().flush();
	}

	//handle stream ends and file unlocking
	aCurrentFileReader.getStream().close();
	aPreviousFileReader.getStream().close();
	aFragmentWriter.getStream().close();
	aFragmentWriter.unlockFragmentFile(_outputFolder, aFolderNum);

	//remove read files
	remove(aCurrentFullPath.c_str());
	remove(aPreviousFullPath.c_str());

	aFileCounter++;
}

CmdMergeSort::PreviousFolderStatus CmdMergeSort::checkIfPreviousFoldersPopulated(const int& aCurrentFolderNum)
{
	CmdMergeSort::PreviousFolderStatus aPreviousFolderStatus = CmdMergeSort::PreviousFolderStatus::FinishedPreviousMerges;

	for (int i = aCurrentFolderNum - 1; i > (aCurrentFolderNum - (_numOfMergerThreads + 1)) && i > 0; --i)
	{
		if (!std::filesystem::exists(_outputFolder + "/temp/MERGEME/" + std::to_string(i)))
		{
			//A folder got deleted, a thread is claiming to have finished merging
			aPreviousFolderStatus = CmdMergeSort::PreviousFolderStatus::FolderDeleted;
			break;
		}
		else if (!std::filesystem::is_empty(_outputFolder + "/temp/MERGEME/" + std::to_string(i)))
		{
			//still more to do from previous folders
			aPreviousFolderStatus = CmdMergeSort::PreviousFolderStatus::StillMerging;
			break;
		}
		
	}
	return aPreviousFolderStatus;
}

bool CmdMergeSort::checkIfNextFoldersPopulated(const int& aCurrentFolderNum)
{
	bool aNextMergesStarted = false;
	for (int i = aCurrentFolderNum + 1; i <= aCurrentFolderNum + (_numOfMergerThreads - 1); ++i)
	{
		if (std::filesystem::exists(_outputFolder + "/temp/MERGEME/" + std::to_string(i)) && !std::filesystem::is_empty(_outputFolder + "/temp/MERGEME/" + std::to_string(i)))
		{
			//merging is still happening in following folders
			aNextMergesStarted = true;
			break;
		}
	}
	return aNextMergesStarted;
}

CmdMergeSort::FileCountStatus CmdMergeSort::checkFileNumStatus(const int& aCurrentFolderNum)
{
	int Nb_ext = 0;

	if (std::filesystem::is_directory(_outputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum)))
	{
		for (auto& p : filesystem::directory_iterator(_outputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum)))
		{
			string aCurrentFileName = p.path().string().substr(p.path().string().find("\\") + 1);
			if (aCurrentFileName == "lockfile")
			{
				//we only want to count data files, not the lock file
				continue;
			}
			else
			{
				++Nb_ext;
				if (Nb_ext >= 2)
					break;
			}
		}
	}

	if (Nb_ext == 0)
	{
		return CmdMergeSort::FileCountStatus::Zero;
	}
	else if (Nb_ext == 1)
	{
		return CmdMergeSort::FileCountStatus::Single;
	}
	else
	{
		return CmdMergeSort::FileCountStatus::Multiple;
	}
}

CmdMergeSort::FolderStatus CmdMergeSort::checkFolderStatus(const int& aCurrentFolderNum)
{
	//JOSH \todo tidy this up, we just want ot check current, previous and future folder status whether they are empty, deleted or populated.. can just use one enum 

	CmdMergeSort::PreviousFolderStatus aPreviousMergesFinishedStatus;
	bool aNextFoldersPopulated;
	CmdMergeSort::FileCountStatus aFileCountStatus;

	try
	{
		aPreviousMergesFinishedStatus = checkIfPreviousFoldersPopulated(aCurrentFolderNum);
		aNextFoldersPopulated = checkIfNextFoldersPopulated(aCurrentFolderNum);
		aFileCountStatus = checkFileNumStatus(aCurrentFolderNum);
	}
	catch (exception& e)
	{
		//The thread which finished must have deleted a folder we tried to access
		if (string(e.what()).find("Access is denied") != std::string::npos 
			|| string(e.what()).find("The system cannot find the file specified") != std::string::npos
			|| string(e.what()).find("The system cannot find the path specified") != std::string::npos)
		{
			return CmdMergeSort::FolderStatus::DoneOtherThread;
		}
	}


	bool aShouldStartNextMerge =
		(( aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FinishedPreviousMerges)
			&& (aNextFoldersPopulated) && (aFileCountStatus == CmdMergeSort::FileCountStatus::Zero));
	if (aShouldStartNextMerge)
		return CmdMergeSort::FolderStatus::NextMerge;

	bool aShouldMoveFile =
		((aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FinishedPreviousMerges)
			&& aNextFoldersPopulated && (aFileCountStatus == CmdMergeSort::FileCountStatus::Single));
	if (aShouldMoveFile)
		return CmdMergeSort::FolderStatus::MoveNextMerge;

	bool aShouldDoneThisThread =
		((aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FinishedPreviousMerges)
			&& !aNextFoldersPopulated && (aFileCountStatus == CmdMergeSort::FileCountStatus::Single));
	if (aShouldDoneThisThread)
		return CmdMergeSort::FolderStatus::DoneThisThread;

	bool aShouldDoneOtherThread =
		((aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FolderDeleted)
			&& !aNextFoldersPopulated && (aFileCountStatus == CmdMergeSort::FileCountStatus::Zero));
	if (aShouldDoneOtherThread)
		return CmdMergeSort::FolderStatus::DoneOtherThread;

	return CmdMergeSort::FolderStatus::Retry;
}

void CmdMergeSort::moveAllFilesToNextDir(const int& aCurrentFolderNum)
{
	for (auto& p : filesystem::directory_iterator(_outputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum)))
	{
		string aCurrentFileName = p.path().string().substr(p.path().string().find("\\") + 1);
		if (aCurrentFileName != "lockfile")
		{
			filesystem::rename((_outputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum) + "/" + aCurrentFileName),
				(_outputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum + 1) + "/" + aCurrentFileName));
		}
	}
}

void CmdMergeSort::mergeFolder(int aCurrentFolderNum)
{
	createFolder(aCurrentFolderNum);
	createFolder(aCurrentFolderNum + 1);

	bool aPreviousMergesFinished = false;

	int aFileCounter = 1;

	CmdMergeSort::FolderStatus aFolderStatus = CmdMergeSort::FolderStatus::Retry;

	string aCurrentDir = _outputFolder + "/temp/MERGEME/" + std::to_string(aCurrentFolderNum);

	while (aFolderStatus == CmdMergeSort::FolderStatus::Retry)
	{
		string aPreviousFileName;
		//We have to set to 1 initially to allow for edge conditions at the start
		int aTimesVisited = 1;
		filesystem::directory_iterator aEnd;

		for (filesystem::directory_iterator aIterator(aCurrentDir);
			aIterator != aEnd;
			++aIterator)
		{
			string aCurrentFileName = aIterator->path().string().substr(aIterator->path().string().find("\\") + 1);
			
			//Ignore the lockfile, we don't treat it as a data file
			if (aCurrentFileName != "lockfile")
			{
				//Want to get both unique file names, otherwise we will be trying to merge the same file
				if (aTimesVisited == 2)
				{
					mergeFiles(aCurrentFileName, aPreviousFileName, aCurrentFolderNum, aFileCounter);

					aTimesVisited = 0;
				}
			}

			aPreviousFileName = aCurrentFileName;

			if (aPreviousFileName == "lockfile" && aCurrentFileName == "lockfile")
				break;

			++aTimesVisited;
		}

		aFolderStatus = checkFolderStatus(aCurrentFolderNum);
	}

	//We are done with this folder, need to handle what to do next
	try
	{
		if (aFolderStatus == CmdMergeSort::FolderStatus::NextMerge)
		{
			while (std::filesystem::exists(aCurrentDir + "/lockfile"))
			{
				try
				{
					filesystem::remove(aCurrentDir + "/lockfile");
				}
				catch (exception& e)
				{
					//The thread which was writing to this lock must still have it open, retry
					if (string(e.what()).find("Access is denied") != std::string::npos)
					{
						throw e;
					}
				}
			}
			mergeFolder(aCurrentFolderNum + _numOfMergerThreads);
		}
		else if (aFolderStatus == CmdMergeSort::FolderStatus::MoveNextMerge)
		{
			moveAllFilesToNextDir(aCurrentFolderNum);
			while (std::filesystem::exists(aCurrentDir + "/lockfile"))
			{
				try
				{
					filesystem::remove(aCurrentDir + "/lockfile");
				}
				catch (exception& e)
				{
					//The thread which was writing to this lock must still have it open, retry
					if (string(e.what()).find("Access is denied") != std::string::npos)
					{
						throw e;
					}
				}
			}
			mergeFolder(aCurrentFolderNum + _numOfMergerThreads);
		}
		else if (aFolderStatus == CmdMergeSort::FolderStatus::DoneThisThread)
		{
			//We are done and have the last file 
			//Rename the file to the desired output and place it in the desired folder

			if (std::filesystem::exists(_outputFolder.c_str()))
			{
				//try to remove output file	if it already exists	
				remove((_outputFolder + "/" + _outputFile).c_str());
				//move "foldernum_1" to output
				rename((aCurrentDir + "/" + std::to_string(aCurrentFolderNum) + "_1").c_str(), (_outputFolder + "/" + _outputFile).c_str());
				//remove origin folder
				std::filesystem::remove_all(aCurrentDir.c_str());
			}
			else
			{
				//try to remove output file if it already exists
				remove((_outputFolder + "/" + _outputFile).c_str());
				//rename folder to output folder
				rename(aCurrentDir.c_str(), _outputFolder.c_str());
				//rename file to output file
				rename((_outputFolder + "/" + std::to_string(aCurrentFolderNum) + "_1").c_str(), (_outputFolder + "/" + _outputFile).c_str());
			}
		}
		else
		{
			//some other thread has moved the output file, can safely clean up and exit
			filesystem::remove(aCurrentDir);
			return;
		}
	}
	catch (exception& e)
	{
		cout << e.what();
	}

}

void CmdMergeSort::execute()
{
	//setup directory structure
	std::filesystem::create_directories(_outputFolder + "/temp/MERGEME");

	ThreadPool aPool(_numOfMergerThreads);

	aPool.doJob([this]() {splitFile(); });

	for (int i = 1; i <= _numOfMergerThreads - 1; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(i));
		aPool.doJob(std::bind(&CmdMergeSort::mergeFolder, this, i));
	}
}