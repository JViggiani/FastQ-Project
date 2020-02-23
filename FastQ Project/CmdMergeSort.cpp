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
	aFragmentReader.getStream().open(this->_aInputFile.c_str());

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
}

void CmdMergeSort::createFolder(int aFolderNum)
{
	// Check if current folder exists
	if (!filesystem::is_directory("data/temp/MERGEME/" + std::to_string(aFolderNum))
		|| !filesystem::exists("data/temp/MERGEME/" + std::to_string(aFolderNum)))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(650));
		std::cout << "DEBUG - " << std::to_string(aFolderNum) << ":  create directory for folder " << std::to_string(aFolderNum) << "\n";
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
	std::this_thread::sleep_for(std::chrono::milliseconds(674));
	std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Opened current: " << aCurrentFullPath << "\n";

	CmdReadFragment aPreviousFileReader;
	aPreviousFileReader.getStream().open(aPreviousFullPath);
	std::this_thread::sleep_for(std::chrono::milliseconds(943));
	std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Opened previous: " << aPreviousFullPath << "\n";

	string aWrittenFileName = "data/temp/MERGEME/" + std::to_string(aFolderNum + 1) + "/" + std::to_string(aFolderNum + 1) + "_" + std::to_string(aFileCounter);
	CmdWriteFragment aFragmentWriter(aWrittenFileName);
	aFragmentWriter.initialiseFileOutput();
	aFragmentWriter.getStream().open(aWrittenFileName.c_str());
	std::this_thread::sleep_for(std::chrono::milliseconds(658));
	std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Writing to file: " << aWrittenFileName << "\n";

	bool isPrintedCurrent = true;
	bool isPrintedPrevious = true;

	std::unique_ptr<FragmentPair> aCurrentPair;
	std::unique_ptr<FragmentPair> aPreviousPair;

	while (!aCurrentFileReader.getStream().eof() && !aPreviousFileReader.getStream().eof())
	{
		if (isPrintedCurrent)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(758));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Printed current in last iteration, fetching from currentfile" << "\n";
			aCurrentPair.reset();
			aCurrentFileReader.populateNextFragmentPair(aCurrentPair);
		}
		if (isPrintedPrevious)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(475));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Printed previous in last iteration, fetching from currentfile" << "\n";
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
				std::this_thread::sleep_for(std::chrono::milliseconds(785));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current fragment greater, appended to written file" << "\n";
			}
			else if (*aCurrentPair.get() < *aPreviousPair.get())
			{
				aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
				isPrintedCurrent = false;
				isPrintedPrevious = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(758));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Previous fragment greater, appended to written file" << "\n";
			}
			else
			{
				//equal, write both
				aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
				aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
				isPrintedCurrent = true;
				isPrintedPrevious = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(849));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current and previous fragment equal, both appended to written file" << "\n";
			}
		}
		else if (!aCurrentPair && aPreviousPair)
		{
			aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
			isPrintedCurrent = false;
			isPrintedPrevious = true;
			std::this_thread::sleep_for(std::chrono::milliseconds(895));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current fragment null, previous appended to written file" << "\n";
		}
		else if (aCurrentPair && !aPreviousPair)
		{
			aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
			isPrintedCurrent = true;
			isPrintedPrevious = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(937));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Previous fragment null, current appended to written file" << "\n";
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(748));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Unknown condition, both fragments null" << "\n";
			//we may get in here at some point, need to handle edge case.. end of files???
			//exception e;
			//throw e;
		}
		aFragmentWriter.getStream().flush();
	}

	aCurrentFileReader.getStream().close();
	aPreviousFileReader.getStream().close();
	aFragmentWriter.getStream().close();

	//remove read files
	remove(aCurrentFullPath.c_str());
	remove(aPreviousFullPath.c_str());

	std::this_thread::sleep_for(std::chrono::milliseconds(694));
	std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Reading complete. Deleting read files" << "\n";

	aFileCounter++;
}

void CmdMergeSort::mergeFolder(int aFolderNum)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(700));
	std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Mergefolder begin" << "\n";

	createFolder(aFolderNum);
	createFolder(aFolderNum + 1);

	bool aPreviousMergesFinished = false;

	int aFileCounter = 1;

	while (!aPreviousMergesFinished)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(895));
		std::cout << "DEBUG - " << std::to_string(aFolderNum) << ":  not all prior folders merged, trying again with best effort" << "\n";

		string aPreviousFileName;
		//We have to set to 1 initially to allow for edge conditions at the start
		int aTimesVisited = 1;
		filesystem::directory_iterator aEnd;
		for (filesystem::directory_iterator aIterator("data/temp/MERGEME/" + std::to_string(aFolderNum));
			aIterator != aEnd;
			++aIterator)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(809));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Iteration" << "\n";

			//Want to get both unique file names, otherwise we will be trying to get data from a file which no longer exists
			if (aTimesVisited == 2)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(576));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Two files found for comparison" << "\n";

				string aCurrentFileName = aIterator->path().string().substr(aIterator->path().string().find("\\") + 1);

				mergeFiles(aCurrentFileName, aPreviousFileName, aFolderNum, aFileCounter);

				aTimesVisited = 0;
			}

			aPreviousFileName = aIterator->path().string().substr(aIterator->path().string().find("\\") + 1);

			++aTimesVisited;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(748));
		std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Finished inner loop" << "\n";

		//JOSH simplify this break check
		if (aFolderNum >= 4)
		{
			aPreviousMergesFinished =
				std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum))
				&& std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum - 1))
				&& std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum - 2))
				&& std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum - 3));

			std::this_thread::sleep_for(std::chrono::milliseconds(758));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Big finished check: " << aPreviousMergesFinished << "\n";
		}
		else
		{
			aPreviousMergesFinished = true;
			for (int i = aFolderNum; i > 0; --i)
			{
				if (!std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(i)))
				{
					//still more to do
					aPreviousMergesFinished = false;
					break;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(648));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Tiny finished check: " << aPreviousMergesFinished << "\n";
		}

		//JOSH add an exception check here for if there is one file in this folder and none in previous folders. If so, just move the file into the next folder
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(859));
	std::cout << "DEBUG: Mergefolder for folder " << std::to_string(aFolderNum) << " end" << "\n";
	//mergeFolder(aFolderNum + 4);
}

void CmdMergeSort::execute(const int& aNumberOfThreads)
{
	//setup directory structure
	std::filesystem::create_directories("data/temp/MERGEME");

	ThreadPool aPool(aNumberOfThreads);

	aPool.doJob([this]() {splitFile(); });

	for (int i = 1; i <= aNumberOfThreads - 1; ++i)
	{
		//aPool.doJob(std::bind(&CmdMergeSort::mergeFolder, this, i));
	}

	auto test = std::bind(&CmdMergeSort::mergeFolder, this, 1);
	aPool.doJob(test);

	//teardown directory structure
	//std::filesystem::remove_all("data/temp");
}