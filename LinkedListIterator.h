#ifndef LINKEDLISTITERATOR_H
#define LINKEDLISTITERATOR_H

#include "Node.h"
#include "LinkedListExcept.h"

template <typename T> class LinkedList;
template <typename T> class LinkedListIterator;
template <typename T> bool operator==
	(const LinkedListIterator<T> &, const LinkedListIterator<T> &);
template <typename T> bool operator!=
	(const LinkedListIterator<T> &, const LinkedListIterator<T> &);

template <typename T>
class LinkedListIterator {
	friend class LinkedList<T>;
	friend bool operator== <T>
		(const LinkedListIterator &, const LinkedListIterator &);
public:
	typedef T value_type;
	typedef value_type & reference;
	typedef value_type * pointer;

	LinkedListIterator() = default;
	LinkedListIterator(NodeBase *);

	reference operator*();
	pointer operator->();

	LinkedListIterator &operator++();
	LinkedListIterator operator++(int);

private:
	NodeBase *curr = nullptr;
};

template <typename T>
LinkedListIterator<T>::LinkedListIterator(NodeBase *p) : curr(p) {}

template <typename T>
typename LinkedListIterator<T>::reference
LinkedListIterator<T>::operator*() {
	if (!curr)
		throw LList::invalid_iterator("nullpointer dereference");
	Node<T> *node = dynamic_cast<Node<T> *>(curr);
	if (!node)
		throw LList::invalid_iterator("invalid node type");
	return node->getValue();
}

template <typename T>
typename LinkedListIterator<T>::pointer
LinkedListIterator<T>::operator->() {
	return &this->operator*();
}

template <typename T>
LinkedListIterator<T> &LinkedListIterator<T>::operator++() {
	if (!curr)
		throw LList::invalid_iterator("nullpointer increment");
	curr = curr->getNext();
	return *this;
}

template <typename T>
LinkedListIterator<T> LinkedListIterator<T>::operator++(int) {
	LinkedListIterator ret = *this;
	++*this;
	return ret;
}

template <typename T>
bool operator==
(const LinkedListIterator<T> &lhs, const LinkedListIterator<T> &rhs) {
	return lhs.curr == rhs.curr;
}

template <typename T>
bool operator!=
(const LinkedListIterator<T> &lhs, const LinkedListIterator<T> &rhs) {
	return !(lhs == rhs);
}

#endif
