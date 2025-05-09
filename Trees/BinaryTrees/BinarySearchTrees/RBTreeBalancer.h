#pragma once
#include "BSTBase.h"

template<class T, class comp=std::less<T>>
struct RBTTreeBalancer : trees::BST<T, comp>::BSTNodeBalancer
{
	typedef trees::BST<T, comp> BST;
	typedef BST::BSTNode BSTNode;
	virtual void balanceInsert(BST& bst, BSTNode* node) {}
	virtual void balanceDelete(BST& bst, BSTNode* node) {}
};
