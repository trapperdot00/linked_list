#include "LinkedList.h"
#include <iostream>

using std::cout;
using std::endl;

template <typename T> std::ostream &print(std::ostream &, const LinkedList<LinkedList<T>> &, char = ' ');
template <typename T> std::ostream &print(std::ostream &, const LinkedList<T> &, char = ' ');

template <typename T>
std::ostream &print(std::ostream &os, const LinkedList<LinkedList<T>> &lli, char sep) {
	for (typename LinkedList<LinkedList<T>>::const_iterator it1 = lli.cbegin(); it1 != lli.cend(); ++it1) {
		print(os, *it1, sep);
		os << '\n';
	}
	return os;
}

template <typename T>
std::ostream &print(std::ostream &os, const LinkedList<T> &li, char sep) {
	for (typename LinkedList<T>::const_iterator it = li.cbegin(); it != li.cend(); ++it)
		os << *it << sep;
	return os;
}

int main() {
	LinkedList<LinkedList<int>> li;
	print(cout, li);

	li.emplace_front(3, 5);
	li.emplace_front(3, 6);
	li.emplace_front(3, 7);
	li.emplace_front(4, 5);
	print(cout, li);

	li.clear();
	print(cout, li);

	li.emplace_front(10, 1);
	li.emplace_front(10, 2);
	print(cout, li.front()) << endl;
	print(cout, *++li.begin()) << endl;
	return 0;
}
