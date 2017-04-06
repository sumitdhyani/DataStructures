#if !defined(_RBTREE_H_)
#define _RBTREE_H_

#include "BSTBase.h"

namespace trees
{
	template <class T, class comp=std::less<T>, class Base = BST<T,comp> >
	class RBTree : public Base
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
		std::pair<BSTNode*,bool> insert(T info)
		{
			std::pair<BSTNode*,bool> insertRes = Base::insert(info);

			if(insertRes.second)
			{
				BSTNode* retNode = insertRes.first;
				BSTNode* parent	= getParent(insertRes.first);
				if( RED == getColour(parent) )
				{
					insertFixup(retNode);
				}

				((BSTNode*)_root)->setColour(BLACK);
			}

			return insertRes;
		}

		unsigned int getColour(BSTNode* node)
		{
			if(NULL == node)
				return BLACK;
			else
				return node->getColour();
		}

		void insertFixup(BSTNode* node)
		{
			BSTNode* currNode = node;
			bool exitLoop = false;
			while(!exitLoop)
			{
				BSTNode* parent = getParent(currNode);
				BSTNode* uncle = getUncle(currNode);
				BSTNode* grandParent = getGrandParent(currNode);

				if(BLACK == getColour(parent))
				{
					exitLoop = true;
				}
				else if(RED == getColour(uncle))
				{
					grandParent->setColour(RED);
					parent->setColour(BLACK);
					uncle->setColour(BLACK);

					currNode = grandParent;
				}
				else if(currNode->whichChildAmI() != parent->whichChildAmI())
				{
					(LEFT == currNode->whichChildAmI())?
						rightRotate(parent):
						leftRotate(parent);

					currNode = parent;
				}
				else
				{
					(LEFT == currNode->whichChildAmI())?
						rightRotate(grandParent):
						leftRotate(grandParent);

					parent->setColour(BLACK);
					grandParent->setColour(RED);

					exitLoop = true;
				}
			}
		}

		virtual std::pair<BSTNode*,bool> remove(T info)
		{
			std::pair<BSTNode*,bool> removeRes = Base::remove( info );
			if( NULL != removeRes.first )
			{
				deleteFixup( (BSTNode*)removeRes.first );
			}

			((BSTNode*)_root)->setColour(BLACK);

			return removeRes;
		}

		void deleteFixup(BSTNode* node)
		{
			unsigned int leftBlackHeight = getBlackHeight( getChild(node,LEFT));
			unsigned int rightBlackHeight = getBlackHeight( getChild(node,RIGHT));

			if(leftBlackHeight < rightBlackHeight)
				node = getChild(node,LEFT);
			else if(leftBlackHeight > rightBlackHeight)
				node = getChild(node,RIGHT);
			else
				node = NULL;




			while( (NULL != node) && (node != _root) && (BLACK == getColour(node)) )
			{
				BSTNode* parent = getParent(node);
				BSTNode* sibling = getSibling(node);

				if( RED  == sibling->getColour() )//case 1
				{
					parent->setColour(RED);
					sibling->setColour(BLACK);
					(RIGHT == sibling->whichChildAmI())?
						rightRotate(parent):
						leftRotate(parent);
				}
				else if( (BLACK  == getColour( getChild(sibling,LEFT) ) ) &&
						 (BLACK  == getColour( getChild(sibling,RIGHT) ) )
						)//Case 2
				{
					sibling->setColour(RED);
					node = getParent(node);
				}
				else  
				{
					if( BLACK == getColour((BSTNode*)(sibling->getChild( node->whichChildAmI() + 1 % 2) ) ) )//Case 3
					{
						(getChild(sibling, node->whichChildAmI()))->setColour( BLACK );
						sibling->setColour(RED);
						(RIGHT == sibling->whichChildAmI())?
							rightRotate(sibling):
							leftRotate(sibling);

						sibling = (BSTNode*)getSibling(node);
					}

					sibling->setColour(getColour(parent));
					parent->setColour(BLACK);
					(getChild(sibling, (node->whichChildAmI() + 1) % 2))->setColour(BLACK);
					(LEFT == node->whichChildAmI())?
						leftRotate(parent):
						rightRotate(parent);

					node = (BSTNode*)_root;
				}
			}
		}

		unsigned long getBlackHeight(BSTNode* node)
		{
			if( NULL == node )
				return 0;
			else
			{
				return ( ((BLACK == node->getColour())? 1 : 0)  
						 + std::max<unsigned long>(getBlackHeight((BSTNode*)node->getChild(LEFT)), getBlackHeight((BSTNode*)node->getChild(RIGHT))) 
						);
			}
		}

	};


	

}

#endif