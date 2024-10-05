#include "LinkedList.h"
#include <iostream>
#include <initializer_list>
#include <vector>

template <typename T>
std::ostream &print(std::ostream &os, const LinkedList<T> &li, char sep = ' ') {
	for (typename LinkedList<T>::const_iterator it = li.cbegin();
		 it != li.cend(); ++it)
		os << *it << sep;
	return os;
}

int main() {
	{
	std::vector<double> dvec{1.1,2.1,3.14,4.5,6.7,10.99};
	LinkedList<int> ili1(dvec.cbegin(), dvec.cend());
	print(std::cout, ili1, '-') << std::endl;
	}
	{
	LinkedList<int> ili2 = {1.1,2.2,3.3};
	print(std::cout, ili2, '/') << std::endl;
	}
	{
	LinkedList<int> ili3 = {0,1,2,3,4,5};
	print(std::cout, ili3, '>') << std::endl;
	ili3 = {5,6,7,8};
	print(std::cout, ili3) << std::endl;
	ili3 = {};
	print(std::cout, ili3) << std::endl;
	}

	{
	LinkedList<int> ili(3, 4);
	LinkedList<int>::const_iterator it = ili.cbefore_begin();
	++it;
	while (it != ili.cend()) {
		std::cout << *it++ << '+';
	}
	std::cout << '\n';
	}
	return 0;
}
