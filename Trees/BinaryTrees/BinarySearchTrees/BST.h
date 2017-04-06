#if !defined(_BST_H_)
#define _BST_H_
#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <conio.h>
#include <functional>


namespace trees
{
	template <class T, int numChildren >
	class MWayTree
	{
	protected:
		class MWayTreeNode
		{
		protected:
			T _info;
			MWayTreeNode* _children[numChildren];
			MWayTreeNode* _parent;

		public:
			MWayTreeNode(T info):
				_info(info)
			{
				for( unsigned int i = 0; i < numChildren; i++)
				{
					_children[i] = NULL;
				}

				_parent = NULL;
			}

			unsigned int getNumChildren()
			{
				unsigned int counter = 0;
				for( unsigned int i = 0; i < numChildren; i++)
				{
					counter += (getChild(i) != NULL)? 1 : 0;
				}

				return counter;
			}

			MWayTreeNode* getChild( unsigned int index )
			{
				return _children[index];
			}

			MWayTreeNode* getParent()
			{
				return _parent;
			}


			virtual void setChild( MWayTreeNode* node, unsigned int index )
			{
				_children[index] = node;
				if(NULL != node)
				{
					node->_parent = this;
				}
			}

			T& getInfo()
			{
				return _info;
			}
		};

		MWayTreeNode* _root;

	public:
		MWayTree()
		{
			_root = NULL;
		}

		MWayTreeNode* getRoot()
		{
			return _root;
		}

		void performBfsTraversal()
		{
			if(NULL == _root)
				return;

			std::queue<MWayTreeNode*> localQueue;
			localQueue.push(_root);

			std::cout<<std::endl<<"BFS order for this tree is:";
			while(!localQueue.empty())
			{
				for( unsigned int i = 0; i < numChildren; i++ )
				{
					if(NULL != localQueue.front()->getChild(i))
						localQueue.push(localQueue.front()->getChild(i));
				}

				std::cout<<localQueue.front()->getInfo()<<" : ";
				localQueue.pop();
			}
		}


		void performSpiralBfsTraversal()
		{
			if(NULL == _root)
				return;

			std::stack<MWayTreeNode* > stacks[2];
			stacks[0].push(_root);
			unsigned int currentIndex = 0;

			std::cout<<std::endl<<"BFS spiral order for this tree is:";
			while( !stacks[currentIndex].empty() )
			{
				unsigned int nextIndex = (currentIndex + 1) % 2;

				while(!stacks[currentIndex].empty())
				{
					for( unsigned int i = 0; i < numChildren; i++ )
					{

						MWayTreeNode* nextChild = (0 == (currentIndex%2))?
												  (stacks[currentIndex].top())->getChild(i) :
												  (stacks[currentIndex].top())->getChild(numChildren - 1 - i);
						if( NULL != nextChild)
						{
							stacks[nextIndex].push(nextChild);
						}
					}

					std::cout<<(stacks[currentIndex].top())->getInfo()<<" : ";;

					stacks[currentIndex].pop();
				}

				currentIndex = nextIndex;
			}
		}

		//virtual MWayTreeNode<T,numChildren>* insert(T info) = 0;

		//virtual std::pair<MWayTreeNode<T,numChildren>*,bool> remove(T info) = 0;
	};

	template <class T, class comp=std::less<T> >
	class BST : public MWayTree<T,2>
	{

	protected:
		enum
		{
			LEFT = 0,
			RIGHT = 1
		};

		typedef  MWayTree<T,2>::MWayTreeNode BSTNode;

	public:

		bool insert(BSTNode* node)
		{
			bool retVal = false;
			std::pair<MWayTreeNode*,bool> findRes = find( node->getInfo() );
			if(!findRes.second)
			{
				if(NULL == findRes.first)
				{
					_root = node;
				}
				else
				{
					if(comp()(node->getInfo(), findRes.first->getInfo()) )
						findRes.first->setChild(node, LEFT);
					else
						findRes.first->setChild(node, RIGHT);
				}
				retVal = true;
			}

			return retVal;
		}

