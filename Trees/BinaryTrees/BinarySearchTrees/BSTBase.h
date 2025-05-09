#if !defined(_BSTBASE_H_)
#define _BSTBASE_H_
#include <iostream>
#include <queue>
#include <stack>
#include <conio.h>

#if !defined(UINT)
typedef unsigned long long UINT;
#endif

#define UINT64_MAX UINT_MAX


#if !defined(INT)
typedef long long INT;
#endif


namespace trees
{
	template <class T, int numChildren >
	class MWayTree
	{
	public:
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
				  for( UINT i = 0; i < numChildren; i++)
				  {
					  _children[i] = NULL;
				  }

				  _parent = NULL;
			  }

			  UINT getNumChildren()
			  {
				  UINT counter = 0;
				  for( UINT i = 0; i < numChildren; i++)
				  {
					  counter += (getChild(i) != NULL)? 1 : 0;
				  }

				  return counter;
			  }

			  UINT whichChildAmI()
			  {
				  MWayTreeNode* parent = getParent();
				  if (NULL != parent)
				  {
					  UINT retVal = 0;

					  while (parent->getChild(retVal) != this)
						  retVal++;

					  return retVal;
				  }
				  else
					  throw std::runtime_error("Got no parent");
			  }

			  MWayTreeNode* getChild( UINT index )
			  {
				  return _children[index];
			  }

			  MWayTreeNode* getParent()
			  {
				  return _parent;
			  }


			  virtual void setChild( MWayTreeNode* node, UINT index )
			  {
				  _children[index] = node;
				  if(NULL != node)
				  {
					  node->_parent = this;
				  }
			  }

			  void setParent(MWayTreeNode* parent)
			  {
				  _parent = parent;
			  }

			  const T& getInfo()
			  {
				  return _info;
			  }

