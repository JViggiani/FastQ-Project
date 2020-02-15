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
	std::string aQualityString = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	for (int i = 0; i < aQualityString.size() - 1; ++i)
	{
		char aQualityString1 = aQualityString.at(i);
		char aQualityString2 = aQualityString.at(i+1);
		
		Quality aQuality1(&aQualityString1);
		Quality aQuality2(&aQualityString2);

		bool isLess = (aQuality1 < aQuality2);

		ASSERT_TRUE(isLess);
	}
}

TEST(Quality, CanGetAsInt1)
{
	std::string aQualityString = "!" + '"' + (string)"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

	for (int i = 0; i < aQualityString.size() - 1; ++i)
	{
		string aQualityString1;
		aQualityString1.push_back(aQualityString.at(i));
		string aQualityString2;
		aQualityString.push_back(aQualityString.at(i + 1));

		Quality aQuality1(aQualityString1);
		Quality aQuality2(aQualityString2);

		int aQuality1Int = aQuality1.toInt();
		int aQuality2Int = aQuality2.toInt();

		ASSERT_EQ(aQuality2Int, aQuality1Int + 1);
	}
}