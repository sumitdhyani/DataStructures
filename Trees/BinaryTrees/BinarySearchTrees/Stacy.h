#if !defined(_STACY_H_)
#define _STACY_H_
#include <iostream>
#include <unordered_map>
#include "string"
#include "WeightedBST.h"
#include "RBTree.h"

#define CHARACTER_ZERO '0'

typedef trees::WeightedBST<std::string, std::less<std::string>, trees::RBTree<std::string> > Base;

class Stacytree : public Base
{
	BSTNode* _lastFoundNode;
	UINT _positionInLastFoundNode;
	UINT _lastQueriedRank;
public:
	typedef trees::WeightedBST<std::string, std::less<std::string>, trees::RBTree<std::string> >::BSTNode BSTNode;

	bool isAncestor(BSTNode* n1, BSTNode* n2)
	{
		bool retVal = false;
		while(n2 != NULL)
		{
			if (n2 == n1)
			{
				retVal = true;
				break;
			}
			else
			{
				n2 = getParent(n2);
			}
		}

		return retVal;
	}

	void clearTree()
	{
		_root = _lastFoundNode = NULL;
		_positionInLastFoundNode = _lastQueriedRank = 0;
	}

	Stacytree()
	{
		_lastFoundNode = NULL;
		_positionInLastFoundNode = 0;
	}

	virtual BSTNode* getElementWithRank(UINT rank)
	{
		UINT currRank = rank;
		BSTNode* retNode = NULL;
		if (NULL != _lastFoundNode)
		{
			UINT numRepetitions = getNumRepetitions(_lastFoundNode);
			UINT startRank = _lastQueriedRank - _positionInLastFoundNode + 1;
			if ((rank - startRank + 1) <= numRepetitions)
			{
				_positionInLastFoundNode = rank - startRank + 1;
				retNode = _lastFoundNode;
			}
		}

		if (NULL == retNode)
		{
			retNode = Base::getElementWithRank(rank);
			_lastFoundNode = retNode;
			_positionInLastFoundNode =   rank - getNodeRank(retNode) + 1;
		}

		_lastQueriedRank = rank;
		return retNode;
	}

	void updateWeightsToTheParents(BSTNode* node, UINT incrementedWeight = 1)
	{

		BSTNode* currNode = node;

		if (NULL != currNode)
			setNumRepetitions(currNode, getNumRepetitions(currNode) + 1);

		while(NULL != currNode)
		{
			currNode->setWeight(currNode->getWeight() + incrementedWeight);
			currNode = getParent(currNode);
		}
	}
};

class Stacy
{
	typedef Stacytree::BSTNode BSTNode;
	typedef std::unordered_map<std::string,BSTNode*> NodeIndexList;
	typedef NodeIndexList::iterator iterator;
	Stacytree _stacyTree;
	std::string _testString;
	
	//_zeroCounter[i] tels us how many digits are there up to the ith digit
	std::vector<int> _zeroCounter;

	UINT _currentlegth;

	NodeIndexList _nodeIndexList;
	BSTNode* _lastFoundNode;
	UINT _positionInLastFoundNode;
public:
	std::string getElementWithRank(UINT rank)
	{	
		UINT nextLength = getNumDigitsInRank(rank);
		rank -= getRankUptoDigits(nextLength - 1);
		if(nextLength > _currentlegth)
		{
			_currentlegth = nextLength;
			_stacyTree.clearTree();
			for( UINT i = 0; i <= _testString.length() - _currentlegth;i++)
			{
				if((CHARACTER_ZERO != _testString[i]) || (1 == _currentlegth) )
				{
					std::string subStr = _testString.substr(i,_currentlegth);
					NodeIndexList::iterator it = _nodeIndexList.find(subStr);
					if( it == _nodeIndexList.end())
					{
						_nodeIndexList[subStr] = _stacyTree.insert(subStr).first;
					}
					else
					{
						_stacyTree.updateWeightsToTheParents(it->second);
					}
				}

			}
		}

		return _stacyTree.getElementWithRank(rank)->getInfo();

	}

	Stacy(const std::string& testString)
	{
		_testString = testString;
		_currentlegth = 0;
		for( UINT index = 0, numZeroesYet = 0;
			 index < _testString.length();
			 index ++
			)
		{
			if( _testString[index] == CHARACTER_ZERO )
				numZeroesYet++;

			_zeroCounter.push_back(numZeroesYet);
		}
	}


	UINT getRankUptoDigits(UINT numDigits)
	{
		if( 0 == numDigits)
			return 0;

		UINT currRank = _testString.length();
		UINT currNumDigits = 1;

		while(currNumDigits < numDigits)
		{
			currNumDigits++;
			currRank += _testString.length() - (currNumDigits  - 1) - _zeroCounter[_testString.length()- currNumDigits];
		}

		return currRank;
	}

	UINT getNumDigitsInRank(UINT rank)
	{
		UINT numDigits = 1;
		if(rank > _testString.length())
		{
			//We came here because the the desired rank is > _testString.length()
			//So, to save iterations, we start with currRank = _testString.length();
			UINT currRank = _testString.length();
			while(getRankUptoDigits(numDigits) < rank)
			{
				numDigits++;
			}
		}

		return numDigits;
	}
};

#endif