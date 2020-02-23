#include "pch.h"

#include "Fragment.h"
#include "CmdReadFragment.h"
#include "CmdWriteFragment.h"
#include "FileThread.h"

#include <filesystem>
#include <stdio.h>

void fast(const string& aInput)
{
	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aInput.c_str());

	std::multiset<std::unique_ptr<FragmentPair>, FragmentPairComparitor> aFragmentPairSet;

	while (!aFragmentReader.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aFragmentPair;
		aFragmentReader.populateNextFragmentPair(aFragmentPair);
		if (aFragmentPair)
		{
			aFragmentPairSet.insert(std::move(aFragmentPair)); //JOSH can we change this from a move? probably not
		}
	}
	aFragmentReader.getStream().close();

	string output = "data/output_fast.fastq";

	CmdWriteFragment aFragmentWriter(output);

	aFragmentWriter.initialiseFileOutput();

	aFragmentWriter.getStream().open(output);

	aFragmentWriter.printDataSetToFile(aFragmentPairSet);

	aFragmentWriter.getStream().close();
}

void lowMemory(const string& aInput)
{
	string aOutput = "data/output_memory.fastq";

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aInput.c_str());

	CmdWriteFragment aFragmentWriter(aOutput);
	aFragmentWriter.initialiseFileOutput();

	while (!aFragmentReader.getStream().eof())
	{
		std::unique_ptr<FragmentPair> aFragmentPair;
		aFragmentReader.populateNextFragmentPair(aFragmentPair);
		if (aFragmentPair)
		{
			aFragmentWriter.printAndOrderFragmentPairToFile(aFragmentPair);
		}
	}

	aFragmentReader.getStream().close();

	aFragmentWriter.getStream().flush();
	aFragmentWriter.getStream().close();
}

void splitFile(const string& aInput)
{
	std::filesystem::create_directory("data/temp/MERGEME/1");

	CmdReadFragment aFragmentReader;
	aFragmentReader.getStream().open(aInput.c_str());

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

void mergeFolder(int aFolderNum)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Mergefolder begin" << "\n";
	
	// Check if current folder exists
	if (!filesystem::is_directory("data/temp/MERGEME/" + std::to_string(aFolderNum)) 
		|| !filesystem::exists("data/temp/MERGEME/" + std::to_string(aFolderNum)))
	{ 
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "DEBUG - " << std::to_string(aFolderNum) << ":  create directory for folder " << std::to_string(aFolderNum) << "\n";
		filesystem::create_directory("data/temp/MERGEME/" + std::to_string(aFolderNum)); // create current folder
	}
	// Check if next folder exists
	if (!filesystem::is_directory("data/temp/MERGEME/" + std::to_string(aFolderNum + 1)) 
		|| !filesystem::exists("data/temp/MERGEME/" + std::to_string(aFolderNum + 1)))
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": create directory for folder " << std::to_string(aFolderNum + 1) << "\n";
		filesystem::create_directory("data/temp/MERGEME/" + std::to_string(aFolderNum + 1)); // create next folder
	}

	bool aPreviousMergesFinished = false;

	int aFileCounter = 1;

	while (!aPreviousMergesFinished)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "DEBUG - " << std::to_string(aFolderNum) << ":  not all prior folders merged, trying again with best effort" << "\n";
		
		string aPreviousFileName;
		//We have to set to 1 initially to allow for edge conditions at the start
		int aTimesVisited = 1;
		filesystem::directory_iterator aEnd;
		for (filesystem::directory_iterator aIterator("data/temp/MERGEME/" + std::to_string(aFolderNum));
			aIterator != aEnd;
			++aIterator)
		{			
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Iteration" << "\n";
			
			//Want to get both unique file names, otherwise we will be trying to get data from a file which no longer exists
			if (aTimesVisited == 2)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Two files found for comparison" << "\n";
				
				string aCurrentFileName = aIterator->path().string().substr(aIterator->path().string().find("\\") + 1);

				string aCurrentFullPath = "data/temp/MERGEME/" + std::to_string(aFolderNum) + "/" + aCurrentFileName;
				string aPreviousFullPath = "data/temp/MERGEME/" + std::to_string(aFolderNum) + "/" + aPreviousFileName;

				//data/big-test.fastq
				CmdReadFragment aCurrentFileReader;
				aCurrentFileReader.getStream().open(aCurrentFullPath);
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Opened current: " << aCurrentFullPath << "\n";
				
				CmdReadFragment aPreviousFileReader;
				aPreviousFileReader.getStream().open(aPreviousFullPath);
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Opened previous: " << aPreviousFullPath << "\n";

				string aWrittenFileName = "data/temp/MERGEME/" + std::to_string(aFolderNum + 1) + "/" + std::to_string(aFolderNum + 1) + "_" + std::to_string(aFileCounter);
				CmdWriteFragment aFragmentWriter(aWrittenFileName);
				aFragmentWriter.initialiseFileOutput();
				aFragmentWriter.getStream().open(aWrittenFileName.c_str());
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Writing to file: " << aWrittenFileName << "\n";

				bool isPrintedCurrent = true;
				bool isPrintedPrevious = true;

				std::unique_ptr<FragmentPair> aCurrentPair;
				std::unique_ptr<FragmentPair> aPreviousPair;

				while (!aCurrentFileReader.getStream().eof() && !aPreviousFileReader.getStream().eof())
				{
					if (isPrintedCurrent)
					{
						std::this_thread::sleep_for(std::chrono::seconds(1));
						std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Printed current in last iteration, fetching from currentfile" << "\n";
						aCurrentPair.reset();
						aCurrentFileReader.populateNextFragmentPair(aCurrentPair);
					}
					if (isPrintedPrevious)
					{
						std::this_thread::sleep_for(std::chrono::seconds(1));
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
							std::this_thread::sleep_for(std::chrono::seconds(1));
							std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current fragment greater, appended to written file" << "\n";
						}
						else if (*aCurrentPair.get() < *aPreviousPair.get())
						{
							aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
							isPrintedCurrent = false;
							isPrintedPrevious = true;
							std::this_thread::sleep_for(std::chrono::seconds(1));
							std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Previous fragment greater, appended to written file" << "\n";
						}
						else
						{
							//equal, write both
							aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
							aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
							isPrintedCurrent = true;
							isPrintedPrevious = true;
							std::this_thread::sleep_for(std::chrono::seconds(1));
							std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current and previous fragment equal, both appended to written file" << "\n";
						}
					}
					else if (!aCurrentPair && aPreviousPair)
					{
						aFragmentWriter.printFragmentPairToEndOfFile(aPreviousPair);
						isPrintedCurrent = false;
						isPrintedPrevious = true;
						std::this_thread::sleep_for(std::chrono::seconds(1));
						std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Current fragment null, previous appended to written file" << "\n";
					}
					else if (aCurrentPair && !aPreviousPair)
					{
						aFragmentWriter.printFragmentPairToEndOfFile(aCurrentPair);
						isPrintedCurrent = true;
						isPrintedPrevious = false;
						std::this_thread::sleep_for(std::chrono::seconds(1));
						std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Previous fragment null, current appended to written file" << "\n";
					}
					else
					{
						std::this_thread::sleep_for(std::chrono::seconds(1));
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

				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Reading complete. Deleting read files" << "\n";
				
				aFileCounter++;
				aTimesVisited = 0;
			}

			aPreviousFileName = aIterator->path().string().substr(aIterator->path().string().find("\\") + 1);

			++aTimesVisited;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Finished inner loop" << "\n";

		//JOSH simplify this break check
		if (aFolderNum >= 4)
		{
			aPreviousMergesFinished =
				std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum))
				&& std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum - 1))
				&& std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum - 2))
				&& std::filesystem::is_empty("data/temp/MERGEME/" + std::to_string(aFolderNum - 3));

			std::this_thread::sleep_for(std::chrono::seconds(1));
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
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "DEBUG - " << std::to_string(aFolderNum) << ": Tiny finished check: " << aPreviousMergesFinished << "\n";
		}

		//JOSH add an exception check here for if there is one file in this folder and none in previous folders. If so, just move the file into the next folder
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "DEBUG: Mergefolder for folder " << std::to_string(aFolderNum) << " end" << "\n";
	//mergeFolder(aFolderNum + 4);
}

