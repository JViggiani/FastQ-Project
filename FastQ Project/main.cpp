// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>

#include "Fragment.h"

Fragment* populateNextFragment(std::ifstream& aInputStream)
{
	Fragment* aFragment = new Fragment();
	int aFragmentLineNum = 1;
	for (string line; 
		aFragmentLineNum <= 4 && getline(aInputStream, line) && !aInputStream.eof(); 
		++aFragmentLineNum)
	{
		cout << line << "\n";

		//File may contain blank lines, added for some resilience
		if (line.size() == 0)
		{
			--aFragmentLineNum;
			continue;
		}

		switch (aFragmentLineNum)
		{
		case 1:
			aFragment->aSeqId = new char[line.length() + 1];
			strcpy(aFragment->aSeqId, line.c_str());
			break;
		case 2:
			aFragment->aRawSequence = new char[line.length() + 1];
			strcpy(aFragment->aRawSequence, line.c_str());
			break;
		case 3:
			break;
		case 4:
			aFragment->aQualityValue = new char[line.length() + 1];
			strcpy(aFragment->aQualityValue, line.c_str());
			break;
		}
	}

	if (aFragment && aFragment->aSeqId && aFragment->aRawSequence && aFragment->aQualityValue)
	{
		//Only want to return complete fragments
		return aFragment;
	}
	else return nullptr;
}

//This is not neccessarily a fixed length in real data but it is 


int main()
{
	//std::cout << "Hello World!\n";
	//https://stackoverflow.com/questions/41245157/unable-to-sort-a-large-file-containing-integers
	//https://stackoverflow.com/questions/30618424/c-reading-large-files-part-by-part
	//https://stackoverflow.com/questions/22101347/parsing-large-file-in-c

	//psuedocode:
	/*
	handle opening of file, check it exists etc otherwise throw error
	
	start reading the file, count 8 newlines and allocate first 4 into one object, last 4 into another object. Then link them together with a struct of pointers and an average weight
	struct added to a multiset on the heap 

	when end of file is reached, start printing the new file 
		foreach item in multiset, start blurting out the data inside 
		delete object from memory as you go
	*/

	//what we know about the format:
	/*
	mate pairs - opposite ends of the same dna fragment
		same name with a trailing /1 or /2 indicator
		these occur adjacently
	base quality: each segment has a quality dictated by arbitrary characters - interpret as binary
	four lines:
		some random machine or date information, but importantly contains "/1" or "/2"
		DNA 
		+
		Quality
	*/

	string aFileName = "data/tiny-test.fastq";

	std::ifstream aInputStream(aFileName);

	//string aLine;

	//getline(aInputStream, aLine);

	//cin.ignore(numeric_limits<streamsize>::max(), '\n');

	/*
	Fragment* aFragment = new Fragment();
	//Sequence Id
	aInputStream.getline(aFragment->aSeqId, aSeqIdLength);
	//Sequence
	string aSeq;
	getline(aInputStream, aSeq);
	aFragment->aRawSequence = new char[aSeq.length()+1];
	strcpy(aFragment->aRawSequence, aSeq.c_str());

	cout << aFragment->aSeqId;
	cout << aFragment->aRawSequence;
	*/

	vector<Fragment*> aFragmentVector;

	while (!aInputStream.eof())
	{
		Fragment* aFragment1 = populateNextFragment(aInputStream);
		if (aFragment1)
		{
			aFragmentVector.push_back(aFragment1);
		}
	}

	cout << aFragmentVector.size();

	aInputStream.close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file