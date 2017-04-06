#if !defined(_STACY_TESTER_H_)
#define _STACY_TESTER_H_

#include <string>
#include "UnitTester.h"
#include "FiileHandler.h"

class StacyTester : public UnitTester
{
	std::string _inFile;
	std::string _resFile;
	std::string _outFile;

	FiileHandler _fileHandler;

	//StacyTester() {};
public:

	StacyTester(const std::string& inputFile = "input.txt", const std::string& resultFile = "res.txt", std::string outputFile = "output.txt");

	virtual bool test();
};
#endif
