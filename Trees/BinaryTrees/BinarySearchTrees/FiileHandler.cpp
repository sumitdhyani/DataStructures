#include "FiileHandler.h"

FiileHandler::FiileHandler()
{
}


std::ifstream& FiileHandler::getInputHandle(std::string fileName)
{
	auto itIn = _inputHandleStore.find(fileName);
	if (itIn != _inputHandleStore.end())
	{
		return itIn->second;
	}
	else
	{
		auto it = _inputHandleStore.insert(std::pair<std::string, std::ifstream>(fileName, std::ifstream(fileName)));
		return it.first->second;
	}
}

std::ofstream& FiileHandler::getOutputHandle(std::string fileName)
{
	auto itOut = _outputHandleStore.find(fileName);
	if (itOut != _outputHandleStore.end())
	{
		return itOut->second;
	}
	else
	{
		auto it = _outputHandleStore.insert(std::pair<std::string, std::ofstream>(fileName, std::ofstream(fileName)));
		return it.first->second;
	}
}


FiileHandler::~FiileHandler()
{
	for (auto itIn = _inputHandleStore.begin(); itIn != _inputHandleStore.end(); itIn++)
	{
		itIn->second.close();
	}

	for (auto itOut = _inputHandleStore.begin(); itOut != _inputHandleStore.end(); itOut++)
	{
		itOut->second.close();
	}
}


std::pair<bool,UINT> FiileHandler::compareFiles( const std::string& file1, const std::string& file2 )
{
	std::string line1;
	std::string line2;
	bool match = true;
	UINT lineNo = 0;
	UINT misMatchLine = 0;
	//char* str = 

	while (match && !getInputHandle(file1).eof() && !getInputHandle(file2).eof() )
	{
		lineNo++;
		std::getline(getInputHandle(file1), line1);
		std::getline(getInputHandle(file2), line2);

		match &= (line1.compare(line2) == 0);
	}

	if (getInputHandle(file1).eof() != getInputHandle(file2).eof())
	{
		match = false;
		lineNo++;
	}

	if (!match)
	{
		misMatchLine = lineNo;
	}

	return std::pair<bool, UINT>( match, misMatchLine );
}