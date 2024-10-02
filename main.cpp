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

int main() {
	LinkedList<int> li;
	for (int i; cin >> i; li.push_front(i)) ;
	for (LinkedList<int>::const_iterator it = li.cbegin(); it != li.cend(); ++it) {
		cout << *it << endl;
	}
	return 0;
}
