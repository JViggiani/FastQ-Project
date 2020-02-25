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
	std::filesystem::create_directory("data/temp/MERGEME/1");

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(this->_inputFile.c_str());

	int aFileCounter = 1;
	while (!aFragmentReader.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aFragmentPair;
		aFragmentReader.populateNextFragmentPair(aFragmentPair);
		if (aFragmentPair)
		{
			string aFileName = "data/temp/MERGEME/1/1_" + std::to_string(aFileCounter);

			CmdWriteFragment aFragmentWriter(aFileName);
			aFragmentWriter.initialiseFileOutput();
			aFragmentWriter.getStream().open(aFileName.c_str());

			aFragmentWriter.printFragmentPairToEndOfFile(aFragmentPair);
			aFragmentWriter.getStream().flush();
			aFragmentWriter.getStream().close();
		}
		++aFileCounter;
	}
	mergeFolder(_numOfMergerThreads);
}

void CmdMergeSort::createFolder(int aFolderNum)
{
	// Check if current folder exists
	if (!filesystem::is_directory("data/temp/MERGEME/" + std::to_string(aFolderNum))
		|| !filesystem::exists("data/temp/MERGEME/" + std::to_string(aFolderNum)))
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(650));
		//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ":  create directory for folder " << std::to_string(aFolderNum) << "\n";

		filesystem::create_directory("data/temp/MERGEME/" + std::to_string(aFolderNum)); // create current folder
	}
}

void CmdMergeSort::mergeFiles(const string& aFile1, const string& aFile2, const int& aFolderNum, int& aFileCounter)
{
	string aCurrentFullPath = "data/temp/MERGEME/" + std::to_string(aFolderNum) + "/" + aFile1;
	string aPreviousFullPath = "data/temp/MERGEME/" + std::to_string(aFolderNum) + "/" + aFile2;

	//data/big-test.fastq
	CmdReadFragment aCurrentFileReader;
	aCurrentFileReader.getStream().open(aCurrentFullPath);
	//std::this_thread::sleep_for(std::chrono::milliseconds(674));
	//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Opened current: " << aCurrentFullPath << "\n";

	CmdReadFragment aPreviousFileReader;
	aPreviousFileReader.getStream().open(aPreviousFullPath);
	//std::this_thread::sleep_for(std::chrono::milliseconds(943));
	//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Opened previous: " << aPreviousFullPath << "\n";

	string aWrittenFileName = "data/temp/MERGEME/" + std::to_string(aFolderNum + 1) + "/" + std::to_string(aFolderNum + 1) + "_" + std::to_string(aFileCounter);
	CmdWriteFragment aFragmentWriter(aWrittenFileName);
	aFragmentWriter.initialiseFileOutput();
	aFragmentWriter.getStream().open(aWrittenFileName.c_str());

	//std::this_thread::sleep_for(std::chrono::milliseconds(658));
	//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Writing to file: " << aWrittenFileName << "\n";

	bool isPrintedCurrent = true;
	bool isPrintedPrevious = true;

	std::unique_ptr<FragmentPair> aCurrentPair;
	std::unique_ptr<FragmentPair> aPreviousPair;

	while (!aCurrentFileReader.getStream().eof() || !aPreviousFileReader.getStream().eof())
	{
		if (isPrintedCurrent)
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(758));
			//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Printed current in last iteration, fetching from currentfile" << "\n";
		
			aCurrentPair.reset();
			aCurrentFileReader.populateNextFragmentPair(aCurrentPair);
		}
		if (isPrintedPrevious)
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(475));
			//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Printed previous in last iteration, fetching from currentfile" << "\n";

			aPreviousPair.reset();
			aPreviousFileReader.populateNextFragmentPair(aPreviousPair);
		}

		if (aCurrentPair && aPreviousPair)
		{
			if (*aCurrentPair.get() > *aPreviousPair.get())
			{
				aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
				isPrintedCurrent = true;
				isPrintedPrevious = false;
				
				//std::this_thread::sleep_for(std::chrono::milliseconds(785));
				//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current fragment greater, appended to written file" << "\n";
			}
			else if (*aCurrentPair.get() < *aPreviousPair.get())
			{
				aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
				isPrintedCurrent = false;
				isPrintedPrevious = true;
				
				//std::this_thread::sleep_for(std::chrono::milliseconds(758));
				//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Previous fragment greater, appended to written file" << "\n";
			}
			else
			{
				//equal, write both
				aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
				aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
				isPrintedCurrent = true;
				isPrintedPrevious = true;
				//std::this_thread::sleep_for(std::chrono::milliseconds(849));
				//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current and previous fragment equal, both appended to written file" << "\n";
			}
		}
		else if (!aCurrentPair && aPreviousPair)
		{
			aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
			isPrintedCurrent = false;
			isPrintedPrevious = true;
			
			//std::this_thread::sleep_for(std::chrono::milliseconds(895));
			//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current fragment null, previous appended to written file" << "\n";
		}
		else if (aCurrentPair && !aPreviousPair)
		{
			aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
			isPrintedCurrent = true;
			isPrintedPrevious = false;
			//std::this_thread::sleep_for(std::chrono::milliseconds(937));
			//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Previous fragment null, current appended to written file" << "\n";
		}
		else
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(748));
			//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Unknown condition, both fragments null" << "\n";

			//we may get in here at some point, need to handle edge case.. end of files???
			//exception e;
			//throw e;
			//need to force a read for both JOSH is this buggy?
			isPrintedCurrent = true;
			isPrintedPrevious = true;
		}
		aFragmentWriter.getStream().flush();
	}

	aCurrentFileReader.getStream().close();
	aPreviousFileReader.getStream().close();
	aFragmentWriter.getStream().close();

	//remove read files
	remove(aCurrentFullPath.c_str());
	remove(aPreviousFullPath.c_str());
	
	//std::this_thread::sleep_for(std::chrono::milliseconds(694));
	//std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Reading complete. Deleting read files" << "\n";

	aFileCounter++;
}

