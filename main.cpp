// BST.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <stack>
#include <conio.h>
#include <functional>
#include "RBTree.h"
#include "RankBST.h"
#include "WeightedBST.h"
#include "Stacy.h"
#include "FiileHandler.h"
#include "StacyTester.h"

trees::WeightedBST<int> t;
void operator *=(std::string& str, UINT num);
void operator ++(std::string& str);
bool operator <(UINT num, const std::string& str);
bool operator >(UINT num, const std::string& str);
bool operator <=(UINT num, const std::string& str);
bool operator >=(UINT num, const std::string& str);
bool operator <(const std::string& str, UINT num);
bool operator >(const std::string& str, UINT num);
void operator -=(std::string& str, UINT num);
UINT getNumLen(UINT num);
UINT getDiitAtPosition(UINT num, UINT position);
UINT getDigitFormCharacter(char digit);

#define ASCII_CHARACTER_TO_DIGIT_OFFSET 48

class Convertor
{
	UINT _modulus;
	UINT _numDigitsInModulus;
	UINT _modulusFirstDigit;
public:
	Convertor(UINT modulus)
	{
		_modulus = modulus;
		_numDigitsInModulus = getNumLen(_modulus);
		_modulusFirstDigit = _modulus/ pow(10, _numDigitsInModulus -1);
	}

	void operator()(std::string& num)
	{
		UINT sum = 0;
		UINT numLen = num.length();
		for(UINT currentNumDigits = _numDigitsInModulus; currentNumDigits <= numLen; currentNumDigits++)
		{
			int temp = _modulus;

			temp *= pow(10, numLen - currentNumDigits);

			UINT start = 0;
			UINT end = 10;

			while (start < end)
			{
				UINT mid = (start + end) / 2;
				if ( sum + temp * mid <= num)
				{
					start = mid;
				}
				else
				{
					end = mid;
				}

				if (start == end - 1)
					end = start;
			}

			sum += temp * start;
		}

		num -= sum;
	}

};

void operator *=(std::string& str, UINT num)
{
	UINT numLen = getNumLen(num);

	while (str.length() <= numLen)
	{
		str = "0" + str;
	}

	str = "0" + str;

	UINT strLen = str.length();
	UINT res = 0;
	UINT carry = 0;

	for (UINT i = 1; i <= numLen || carry != 0; i++)
	{
		UINT res = getDigitFormCharacter(str[strLen - i]) + carry;
		if (i <= numLen)
			res += getDiitAtPosition(num, i);
		UINT temp = res;
		res %= 10;

		str[strLen - i] = res + ASCII_CHARACTER_TO_DIGIT_OFFSET;
		carry = temp / 10;
	}


	UINT firstNotZero = str.find_first_not_of(CHARACTER_ZERO);
	if (firstNotZero != std::string::npos)
		str = str.substr(firstNotZero, str.length() - firstNotZero + 1);
	else
		str = "0";
}

void operator ++(std::string& str)
{
	str *=1;
}


UINT getDiitAtPosition(UINT num, UINT position)
{
	num %= (UINT)pow(10, position);
	num /= (UINT)pow(10, position - 1);
	return num;
}

UINT getNumLen(UINT num)
{
	UINT len = 1;
	while( (num /= 10) > 0 )
		len++;

	return len;

}

bool operator <(UINT num, const std::string& str)
{
	UINT len = str.length();
	UINT numLen = getNumLen(num);
	if (len < numLen)
		return false;
	else if (numLen < len)
		return true;

	for (UINT i = len; i >= 1; i--)
	{
		UINT digitI = getDiitAtPosition(num, i);
		if (digitI != getDigitFormCharacter(str[len - i]))
			return digitI < getDigitFormCharacter(str[len - i]);
	}

}

bool operator >(UINT num, const std::string& str)
{
	UINT len = str.length();
	UINT numLen = getNumLen(num);
	if (len > numLen)
		return false;
	else if (numLen > len)
		return true;

	for (UINT i = len; i >= 1; i--)
	{
		UINT digitI = getDiitAtPosition(num, i);
		if (digitI != getDigitFormCharacter(str[len - i]))
			return digitI > getDigitFormCharacter(str[len - i]);
	}

}


bool operator <=(UINT num, const std::string& str)
{
	return !(num > str);
}

bool operator >=(UINT num, const std::string& str)
{
	return !(num < str);
}


bool operator >(const std::string& str, UINT num)
{
	return !(num >= str);
}

bool operator <(const std::string& str, UINT num)
{
	return !(num <= str);
}