void mergeSort(const string& aInput)
{
	//setup directory structure
	std::filesystem::create_directories("data/temp/MERGEME");

	ThreadPool aPool(4);

	aPool.doJob(std::bind(splitFile, aInput));
	aPool.doJob(std::bind(mergeFolder, 1));
	aPool.doJob(std::bind(mergeFolder, 2));
	aPool.doJob(std::bind(mergeFolder, 3));

	

	/*
	FileThreadPool aThreadPool(4);

	FileThread aFileThread1;
	aFileThread1._threadId = 0;

	FileThread aFileThread2;
	aFileThread2._threadId = 1;

	FileThread aFileThread3;
	aFileThread3._threadId = 2;

	FileThread aFileThread4;
	aFileThread4._threadId = 3;

	aThreadPool.getThreadPool().push_back(aFileThread1);
	aThreadPool.getThreadPool().push_back(aFileThread2);
	aThreadPool.getThreadPool().push_back(aFileThread3);
	aThreadPool.getThreadPool().push_back(aFileThread4);

	aThreadPool.getThreadPool().at(0)._thread = thread(splitFile, aInput);
	aThreadPool.getThreadPool().at(1)._thread = thread(mergeFolder, aThreadPool, 1);
	aThreadPool.getThreadPool().at(2)._thread = thread(mergeFolder, aThreadPool, 2);
	aThreadPool.getThreadPool().at(3)._thread = thread(mergeFolder, aThreadPool, 3);
	*/

	/*
	aThreadPool.getThreadPool().emplace_back(splitFile, aInput);
	aThreadPool.getThreadPool().emplace_back(mergeFolder, aThreadPool, 1);
	aThreadPool.getThreadPool().emplace_back(mergeFolder, aThreadPool, 2);
	aThreadPool.getThreadPool().emplace_back(mergeFolder, aThreadPool, 3);
	*/
	
	//mergeFolder(aThreadPool);

	//teardown threadpool
	//aThreadPool.joinAllThreads();

}

int main()
{
	string aInput = "data/tiny-test.fastq";
	
	//fast(aInput);
	
	//lowMemory(aInput);

	mergeSort(aInput);

	//teardown directory structure
	//std::filesystem::remove_all("data/temp");
}
