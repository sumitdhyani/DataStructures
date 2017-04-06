#if !defined(_RANK_BST_H_)
#define _RANK_BST_H_

#include "BSTBase.h"
namespace trees
{
	template <class T, class comp=std::less<T>, class Base = BST<T,comp> >
	class RankBST : public Base
	{
	protected:
		typedef typename Base::BSTNode BSTNode;

		virtual BSTNode* getBSTNode(T info)
		{
			return new BSTNode(info);
		}

		virtual void releaseNode(BSTNode* node)
		{
			delete node;
		}
	public:
		virtual void setChild(BSTNode* parentNode, BSTNode* childNode, unsigned int childNum)
		{
			Base::setChild(parentNode,childNode,childNum);

			if(parentNode)
				parentNode->setSize(getSize(getChild(parentNode,LEFT)) + getSize(getChild(parentNode,RIGHT)) + 1 );
		}

		virtual unsigned int getRankInMySubTree(BSTNode* node)
		{
			if(NULL == node)
				return 0;
			else
				return ( getSize(getChild(node,LEFT)) + 1 );
		}

		virtual unsigned int getElementRank(T info)
		{
			std::pair<BSTNode*,bool> findRes = find(info);
			unsigned int rank = 0;
			if(findRes.second)
			{
				rank = getRankInMySubTree(findRes.first);
			}

			return rank;
		}

		virtual BSTNode* getElementWithRank(unsigned int rank )
		{
			BSTNode* currNode = (BSTNode*)_root;
			unsigned int currRank = rank;

			bool exitLoop = false;
			while(!exitLoop)
			{
				if( (currNode == NULL) ||  (getRankInMySubTree(currNode) == currRank) )
				{
					exitLoop = true;
				}
				else if(getRankInMySubTree(currNode) > currRank)
				{
					currNode = getChild(currNode,LEFT);
				}
				else
				{
					currRank -= getRankInMySubTree(currNode);
					currNode = getChild(currNode,RIGHT);
				}
			}

			return currNode;
		}

		virtual  std::pair<BSTNode*,bool> insert(T info)
		{
			std::pair<BSTNode*,bool>  insertRes = Base::insert(info);
			BSTNode* insertedNode = insertRes.first;
			BSTNode* grandParent = (NULL != insertedNode)? getGrandParent(insertedNode) : NULL;

			while(NULL != grandParent)
			{
				grandParent->setSize(grandParent->getSize() + 1);
				grandParent = getParent(grandParent);
			}

			return insertRes;
		}

		virtual std::pair<BSTNode*,bool> remove(T info)
		{
			std::pair<BSTNode*,bool> removeRes = Base::remove(info);
			BSTNode* removedNode = removeRes.first;
			BSTNode* parent = (NULL != removedNode)? getParent(removedNode) : NULL;

			while(NULL != parent)
			{
				parent->setSize(parent->getSize() - 1);
				parent = getParent(parent);
			}

			return removeRes;
		}
	};
}

#endif