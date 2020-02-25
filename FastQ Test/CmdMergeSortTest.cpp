#include "pch.h"

/*
#include <functional>
#include <fstream>
#include <filesystem>
#include <stdio.h>

#include "CmdMergeSort.h"
#include "CmdMergeSort.cpp"

TEST(CmdMergeSort, CanMergeFiles)
{
	//create files
	ofstream aFilewriter1("data/CanMergeFiles/2/2_1", std::ofstream::out);
	aFilewriter1 << "@machine1:HiMom:abcdeACXX:1:1101:1071:2233/1 1:N:0:TATCCAGG"
		<< "\n"
		<< "TTTGACAGTCTCTGAATGAGAANGG"
		<< "\n"
		<< "+"
		<< "\n"
		<< "CCCFFFFFHHHHHJIIIJJJIJ#4A"
		<< "\n"
		<< "@machine1:HiMom:abcdeACXX:1:1101:1071:2233/2 2:N:0:TATCCAGG"
		<< "\n"
		<< "NNNNNNNNNNNNNNNNN"
		<< "\n"
		<< "+"
		<< "\n"
		<< "#################"
		<< "\n"
		<< "@machine1:HiMom:abcdeACXX:1:1201:1140:2125/1 1:N:0:TATCCAGG"
		<< "\n"
		<< "NTTTCAGTTCAGAGAACTGCAGAAT"
		<< "\n"
		<< "+"
		<< "\n"
		<< "#1=DBDFDHHHHGJIJJJJJIIIJI"
		<< "\n"
		<< "@machine1:HiMom:abcdeACXX:1:1201:1140:2125/2 2:N:0:TATCCAGG"
		<< "\n"
		<< "TTGGTCTTAGATGTTGC"
		<< "\n"
		<< "+"
		<< "\n"
		<< "HHHHFGIJIIIJIJIJJ"
		<< "\n";
	aFilewriter1.close();

	ofstream aFilewriter2("data/CanMergeFiles/2/2_2", std::ofstream::out);
	aFilewriter2
		<< "@machine1:HiMom:abcdeACXX:1:1201:1236:2187/1 1:N:0:TATCCAGG"
		<< "\n"
		<< "TTTAAATGGGTAAGAAGCCCGGCTC"
		<< "\n"
		<< "+"
		<< "\n"
		<< "@BCDDFEFHHDHHJJJJJIJJIJJJ"
		<< "\n"
		<< "@machine1:HiMom:abcdeACXX:1:1201:1236:2187/2 2:N:0:TATCCAGG"
		<< "\n"
		<< "CGGATTCCGACTTCCAT"
		<< "\n"
		<< "+"
		<< "\n"
		<< "HHHHGIJJIGIGIJJGG"
		<< "\n"
		<< "@machine1:HiMom:abcdeACXX:1:2101:1133:2239/1 1:N:0:TATCCAGG"
		<< "\n"
		<< "AGACAGAAGTACGGGAAGGCGAAGA"
		<< "\n"
		<< "+"
		<< "\n"
		<< "@@@FFFFEHFHHHJJCGDHIIECD@"
		<< "\n"
		<< "@machine1:HiMom:abcdeACXX:1:2101:1133:2239/2 2:N:0:TATCCAGG"
		<< "\n"
		<< "GTTTCCTAGCTTGTCTT"
		<< "\n"
		<< "+"
		<< "\n"
		<< "HHHHF4ACFHIJHHHGH"
		<< "\n";
	aFilewriter2.close();
	
	CmdMergeSort aMerger("junk");

	int i;

	aMerger.mergeFiles("data/2/2_1", "data/2/2_2", 2, i);

}
*/