//Assumption is that the str is always > num otherwise the result may come out to be absurd
void operator -=(std::string& str, UINT num)
{
	UINT numLen = getNumLen(num);
	UINT strLen = str.length();
	UINT lastZero = -1;

	for (UINT i = 0; i < numLen; i++)
	{
		UINT digitI = getDiitAtPosition(num, i + 1);
		if ( getDigitFormCharacter(str[strLen - i - 1]) >= digitI)
			str[strLen - i - 1] -= digitI;
		else
		{
			str[strLen - i - 1] += (10 - digitI);
			str[strLen - i - 2] -= 1;
		}

		if (str[strLen - i - 1] == CHARACTER_ZERO)
			lastZero = (-1 == lastZero) ? strLen - i - 1 : lastZero;
		else
			lastZero = -1;
	}

	if (str[0] != CHARACTER_ZERO)
		lastZero = -1;

	if (-1 != lastZero)
	{
		if (lastZero != strLen - 1)
			str = str.substr(lastZero + 1, str.length() - 1);
		else
			str = "0";
	}
}

UINT getDigitFormCharacter(char digit)
{
	return (digit - ASCII_CHARACTER_TO_DIGIT_OFFSET);
}


template <class T>
void makeInsertions(trees::RBTree<T>& bst)
{
	std::cout<<std::endl<<"Insertion process started , enter value -999 whenever you want to exit insertion process";

	
	while(true)
	{
		T info = 0;
		std::cout<<std::endl<<"Enter next element: ";
		std::cin>>info;
		if( -999 == info)
			break;

		if(bst.insert( info ).second)
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
void makeDeletions(trees::BST<T>& bst)
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

template <class T>
T nextInput()
{
	T local;
	std::cin>>local;
	return local;
}

template <class T>
void printElementwithRanks(trees::WeightedBST<T>& bst)
{
	std::cout<<std::endl<<"Enter the rank of the element you want to print, enter 0 to exit";

	
	while(true)
	{
		int rank = -1;
		std::cout<<std::endl<<"Enter next rank: ";
		std::cin>>rank;
		if( 0 >= rank )
			break;

		trees::BST<T>::BSTNode* res = bst.getElementWithRank( rank );
		if( NULL != res)
		{
			std::cout<<std::endl<<"The Element with rank "<<rank<<" is: "<<res->getInfo();
		}
		else
		{
			std::cout<<std::endl<<"Rank out of range";
		}
	}

	std::cout<<std::endl<<"Exiting...";
}

//operator >>(std::ifstream fin)
//{
//}

int main(int argc, char* argv[])
{
	/*trees::RBTree<int, std::less<int>, trees::WeightedBST<int> > bst;
	makeInsertions<int>(bst);
	makeDeletions<int>(bst);
	printElementwithRanks<int>(bst);
	bst.performBfsTraversal();
	bst.performSpiralBfsTraversal();*/

	/*std::string str;
	std::ifstream fileIn;
	fileIn.open("input.txt", std::ios::in);
	std::getline(fileIn, str);
	long nQuestions = atol(str.substr( str.find_first_of(" ") + 1).c_str());
	std::vector<long> ranks;

	std::getline(fileIn, str);

	Stacy st(str);

	while(nQuestions-- > 0)
	{
		std::getline(fileIn, str);
		ranks.push_back(atol(str.c_str()));
	}

	for(UINT i = 0; i < ranks.size(); i++)
	{
		std::cout<<std::endl<<atoll(st.getElementWithRank(ranks[i]).c_str()) % (INT)(pow(10,9) + 7);
	}*/

	//std::ofstream ofs("output.txt");
	//
	//std::string str = "0";

	//UINT mod = 1000000007;
	//
	////for (std::string str = "0"; str < mod; str*=1)
	////{
	//	std::string str2  = str;
	//	std::cout << std::endl;
	//	Convertor c(mod);
	//	c(str2);
	//	//std::cout << str2;
	//	ofs << str2<<std::endl;
	//	ofs.flush();
	//}


	/*Stacy stacy("2131334");

	char choice = 'y';
	UINT rank;
	while (choice == 'y')
	{
		std::cout << std::endl << "Enter the rank: ";
		std::cin >> rank;

		std::string res = stacy.getElementWithRank(rank);
		std::cout << std::endl << "The element with rank " << rank << " is: " <<res;
		ofs << std::endl << "The element with rank " << rank << " is: " << res;
		ofs.flush();
	}

	ofs.close();*/

	StacyTester st;

	auto res = st.test();
	getch();

	/*trees::RBTree<int> rbTree;
	makeInsertions<int>(rbTree);*/
	return 0;
}

