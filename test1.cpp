#include "LinkedList.h"
#include <iostream>
#include <cstddef>

template class LinkedListIterator<int>;
template class LinkedListConstIterator<int>;
template class LinkedList<int>;
template class Node<int>;

using std::cout;
using std::cin;
using std::endl;
using std::size_t;
using std::boolalpha;
using std::noboolalpha;

template <typename T>
std::ostream &print(std::ostream &os, const LinkedList<T>& li, char sep = ' ') {
	for (typename LinkedList<T>::const_iterator it = li.cbegin(); it != li.cend(); ++it) {
		os << *it << sep;
	}
	return os;
}

int main() {
	LinkedList<int> li;
	LinkedList<int> li2(5);
	LinkedList<int> li3(5, 6);

	print(cout, li) << endl;
	print(cout, li2) << endl;
	print(cout, li3) << endl;

	for (int i; cin >> i; li.push_front(i)) ;
	print(cout, li) << endl;

	LinkedList<int> li4(li);
	print(cout, li4) << endl;

	li4 = li2;
	print(cout, li4) << endl;
	return 0;
}
