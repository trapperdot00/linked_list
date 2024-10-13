#include <iostream>
#include "Node.h"

using std::cout;
using std::endl;

template class Node<int>;

template <typename T>
void print_node_chain(const Node<T> &n) {
	const Node<T> *p = &n;
	while (p) {
		cout << *p << ' ';
		p = p->getNext();
	}
	cout << endl;
}

int main() {
	int i = 5, j = 12;
	Node<int> n1, n2(i), n3(j, &n2), n4(5), n5(12, &n4);

	print_node_chain(n1);
	print_node_chain(n2);
	print_node_chain(n3);
	print_node_chain(n4);
	print_node_chain(n5);
	
	return 0;
}
