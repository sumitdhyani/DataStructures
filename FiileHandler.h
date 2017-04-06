#pragma once
#include <string>
#include <map>
#include <fstream>

typedef unsigned long long UINT;

//Not thread safe so the user of this class will have to take care of multi-threaded issues
class FiileHandler
{
	std::map<std::string, std::ifstream> _inputHandleStore;
	std::map<std::string, std::ofstream> _outputHandleStore;

public:

	std::pair<bool, UINT> compareFiles(const std::string& file1, const std::string& file2);
	std::ifstream& getInputHandle(std::string fileName);
	std::ofstream& getOutputHandle(std::string fileName);

	FiileHandler();
	~FiileHandler();

	typedef std::ifstream InputHandle;
	typedef std::ofstream OutputHandle;
};

