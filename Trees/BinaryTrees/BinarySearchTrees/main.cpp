// BST.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include "RBTree.h"
#include "RankBST.h"
#include "WeightedBST.h"
#include "NULLBalancer.h"

typedef int INTEGER;
typedef std::vector<INTEGER> Vector;

//template <class T, typename comp = std::less<T> >
typedef trees::BST<int> BST;
typedef trees::BST<int>::BSTNode BSTNode;
typedef NullBalancer<int> Balancer;

bool isBST(trees::BST<int>::BSTNode* root)
{
	bool retVal = true;
	BSTNode* leftChild = root->getLeftChild();
	BSTNode* rightChild = root->getRightChild();
	if (NULL != leftChild)
		retVal &= (root->getInfo() > leftChild->getInfo() && isBST(leftChild));
	
	if(retVal && (NULL != rightChild))
		retVal &= (root->getInfo() < rightChild->getInfo() && isBST(rightChild));
	
	return retVal;

}

std::pair<bool,int> checkRBProperties(BSTNode* node)
{
	typedef std::pair<bool, int> pair;

	if (BST::RED == node->getColour())
	{
		BSTNode* left = node->getLeftChild();
		BSTNode* right = node->getRightChild();

		if (left && (BST::RED == left->getColour()))
			return pair(false, 0);
		else if (right && (BST::RED == right->getColour()))
			return pair(false, 0);
	}

	if (0 == node->getNumChildren())
		return pair(true, (node->getColour() == BST::BLACK)? 1 : 0);
	else if (1 == node->getNumChildren())
	{
		BSTNode* onlyChild = (NULL != node->getLeftChild()) ?
							  node->getLeftChild():
							  node->getRightChild();

		pair res = checkRBProperties(onlyChild);
		if (res.first)
			return pair(true, res.second + ((BST::BLACK == node->getColour()) ? 1 : 0));
		else
			return pair(false, 0);
	}
	else
	{
		BSTNode* left = node->getLeftChild();
		BSTNode* right = node->getRightChild();

		pair resLeft = checkRBProperties(left);
		pair resRight = checkRBProperties(right);

		if ( resLeft.first &&
			 resRight.first &&
			 (resLeft.second == resRight.second)
			)
			return pair(true, resLeft.second + ((BST::BLACK == node->getColour()) ? 1 : 0));
		else
			return pair(false, 0);
	}
}

bool isRBTrtee(BSTNode* root)
{
	bool  retVal = false;

	if (isBST(root))
		return (NULL != root) && (root->getColour() == BST::BLACK) && checkRBProperties(root).first;
}

void performInorder(BSTNode* root)
{
	if (NULL == root)
		return;
	else
	{
		performInorder(root->getLeftChild());
		std::cout << root->getInfo() << ", ";
		performInorder(root->getRightChild());
	}
}

void findLeackage(BSTNode* root, std::vector<BSTNode*>& leackagePath)
{
	if (NULL == root)
		return;
	else if (root->getColour() > 1 || root->getColour() < 0)
	{
		leackagePath.push_back(root);
		throw std::runtime_error("Leackage found");
	}
	else
	{
		try
		{
			findLeackage(root->getLeftChild(), leackagePath);
			findLeackage(root->getRightChild(), leackagePath);
		}
		catch (const std::exception& ex)
		{
			leackagePath.push_back(root);
			throw ex;
		}
	}
}

int main(int argc, char* argv[])
{
	/*std::ifstream in("test.txt");

	testinversionCount(in, std::cout);*/

	trees::DefaultBST<int> bst(new Balancer);

	std::unordered_set<int> unordered_set;

	while (1000 > unordered_set.size())
	{
		unordered_set.insert(rand());
	}

	for (auto item : unordered_set)
		bst.insert(item);

	if (isBST(bst.getRoot()))
		std::cout << "Is a bst"<<std::endl;
	else
		std::cout << "Isn't a bst" << std::endl;

	performInorder(bst.getRoot());

	std::vector<BSTNode*> leackagePath;
	int currDeleted;
	try
	{
		for (auto item : unordered_set)
		{
			bst.remove(item);
			currDeleted = item;
			if (NULL != bst.getRoot())
				std::cout << (isBST(bst.getRoot())? "Is" : "Isn't") << " a bst after deletion of " << item << std::endl;

				//performInorder(bst.getRoot());
		}
	}
	catch(const std::exception& ex)
	{
		rand();
	}

	return 0;
}

