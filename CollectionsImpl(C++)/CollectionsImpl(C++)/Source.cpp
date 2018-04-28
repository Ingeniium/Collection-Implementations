#include "stdafx.h"
#include "RedBlackTree.h"
#include <random>
#include <ctime>
#include <climits>
#include <numeric>
#include <vector>
#include <tuple>

void test_accuracy()
{
	const int NUM_TESTS = 10000;
	const int NUM_ELEMENTS = 100;
	double correct_tests = 0;
	srand(time(NULL));
	RedBlackTree<int> tree;
	for (int i = 0;i < NUM_TESTS;i++)
	{
		double smallest = NUM_TESTS + 1;
		int small_index = -1;
		int large_index = -1;
		double largest = -1;
		for (int j = 0;j < NUM_ELEMENTS;j++)
		{
			double val = rand() % NUM_TESTS;
			if (val < smallest)
			{
				smallest = val;
				small_index = j;
			}
	        if (val > largest)
			{
				largest = val;
				large_index = j;
			}
			tree.Insert(j, val);
		}
		if (tree.MinimumVal() == small_index
			&& tree.MinimumKey() == smallest
			&& tree.MaximumKey() == largest
			&& tree.MaximumVal() == large_index)
		{
			correct_tests++;
		}
		else
		{
			if (tree.MinimumKey() != smallest)
			{
				std::cout << "Got " << tree.MinimumKey() << ", expecting a smaller " << smallest << std::endl;
			}
			if (tree.MaximumKey() != largest)
			{
				std::cout << "Got " << tree.MaximumKey() << ", expecting a larger " << largest << std::endl;
			}
			if (tree.MinimumVal() != small_index)
			{
				std::cout << "Got " << tree.MinimumVal() << ", expecting a value of " << small_index << std::endl;
			}
			if (tree.MaximumVal() != large_index)
			{
				std::cout << "Got " << tree.MaximumVal() << ", expecting a value of " << large_index << std::endl;
			}
		}
		tree.Clear();
	}
	std::cout << "Out of " << NUM_TESTS << " tests, the RedBlackTree passed " << std::endl;
	std::cout << correct_tests << " tests, having a accuracy percentage of " << correct_tests / NUM_TESTS * 100 << "%." << std::endl;
}

/*Test relies on Node constructors and deconstructors to be uncommented*/
void test_deallocation()
{
	RedBlackTree<int,int> tree;
	const int NUM_ELEMENTS = 5;
	std::cout << "Inserting " << NUM_ELEMENTS << " elements..." << std::endl;
	for (int i = 0;i < NUM_ELEMENTS;i++)
	{
		tree.Insert(i, i);
	}
	std::cout << "Clearing container..." << std::endl;
	tree.Clear();
	std::cout << "Inserting " << NUM_ELEMENTS << " more elements..." << std::endl;
	for (int i = 0;i < NUM_ELEMENTS;i++)
	{
		tree.Insert(i, i);
	}
	std::cout << "Cleaning up resources for function end..." << std::endl;
}

void test_iterator()
{
	RedBlackTree<int> tree;
	srand(time(NULL));
	const int NUM_ELEMENTS = 10;
	for (int i = 0; i < NUM_ELEMENTS;i++)
	{
		int num = rand() % NUM_ELEMENTS * 100;
		tree.Insert(num,num);
	}
	for (RedBlackTree<int>::iterator iter = tree.Begin();iter != tree.End();iter++)
	{
		std::cout << *iter << std::endl;
	}
	std::cout << "Total sum of tree elements is " << std::accumulate(tree.Begin(), tree.End(), 0) << std::endl;
	tree.Insert(3223, 1.2);
	tree.Insert(-21, 234.123);
	for (RedBlackTree<int>::iterator iter = tree.Begin();iter != tree.End();iter++)
	{
		std::cout << *iter << std::endl;
	}
	std::cout << "Total sum of tree elements is " << std::accumulate(tree.Begin(), tree.End(), 0) << std::endl;
	
}

void test_heap()
{
	const int SIZE = 1000;
	for (int i = 0;i < RAND_MAX / 10;i++)
	{
		RedBlackTree<long, long> tree;
		for(int i = 0;i < SIZE;i++)
		tree.Insert(rand(), rand());
		//tree.Reserve(SIZE);
	}
	std::cout << "Passed heap test." << std::endl;
}

void test_order()
{
	srand(time(NULL));
	RedBlackTree<char,int> tree;
	const int SIZE = 8;
	for (int i = 0;i < SIZE;i++)
	{
		tree.Insert(rand() % 26 + 65, rand() % 1000);
	}
	std::cout << tree << std::endl;
}

void test_to_array()
{
	srand(time(NULL));
	RedBlackTree<char, int> tree;
	const int SIZE = 8;
	for (int i = 0;i < SIZE;i++)
	{
		tree.Insert(rand() % 26 + 65, rand() % 1000);
	}
	std::tuple<char, int>* arr = tree.ToSortedArray();
	for (int i = 0;i < tree.GetSize();i++)
	{
		std::cout << "(" << std::get<0>(arr[i]) << " ," << std::get<1>(arr[i]) << ")" << std::endl;
	}
	delete[] arr;
}

int main()
{
	test_deallocation();
	test_to_array();
	test_order();
	test_heap();
	test_accuracy();
	test_iterator();
	std::cin.get();
	return 0;
}

