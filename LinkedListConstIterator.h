#ifndef LINKEDLISTCONSTITERATOR_H
#define LINKEDLISTCONSTITERATOR_H

#include "Node.h"
#include "LinkedListExcept.h"
#include "LinkedListIteratorBase.h"

template <typename T> class LinkedList;
template <typename T> class LinkedListConstIterator;

template <typename T>
class LinkedListConstIterator : public LinkedListIteratorBase {
	friend class LinkedList<T>;
public:
	typedef T value_type;
	typedef const value_type & const_reference;
	typedef const value_type * pointer_to_const;

	LinkedListConstIterator() = default;
	LinkedListConstIterator(NodeBase *);

	const_reference operator*();
	pointer_to_const operator->();

	LinkedListConstIterator &operator++();
	LinkedListConstIterator operator++(int);
};

template <typename T>
LinkedListConstIterator<T>::LinkedListConstIterator(NodeBase *p)
	: LinkedListIteratorBase(p) {}

template <typename T>
typename LinkedListConstIterator<T>::const_reference
LinkedListConstIterator<T>::operator*() {
	if (!curr)
		throw LList::invalid_iterator("nullpointer dereference");
	Node<T> *node = dynamic_cast<Node<T> *>(curr);
	if (!node)
		throw LList::invalid_iterator("invalid node type");
	return node->getValue();
}

template <typename T>
typename LinkedListConstIterator<T>::pointer_to_const
LinkedListConstIterator<T>::operator->() {
	return &this->operator*();
}

template <typename T>
LinkedListConstIterator<T> &LinkedListConstIterator<T>::operator++() {
	if (!curr)
		throw LList::invalid_iterator("nullpointer increment");
	curr = curr->getNext();
	return *this;
}

template <typename T>
LinkedListConstIterator<T> LinkedListConstIterator<T>::operator++(int) {
	LinkedListConstIterator ret = *this;
	++*this;
	return ret;
}

#endif
