#include <vector>
#include "Stacy.h"
#include "StacyTester.h"

StacyTester::StacyTester(const std::string& inputFile, const std::string& resultFile, std::string outputFile )
{
	_inFile = inputFile;
	_resFile = resultFile;
	_outFile = outputFile;
}


bool StacyTester::test()
{
	std::string str;
	std::ifstream fileIn(_inFile);
	std::ofstream fileOut(_outFile);
	std::getline(fileIn, str);
	long nQuestions = atol(str.substr(str.find_first_of(" ") + 1).c_str());
	std::vector<long long> ranks;

	std::getline(fileIn, str);

	Stacy st(str);

	while (nQuestions-- > 0)
	{
		std::getline(fileIn, str);
		ranks.push_back(atoll(str.c_str()));
	}

	for (UINT i = 0; i < ranks.size(); i++)
	{
		fileOut << atoll(st.getElementWithRank(ranks[i]).c_str()) <<std::endl;
	}

	fileIn.close();
	fileOut.close();

	auto res = _fileHandler.compareFiles(_resFile,_outFile);

	if (res.first)
		std::cout << "Test Pass";
	else
		std::cout << "Test fail, wrong answer for question "<<res.second;

	return res.first;
}
