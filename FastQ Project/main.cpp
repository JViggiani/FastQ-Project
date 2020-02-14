// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

int main()
{
	std::cout << "Hello World!\n";
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
