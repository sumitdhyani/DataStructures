// BST.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <algorithm>
#include <queue>
#include <stack>
#include <conio.h>
#include <functional>
#include "BST.h"

template <class T, class comp=std::less<T> >
class BST : public trees::BST<int,comp>
{
protected:
	typedef trees::BST<T,comp>::BSTNode BSTNode;
public:
	BSTNode* insert(T info)
	{
		BSTNode* newNode = new BSTNode(info);
		if( trees::BST<T,comp>::insert(newNode))
			return newNode;
		else
		{
			delete newNode;
			return NULL;
		}
	}
};



template <class T>
void makeInsertions(BST<T> &bst)
{
	std::cout<<std::endl<<"Insertion process started , enter value -999 whenever you want to exit insertion process";

	
	while(true)
	{
		T info = 0;
		std::cout<<std::endl<<"Enter next element: ";
		std::cin>>info;
		if( -999 == info)
			break;

		if( NULL != bst.insert( info ))
		{
			std::cout<<std::endl<<"Element "<<info<<" inserted successfully";
		}
		else
		{
			std::cout<<std::endl<<"Element "<<info<<" not inserted, possibly duplicate element";
		}
	}

	std::cout<<std::endl<<"Exiting insertion process process";
}


template <class T>
void makeDeletions(BST<T> &bst)
{
	std::cout<<std::endl<<"Deletion process started , enter value -999 whenever you want to exit deletion process";

	
	while(true)
	{
		T info = 0;
		std::cout<<std::endl<<"Enter next element: ";
		std::cin>>info;
		if( -999 == info)
			break;

		if( bst.remove( info ).second)
		{
			std::cout<<std::endl<<"Element "<<info<<" deleted successfully";
		}
		else
		{
			std::cout<<std::endl<<"Element "<<info<<" not deleted, possibly non-existent element";
		}
	}

	std::cout<<std::endl<<"Exiting deletion process process";
}

int main(int argc, char* argv[])
{
	BST<int> bst;
	makeInsertions<int>(bst);
	makeDeletions<int>(bst);
	bst.performBfsTraversal();
	bst.performSpiralBfsTraversal();
	getch();
	return 0;
}