CmdMergeSort::PreviousFolderStatus CmdMergeSort::checkIfPreviousFoldersPopulated(const int& aCurrentFolderNum)
{
	CmdMergeSort::PreviousFolderStatus aPreviousFolderStatus = CmdMergeSort::PreviousFolderStatus::FinishedPreviousMerges;

	for (int i = aCurrentFolderNum - 1; i > (aCurrentFolderNum - (_numOfMergerThreads + 1)) && i > 0; --i)
	{		
		if(!std::filesystem::exists("data/temp/MERGEME/" + std::to_string(i)))
		{
			//A folder got deleted, a thread is claiming to have finished merging
			aPreviousFolderStatus = CmdMergeSort::PreviousFolderStatus::FolderDeleted;
			break;
		}
		
		if (!std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(i)))
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
	for (int i = aCurrentFolderNum + 1; i <= aCurrentFolderNum + _numOfMergerThreads; ++i) //JOSH _numofmergerthreads - 1?
	{
		if (std::filesystem::exists("data/temp/MERGEME/" + std::to_string(i)) && !std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(i)))
		{
			//merging is happening in following folders
			aNextMergesStarted = true;
			break;
		}
	}
	return aNextMergesStarted;
}

/*
int CmdMergeSort::countFilesInFolder(const int& aFolderNum)
{
	return std::distance(std::filesystem::directory_iterator("data/temp/MERGEME/" + std::to_string(aFolderNum)), std::filesystem::directory_iterator{});
}
*/

