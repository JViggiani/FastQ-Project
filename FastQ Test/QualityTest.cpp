#include "pch.h"

#include "../FastQ Project/Quality.h"
#include "../FastQ Project/Quality.cpp"

TEST(Quality, CanCompareQualitySameLength1) 
{
	std::string aQualityString1 = "#$%";
	std::string aQualityString2 = "|}~";
	
	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_TRUE(aQuality1 < aQuality2);
}

TEST(Quality, CanCompareQualitySameLength2)
{
	std::string aQualityString1 = ";Az";
	std::string aQualityString2 = "zA;";

	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_TRUE(aQuality1 < aQuality2);
}

TEST(Quality, CanCompareQualitySameLength3)
{
	std::string aQualityString1 = "ABC";
	std::string aQualityString2 = "ABD";

	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_TRUE(aQuality1 < aQuality2);
}

TEST(Quality, CanCompareQualitySameLength4)
{
	std::string aQualityString1 = "YYY";
	std::string aQualityString2 = "YYY";

	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_FALSE(aQuality1 < aQuality2);
}

TEST(Quality, CanCompareQualitySameLength5)
{
	std::string aQualityString1 = "\"";
	std::string aQualityString2 = "#";

	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_TRUE(aQuality1 < aQuality2);
}

TEST(Quality, CanCompareQualitySameLength6)
{
	std::string aQualityString = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	for (int i = 0; i < aQualityString.size() - 1; ++i)
	{
		string aQualityString1 = aQualityString.substr(i, 1);
		string aQualityString2 = aQualityString.substr(i + 1, 1);
		
		Quality aQuality1(aQualityString1);
		Quality aQuality2(aQualityString2);

		bool isLess = (aQuality1 < aQuality2);

		ASSERT_TRUE(isLess);
	}
}

TEST(Quality, CanCompareQualityDifferentLength1)
{
	std::string aQualityString1 = "#1=DBDFDHHHHGJIJJJJJIIIJI";
	std::string aQualityString2 = "CCCFFFFFHHHHHJIIIJJJIJ#4A";

	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_TRUE(aQuality1 < aQuality2);
}

TEST(Quality, CanCompareQualityDifferentLength2)
{
	std::string aQualityString1 = "#1=DBDFDHHHHGJIJJJJJIIIJI";
	std::string aQualityString2 = "@BCDDFEFHHDHHJJJJJIJJIJJJ";

	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_TRUE(aQuality1 < aQuality2);
}

TEST(Quality, CanCompareQualityDifferentLength3)
{
	std::string aQualityString1 = "#1=DBDFDHHHHGJIJJJJJIIIJI";
	std::string aQualityString2 = "@@@FFFFEHFHHHJJCGDHIIECD@";

	Quality aQuality1(aQualityString1);
	Quality aQuality2(aQualityString2);

	ASSERT_TRUE(aQuality1 < aQuality2);
}

TEST(Quality, CanGetAsInt1)
{
	std::string aQualityString = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	for (int i = 0; i < aQualityString.size() - 1; ++i)
	{
		int j = i + 1;
		
		string aQualityString1 = aQualityString.substr(i, 1);
		string aQualityString2 = aQualityString.substr(i+1, 1);

		if (aQualityString1.size() == 1 && aQualityString2.size() == 1)
		{
			Quality aQuality1(aQualityString1);
			Quality aQuality2(aQualityString2);

			int aQuality1Int = aQuality1.toInt();
			int aQuality2Int = aQuality2.toInt();

			ASSERT_EQ(aQuality2Int, aQuality1Int + 1);
		}
	}
}

TEST(Quality, CanAddQuality1)
{
	int aQuality1Value = 10;
	int aQuality2Value = 7;

	char aQuality1Char = (char)aQuality1Value;
	char aQuality2Char = (char)aQuality2Value;

	string aQuality1String;
	aQuality1String.push_back(aQuality1Char);
	string aQuality2String;
	aQuality2String.push_back(aQuality2Char);
	
	Quality aQuality1(aQuality1String);
	Quality aQuality2(aQuality2String);

	Quality aAddedQuality = aQuality1 + aQuality2;

	ASSERT_EQ(aAddedQuality.toInt(), aQuality1Value + aQuality2Value);
}

TEST(Quality, CanDivideQuality1)
{
	int aQuality1Value = 50;
	int divisor = 2;

	char aQuality1Char = (char)aQuality1Value;

	string aQuality1String;
	aQuality1String.push_back(aQuality1Char);

	Quality aQuality1(aQuality1String);

	Quality aDividedQuality = aQuality1 / divisor;

	ASSERT_EQ(aDividedQuality.toInt(), aQuality1Value / divisor);
}