		std::pair<BSTNode*,bool> find(T info)
		{
			BSTNode* currNode = _root;
			BSTNode* retNode = NULL;
			bool retVal = false;

			bool exitLoop = false;
			while(!exitLoop)
			{
				if( currNode == NULL )
				{
					exitLoop = true;
				}
				else if(comp()(info, currNode->getInfo()))
				{
					retNode = currNode;
					currNode = currNode->getChild(LEFT);
				}
				else if(comp()(currNode->getInfo(), info))
				{
					retNode = currNode;
					currNode = currNode->getChild(RIGHT);
				}
				else
				{
					retNode = currNode;
					retVal = true;
					exitLoop = true;
				}
			}

			return std::pair<BSTNode*,bool>(retNode,retVal);
		}

		BSTNode* getInOrderSuccessor(BSTNode* node)	
		{
			BSTNode* currNode = node;
			if(NULL != currNode->getChild(RIGHT))
				currNode = currNode->getChild(RIGHT);
			else if(NULL != currNode->getParent())
			{
				currNode = currNode->getParent();
			}
			else
				currNode = NULL;


			while( (NULL != currNode) && 
				   (node != currNode->getChild(LEFT)) && 
				   (NULL != currNode->getChild(LEFT))
				  )
			{
				currNode = currNode->getChild(LEFT);
			}

			return currNode;
		}

		std::pair<BSTNode*,bool> remove(T info)
		{
			bool exitLoop = false;
			BSTNode* currNode = _root;
			BSTNode* retNode = NULL;
			bool retVal = true;
			std::pair<BSTNode*,bool> findRes = find(info);
			if(!findRes.second)
			{
				retNode = NULL;
				retVal = false;
			}
			else
			{
				BSTNode* currNode = findRes.first;
				BSTNode* parent = currNode->getParent();
				BSTNode* retNode = NULL;
				if( 0 == currNode->getNumChildren())//No children
				{
					if(NULL != parent)
						(parent->getChild(LEFT) == currNode)? parent->setChild(NULL, LEFT) : parent->setChild(NULL, RIGHT);

					delete currNode;
					retNode = parent;

					if(currNode == _root)
						_root = NULL;
				}
				else if(1 == currNode->getNumChildren())//1 child
				{
					BSTNode* onlyChild = (NULL != currNode->getChild(LEFT))? currNode->getChild(LEFT) : currNode->getChild(RIGHT);
					if(NULL != parent)
						(parent->setChild(onlyChild, (parent->getChild(LEFT) == onlyChild)?LEFT:RIGHT)) ;

					delete currNode;
					retNode = parent;

					if(currNode == _root)
						_root = onlyChild;
				}
				else//Node has 2 children
				{
					BSTNode* inOrderSuccessor = getInOrderSuccessor(currNode);
					BSTNode* inOrderSuccessorParent = inOrderSuccessor->getParent();

					if( inOrderSuccessor == inOrderSuccessorParent->getChild(LEFT))//Inorder successor's parent will never be NULL inthis scenario
						inOrderSuccessorParent->setChild(inOrderSuccessor->getChild(RIGHT), LEFT);
					else
						inOrderSuccessorParent->setChild(inOrderSuccessor->getChild(RIGHT), RIGHT);

					inOrderSuccessor->setChild(currNode->getChild(LEFT),LEFT);

					if( inOrderSuccessor != currNode->getChild(RIGHT) )
						inOrderSuccessor->setChild(currNode->getChild(RIGHT), RIGHT);

					if(NULL != parent)
					{
						parent->setChild(inOrderSuccessor, (parent->getChild(LEFT) == currNode)?LEFT:RIGHT);
					}

					delete currNode;
					retNode = inOrderSuccessorParent;

					if(currNode == _root)
						_root = inOrderSuccessor;
				}

				exitLoop = true;
			}

			return std::pair<BSTNode*,bool>(retNode, retVal);
		}

	};


}

#endif