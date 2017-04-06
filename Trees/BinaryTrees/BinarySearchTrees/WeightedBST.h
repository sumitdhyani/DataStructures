#if !defined(_STACY_BST_H_)
#define _STACY_BST_H_

#include "BSTBase.h"

namespace trees
{
	template <class T, class comp=std::less<T>, class Base = trees::BST<T,comp> >
	class WeightedBST : public Base
	{
	protected:
		typedef typename Base::BSTNode BSTNode ;

		virtual BSTNode* getBSTNode(T info)
		{
			return new BSTNode(info);
		}

		virtual void releaseNode(BSTNode* node)
		{
			delete node;
		}
	public:

		virtual void setChild(BSTNode* parentNode, BSTNode* childNode, UINT childNum)
		{
			if(parentNode)
			{
				Base::setChild(parentNode,childNode,childNum);
				setWeight(parentNode, getNumRepetitions(parentNode) + getWeight(getChild(parentNode,LEFT)) +  getWeight(getChild(parentNode,RIGHT)) );
			}
		}

		virtual std::pair<BSTNode*,bool> insert(T info)
		{
			std::pair<BSTNode*,bool> findRes = find(info);
			std::pair<BSTNode*,bool> insertRes;
			BSTNode* currNode = NULL;
			if(findRes.second)
			{
				currNode = findRes.first;
				setNumRepetitions(currNode, getNumRepetitions(currNode) + 1 );
			}
			else 
			{
				insertRes = Base::insert(info);
				currNode = getGrandParent(insertRes.first);
			}

			while(NULL != currNode)
			{
				currNode->setWeight(currNode->getWeight() + 1);
				currNode = getParent(currNode);
			}

			if(findRes.second)
				return std::pair<BSTNode*,bool>(NULL, false);
			else
				return insertRes;
		}

		virtual std::pair<BSTNode*,bool> remove(T info)
		{
			std::pair<BSTNode*,bool> findRes = find(info);
			if(findRes.second)
			{
				BSTNode* foundNode = findRes.first;
				foundNode->setWeight(foundNode->getWeight() - 1);

				BSTNode* currNode = findRes.first;
				while(NULL != currNode)
				{
					currNode->setWeight(currNode->getWeight() - 1);
					currNode = getParent(currNode);
				}

				if(foundNode->getWeight() == 0)
				{
					return Base::remove(info);
				}
				else
				{
					return std::pair<BSTNode*,bool>(NULL,false);
				}
			}
			else 
				return std::pair<BSTNode*,bool>(NULL,false);
		}

		virtual UINT getRankInMySubTree(BSTNode* node)
		{
			if(NULL == node)
				return 0;
			else
				return ( getWeight(getChild(node,LEFT)) + 1);
		}

		virtual BSTNode* getElementWithRank(UINT rank )
		{
			BSTNode* currNode = (BSTNode*)_root;
			UINT currRank = rank;

			bool exitLoop = false;
			while(!exitLoop)
			{
				if( NULL == currNode)
				{
					exitLoop = true;
				}
				else if(currRank >= getRankInMySubTree(currNode) )
				{
					if( doIHoldThisRankInMySubTree(currNode,currRank) )
						exitLoop = true;
					else
					{
						currRank -= ( getRankInMySubTree(currNode) + getNumRepetitions(currNode) - 1);
						currNode = getChild(currNode,RIGHT);
					}
				}
				else
				{
					currNode = getChild(currNode,LEFT);
				}
			}

			return currNode;
		}

		virtual bool doIHoldThisRankInMySubTree(BSTNode* node, UINT rank)
		{
			UINT startRank = getRankInMySubTree(node);
			UINT endRank = startRank +  getNumRepetitions(node) - 1;

			return ( (rank >= startRank) && (rank <= endRank) );
		}

		std::pair<BSTNode*, bool> find(T info)
		{
			BSTNode* currNode = (BSTNode*)_root;
			BSTNode* retNode = NULL;
			bool retVal = false;

			bool exitLoop = false;
			while (!exitLoop)
			{
				if (currNode == NULL)
				{
					exitLoop = true;
				}
				else if (comp()(info, currNode->getInfo()))
				{
					retNode = currNode;
					currNode = getChild(currNode, LEFT);
				}
				else if (comp()(currNode->getInfo(), info))
				{
					retNode = currNode;
					currNode = getChild(currNode, RIGHT);
				}
				else
				{
					retNode = currNode;
					retVal = true;
					exitLoop = true;
				}
			}

			return std::pair<BSTNode*, bool>(retNode, retVal);
		}


		UINT getNodeRank(BSTNode* node)
		{
			UINT currRank = 0;
			BSTNode* currNode = (BSTNode*)_root;
			BSTNode* retNode = NULL;
			bool retVal = false;

			bool exitLoop = false;
			while (!exitLoop)
			{
				if (currNode == NULL)
				{
					exitLoop = true;
				}
				else if (comp()(node->getInfo(), currNode->getInfo()))
				{
					retNode = currNode;
					currNode = getChild(currNode, LEFT);
				}
				else if (comp()(currNode->getInfo(), node->getInfo()))
				{
					currRank += getRankInMySubTree(currNode) + getNumRepetitions(currNode);
					retNode = currNode;
					currNode = getChild(currNode, RIGHT);
				}
				else
				{
					retNode = currNode;
					retVal = true;
					exitLoop = true;
				}
			}

			return currRank;
		}
	};
}
#endif
