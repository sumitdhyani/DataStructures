#pragma once
#include "RBTree.h"
#include "RankBST.h"
#include "WeightedBST.h"


template<class T, class comp = std::less<T>>
struct NullBalancer : trees::BST<T, comp>::BSTNodeBalancer
{
	typedef trees::BST<int> BST;
	typedef BST::BSTNode BSTNode;
	virtual void balanceInsert(BST& bst, BSTNode* node) {}
	virtual void balanceDelete(BST& bst, BSTNode* node) {}
};