CmdMergeSort::FileCountStatus CmdMergeSort::checkFileNumStatus(const int& aCurrentFolderNum)
{
	int Nb_ext = 0;

	if (std::filesystem::is_directory("data/temp/MERGEME/" + std::to_string(aCurrentFolderNum)))
	{
		for (auto& p : filesystem::directory_iterator("data/temp/MERGEME/" + std::to_string(aCurrentFolderNum)))
		{
			++Nb_ext;
			if (Nb_ext >= 2)
				break;
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
	//JOSH tidy this up, we just want ot check current, previous and future folder status whether they are empty, deleted or populated.. can just use one enum 
	
	CmdMergeSort::PreviousFolderStatus aPreviousMergesFinishedStatus = checkIfPreviousFoldersPopulated(aCurrentFolderNum);
	bool aNextFoldersPopulated = checkIfNextFoldersPopulated(aCurrentFolderNum);
	CmdMergeSort::FileCountStatus aFileCountStatus = checkFileNumStatus(aCurrentFolderNum);

	bool aShouldStartNextMerge = 
		((aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FolderDeleted || aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FinishedPreviousMerges) 
			&& (aNextFoldersPopulated) && (aFileCountStatus == CmdMergeSort::FileCountStatus::Zero));
	if (aShouldStartNextMerge)
		return CmdMergeSort::FolderStatus::NextMerge;

	bool aShouldMoveFile = 
		((aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FolderDeleted) 
			&& aNextFoldersPopulated && (aFileCountStatus == CmdMergeSort::FileCountStatus::Single));
	if (aShouldMoveFile)
		return CmdMergeSort::FolderStatus::MoveNextMerge;

	bool aShouldDoneThisThread = 
		((aPreviousMergesFinishedStatus == CmdMergeSort::PreviousFolderStatus::FolderDeleted) 
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
	for (auto& p : filesystem::directory_iterator("data/temp/MERGEME/" + std::to_string(aCurrentFolderNum)))
	{
		string aCurrentFileName = p.path().string().substr(p.path().string().find("\\") + 1);
		filesystem::rename(("data/temp/MERGEME/" + std::to_string(aCurrentFolderNum) + "/" + aCurrentFileName),
			("data/temp/MERGEME/" + std::to_string(aCurrentFolderNum + 1) + "/" + aCurrentFileName));
	}
}

void CmdMergeSort::mergeFolder(int aCurrentFolderNum)
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(700));
	//std::cout << "DEBUG - " << std::to_string(aCurrentFolderNum) << ": Mergefolder begin" << "\n";

	createFolder(aCurrentFolderNum);
	createFolder(aCurrentFolderNum + 1);

	bool aPreviousMergesFinished = false;

	int aFileCounter = 1;

	CmdMergeSort::FolderStatus aFolderStatus = CmdMergeSort::FolderStatus::Retry;

	while (aFolderStatus == CmdMergeSort::FolderStatus::Retry)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(895));
		//std::cout << "DEBUG - " << std::to_string(aCurrentFolderNum) << ":  not all prior folders merged, trying again with best effort" << "\n";

		string aPreviousFileName;
		//We have to set to 1 initially to allow for edge conditions at the start
		int aTimesVisited = 1;
		filesystem::directory_iterator aEnd;

		for (filesystem::directory_iterator aIterator("data/temp/MERGEME/" + std::to_string(aCurrentFolderNum));
			aIterator != aEnd;
			++aIterator)
		{
			//std::this_thread::sleep_for(std::chrono::milliseconds(809));
			//std::cout << "DEBUG - " << std::to_string(aCurrentFolderNum) << ": Iteration" << "\n";

			//Want to get both unique file names, otherwise we will be trying to get data from a file which no longer exists
			if (aTimesVisited == 2)
			{
				//std::this_thread::sleep_for(std::chrono::milliseconds(576));
				//std::cout << "DEBUG - " << std::to_string(aCurrentFolderNum) << ": Two files found for comparison" << "\n";

				string aCurrentFileName = aIterator->path().string().substr(aIterator->path().string().find("\\") + 1);

				mergeFiles(aCurrentFileName, aPreviousFileName, aCurrentFolderNum, aFileCounter);

				aTimesVisited = 0;
			}

			aPreviousFileName = aIterator->path().string().substr(aIterator->path().string().find("\\") + 1);

			++aTimesVisited;
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(748));
		//std::cout << "DEBUG - " << std::to_string(aCurrentFolderNum) << ": Finished inner loop" << "\n";

		aFolderStatus = checkFolderStatus(aCurrentFolderNum);

		//std::this_thread::sleep_for(std::chrono::milliseconds(648));
		//std::cout << "DEBUG - " << std::to_string(aCurrentFolderNum) << ": Folder status: " << aFolderStatus << "\n";
		//count number of files in current folder

		/*
		if previous merges done and only one currently, we are finished the whole merge
		else if 
		*/
		
		//JOSH add an exception check here for if there is one file in this folder and none in previous folders. If so, just move the file into the next folder

		//if(aPreviousMergesFinished)
	}

	string aCurrentDir = "data/temp/MERGEME/" + std::to_string(aCurrentFolderNum);
	if (aFolderStatus == CmdMergeSort::FolderStatus::NextMerge)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(859));
		//std::cout << "DEBUG: NextMerge for folder " << std::to_string(aCurrentFolderNum) << " end" << "\n";
		filesystem::remove(aCurrentDir);
		mergeFolder(aCurrentFolderNum + _numOfMergerThreads);
	}
	else if (aFolderStatus == CmdMergeSort::FolderStatus::MoveNextMerge)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(859));
		//std::cout << "DEBUG: MoveNextMerge for folder " << std::to_string(aCurrentFolderNum) << " end" << "\n";
		moveAllFilesToNextDir(aCurrentFolderNum);
		filesystem::remove(aCurrentDir);
		mergeFolder(aCurrentFolderNum + _numOfMergerThreads);
	}
	else if(aFolderStatus == CmdMergeSort::FolderStatus::DoneThisThread)
	{
		//We are done and have the last file 
		//rename directory
		rename(aCurrentDir.c_str(), _output.c_str());
	}
	else
	{
		//some other thread has moved the output file, can safely clean up and exit
		filesystem::remove(aCurrentDir);
		return;
	}
}

void CmdMergeSort::execute()
{
	//setup directory structure
	std::filesystem::create_directories("data/temp/MERGEME");
	std::filesystem::remove_all(_output);

	ThreadPool aPool(_numOfMergerThreads);

	aPool.doJob([this]() {splitFile(); });

	for (int i = 1; i <= _numOfMergerThreads - 1; ++i)
	{
		aPool.doJob(std::bind(&CmdMergeSort::mergeFolder, this, i));
	}

	//auto test = std::bind(&CmdMergeSort::mergeFolder, this, 1);
	//aPool.doJob(test);

	//teardown directory structure
	//std::filesystem::remove_all("data/temp");
}