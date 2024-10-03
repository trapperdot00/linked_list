#include "LinkedList.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

template <typename T>
std::ostream &print(std::ostream &os, LinkedList<T> &li, char sep = ' ') {
	for (typename LinkedList<T>::iterator it = li.begin(); it != li.end(); ++it) {
		os << *it << sep;
	}
	return os;
}
template <typename T>
std::ostream &print(std::ostream &os, const LinkedList<T> &cli, char sep = ' ') {
	for (typename LinkedList<T>::const_iterator it = cli.cbegin(); it != cli.cend(); ++it) {
		os << *it << sep;
	}
	return os;
}

template <typename T>
void changeTest(LinkedList<T> &li, const T &val) {
	for (typename LinkedList<T>::iterator it = li.begin(); it != li.end(); ++it) {
		*it = val;
	}
}

int main() {
	LinkedList<string> li1(2, "hi");
	print(cout, li1) << endl;
	const LinkedList<string> cli1(li1);
	print(cout, cli1) << endl;

	changeTest(li1, string("hello"));
	print(cout, li1) << endl;
	return 0;
}