			  void setInfo(const T& info)
			  {
				  _info = info;
			  }
		};
	protected:
		
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
				for( UINT i = 0; i < numChildren; i++ )
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
			rem
			std::stack<MWayTreeNode* > stacks[2];
			stacks[0].push(_root);
			UINT currentIndex = 0;

			std::cout<<std::endl<<"BFS spiral order for this tree is:";
			while( !stacks[currentIndex].empty() )
			{
				UINT nextIndex = (currentIndex + 1) % 2;

				while(!stacks[currentIndex].empty())
				{
					for( UINT i = 0; i < numChildren; i++ )
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
	};

	template <class T, class comp=std::less<T> >
	class BST : public MWayTree<T,2>
	{
	protected:
		

	public:
		enum
		{
			LEFT = 0,
			RIGHT = 1,
			NONE = -1
		} CHILD_NUM;

		enum
		{
			RED = 0,
			BLACK = 1
		} COLOUR;

		class BSTNode : public  MWayTree<T,2>::MWayTreeNode
		{
		protected:
			UINT _colour;
			UINT _size;
			//For trees which allow duplicate elements
			UINT _weight;
			UINT _numRepetitions;

		public:

			BSTNode(T info):
				MWayTree<T,2>::MWayTreeNode(info)
			{
				_colour = RED;
				_size = _weight = _numRepetitions = 1;
			}

			UINT getColour()
			{
				return _colour;
			}

			void setColour(UINT colour)
			{
				_colour = colour;
			}

			UINT getSize()
			{
				return _size;
			}

			void setSize(UINT size)
			{
				_size = size;
			}

			UINT getWeight()
			{
				return _weight;
			}

			void setWeight(UINT weight)
			{
				_weight = weight;
			}

			UINT getNumRepetitions()
			{
				return _numRepetitions;
			}

			void setNumRepetitions(UINT numRepetitions)
			{
				_numRepetitions = numRepetitions;
			}

			BSTNode* getLeftChild()
			{
				return (BSTNode*)getChild(LEFT);
			}

			BSTNode* getRightChild()
			{
				return (BSTNode*)getChild(RIGHT);
			}
		};

		struct BSTNodeBalancer
		{
			virtual void balanceInsert(BST<T, comp>& bst, BSTNode* node) = 0;
			virtual void balanceDelete(BST<T, comp>& bst, BSTNode* node) = 0;
		};

		BSTNodeBalancer* _balancer;
		

		BST(BSTNodeBalancer* balancer):
			_balancer(balancer)
		{
		}

		UINT getSize(BSTNode* node)
		{
			if(NULL == node)
				return 0;
			else
				return node->getSize();
		}

		UINT getWeight(BSTNode* node)
		{
			if(NULL == node)
				return 0;
			else
				return node->getWeight();
		}

		void setWeight( BSTNode* node, UINT weight )
		{
			if( NULL != node)
				node->setWeight(weight);
		}

		UINT getNumRepetitions(BSTNode* node)
		{
			if(NULL == node)
				return 0;
			else
				return node->getNumRepetitions();
		}

		void setNumRepetitions( BSTNode* node, UINT numRepetitions)
		{
			if( NULL != node)
				node->setNumRepetitions(numRepetitions);
		}


		BSTNode* getParent(BSTNode* node)
		{
			return (BSTNode*)node->getParent();
		}

		BSTNode* getChild(BSTNode* node, UINT childNum)
		{
			return (BSTNode*)node->getChild(childNum);
		}

		void setParent(MWayTreeNode* node, MWayTreeNode* parent)
		{
			if(NULL != node)
				node->setParent(parent);
		}

		virtual void setChild(BSTNode* parentNode, BSTNode* childNode, UINT childNum)
		{
			if(NULL != parentNode)
				parentNode->setChild(childNode,childNum);
		}

		BSTNode* getSibling(BSTNode* node)
		{
			BSTNode* parent = getParent(node);
			BSTNode* retNode = NULL;

			if(NULL != parent)
			{
				if(getChild(parent,LEFT) == node)
					retNode = getChild(parent,RIGHT);
				else
					retNode = getChild(parent,LEFT);
			}

			return retNode;
		}

		BSTNode* getGrandParent(BSTNode* node)
		{
			BSTNode* parent = getParent(node);
			BSTNode* retNode = NULL;

			if(NULL != parent)
			{
				retNode = getParent(parent);
			}

			return retNode;
		}

		BSTNode* getUncle(BSTNode* node)
		{
			BSTNode* parent = getParent(node);
			BSTNode* grandParent = getGrandParent(node);
			BSTNode* retNode = NULL;

			if(NULL != grandParent)
			{
				if(grandParent->getChild(LEFT) == parent)
					retNode = getChild(grandParent,RIGHT);
				else
					retNode = getChild(grandParent,LEFT);
			}

			return retNode;
		}

		virtual std::pair<BSTNode*,bool> insert(T info)
		{
			std::pair<BSTNode*,bool> findRes = find( info );
			BSTNode* retNode = NULL;
			bool retVal = false;
			if(!findRes.second)
			{
				retNode = getBSTNode(info);
				BSTNode* parent = findRes.first;
				if(NULL == parent)
				{
					_root = retNode;
				}
				else 
				{
					insert( findRes.first, retNode);
				}

				retVal = true;
			}

			auto ret = std::pair<BSTNode*, bool>((NULL != retNode) ? retNode : findRes.first, retVal);
			if (ret.second)
				_balancer->balanceInsert(*this, ret.first);

			return ret;
		}

		virtual void insert(BSTNode* parent, BSTNode* child)
		{
			if( comp()(child->getInfo(), parent->getInfo()) )
				setChild(parent,child,LEFT);
			else
				setChild(parent,child,RIGHT);
		}

		virtual void leftRotate(BSTNode* node)
		{
			BSTNode* rightChild = getChild(node,RIGHT);
			BSTNode* parent = getParent(node);

			if( NULL != rightChild)
			{
				setChild(node,getChild(rightChild,LEFT), RIGHT);
				setChild(rightChild,node, LEFT);
				
				if(NULL != parent)
					setChild(parent,rightChild, node->whichChildAmI());
				else
					setParent(rightChild,NULL);

				if(node == _root)
					_root = rightChild;
			}
		}

		virtual void rightRotate(BSTNode* node)
		{
			BSTNode* leftChild = getChild(node,LEFT);
			BSTNode* parent = getParent(node);

			if( NULL != leftChild)
			{
				setChild(node,getChild(leftChild,RIGHT), LEFT);
				setChild(leftChild,node, RIGHT);

				if(NULL != parent)
					setChild(parent,leftChild, node->whichChildAmI());
				else
					setParent(leftChild,NULL);

				if(node == _root)
					_root = leftChild;
			}
		}

		std::pair<BSTNode*,bool> find(T info)
		{
			BSTNode* currNode = (BSTNode*)_root;
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
					currNode = getChild(currNode,LEFT);
				}
				else if(comp()(currNode->getInfo(), info))
				{
					retNode = currNode;
					currNode = getChild(currNode,RIGHT);
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

		virtual BSTNode* getBSTNode(T info) = 0;
		virtual void releaseNode(BSTNode* node) = 0;

		BSTNode* getLeftMostChild(BSTNode* node)
		{
			while (NULL != getChild(node, LEFT))
				node = getChild(node, LEFT);

			return node;
		}

		BSTNode* getInOrderSuccessor(BSTNode* node)	
		{
			BSTNode* currNode = node;
			if (NULL != getChild(currNode, RIGHT))
				return getLeftMostChild(getChild(currNode, RIGHT));
			else if (currNode->getParent() != NULL)
			{
				if (LEFT == node->whichChildAmI())
					return (BSTNode*)currNode->getParent();
				else
				{
					while (RIGHT == currNode->whichChildAmI())
						currNode = (BSTNode*)currNode->getParent();

					return (currNode != _root) ? (BSTNode*)currNode->getParent() : NULL;
				}
			}
		}


		void transPlant(BSTNode* n1, BSTNode* n2)
		{
			BSTNode* parent = getParent(n1);
			if (NULL == parent)
			{
				_root = n2;
			}
			else
				setChild(parent, n2, n1->whichChildAmI());
		}

		BSTNode* removeNodeWith2Children(BSTNode* node)
		{
			BSTNode* successor = getLeftMostChild(((BSTNode*)node)->getRightChild());
			BSTNode* retNode = successor->getRightChild();
			if (getParent(successor) != ((BSTNode*)node))
			{
				transPlant(successor, successor->getRightChild());
				setChild(successor, ((BSTNode*)node)->getRightChild(), RIGHT);
			}

			transPlant((BSTNode*)node, successor);
			setChild(successor, node->getLeftChild(), LEFT);

			return retNode;
		}

		virtual BSTNode* remove(BSTNode* node)
		{
			BSTNode* retNode = NULL;
			bool noChild = (0 == node->getNumChildren());
			bool oneChild = (1 == node->getNumChildren());
			bool botChildren = (2 == node->getNumChildren());

			if (node == _root)
			{
				if (noChild)
					_root = NULL;
				else if (oneChild)
					_root = (NULL != ((BSTNode*)_root)->getLeftChild()) ? ((BSTNode*)_root)->getLeftChild() : ((BSTNode*)_root)->getRightChild();
				else
					retNode = removeNodeWith2Children((BSTNode*)_root);
			}
			else
			{
				BSTNode* parent = getParent(node);
				if (noChild)
					setChild(parent, NULL, node->whichChildAmI());
				else if (oneChild)
				{
					BSTNode* onlyChild = (NULL != node->getLeftChild()) ?
						node->getLeftChild() :
						node->getRightChild();

					setChild(parent, onlyChild, node->whichChildAmI());

					retNode = onlyChild;
				}
				else
					retNode = removeNodeWith2Children(node);
			}

			releaseNode(node);

			if (NULL != _root)
				setParent(_root, NULL);

			return retNode;
		}

		virtual std::pair<BSTNode*, bool> remove(T info)
		{
			BSTNode* currNode = (BSTNode*)_root;
			BSTNode* retNode = NULL;
			std::pair<BSTNode*,bool> findRes = find(info);
			std::pair<BSTNode*, bool> retVal;
			if(!findRes.second)
				retVal = std::pair<BSTNode*, bool>(NULL, false);
			else
				retVal = std::pair<BSTNode*, bool>(remove(findRes.first), true);

			if (retVal.second)
				_balancer->balanceDelete(*this, retVal.first);

			return retVal;
		}
		//{
		//	bool exitLoop = false;
		//	BSTNode* currNode = (BSTNode*)_root;
		//	BSTNode* retNode = NULL;
		//	bool retVal = true;
		//	std::pair<BSTNode*,bool> findRes = find(info);
		//	if(!findRes.second)
		//	{
		//		retNode = NULL;
		//		retVal = false;
		//	}
		//	else
		//	{
		//		BSTNode* currNode = findRes.first;
		//		BSTNode* retNode = NULL;

		//		while (!exitLoop)
		//		{
		//			BSTNode* parent = getParent(currNode);
		//			if (0 == currNode->getNumChildren())//No children
		//			{
		//				if (NULL != parent)
		//					setChild(parent, NULL, currNode->whichChildAmI());

		//				releaseNode(currNode);
		//				retNode = parent;

		//				if (currNode == _root)
		//					_root = NULL;

		//				exitLoop = true;
		//			}
		//			else if (1 == currNode->getNumChildren())//1 child
		//			{
		//				BSTNode* onlyChild = (BSTNode*)((NULL != getChild(currNode, LEFT)) ? getChild(currNode, LEFT) : getChild(currNode, RIGHT));
		//				if (NULL != parent)
		//					setChild(parent, onlyChild, currNode->whichChildAmI());

		//				releaseNode(currNode);
		//				retNode = onlyChild;

		//				if (currNode == _root)
		//					_root = onlyChild;

		//				exitLoop = true;
		//			}
		//			else//Node has 2 children
		//			{
		//				BSTNode* inOrderSuccessor = getInOrderSuccessor(currNode);
		//				currNode->setInfo(inOrderSuccessor->getInfo());
		//				currNode = inOrderSuccessor;
		//			}
		//		}

		//		if(NULL != _root)
		//			_root->setParent(NULL);
		//	}

		//	return std::pair<BSTNode*,bool>(retNode, retVal);
		//}

		BSTNode* getRoot()
		{
			return (BSTNode*)_root;
		}

	};

	template <class T, class comp = std::less<T> >
	class DefaultBST : public BST<T,comp>
	{
	public:

		DefaultBST(BSTNodeBalancer* balancer):
			BST<T, comp>(balancer)
		{
		}
		virtual BSTNode* getBSTNode(T info)
		{
			return new BSTNode(info);
		}


		virtual void releaseNode(BSTNode* node)
		{
			delete node;
		}
	};
}

#endif