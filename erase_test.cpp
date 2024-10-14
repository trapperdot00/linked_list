#include <iostream>
#include "LinkedList.h"

template class LinkedList<int>;
template class LinkedListIterator<int>;
template class LinkedListConstIterator<int>;
template class Node<int>;

template <typename T>
std::ostream &operator<<(std::ostream &os, const LinkedList<T> &li) {
	for (const T &e : li)
		os << e << ' ';
	return os;
}

int main() {
	LinkedList<int> li = {0,1,2,3,4,5,6,7,8,9};
	std::cout << li << std::endl;

	LinkedList<int>::iterator prev = li.before_begin(),
		curr = li.begin();
	while (curr != li.cend()) {
		if (*curr % 2) {
			curr = li.erase_after(prev);
		} else {
			prev = curr;
			++curr;
		}
	}

	std::cout << li << std::endl;
	return 0;
}
