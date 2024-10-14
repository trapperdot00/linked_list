#ifndef LINKEDLISTITERATORBASE_H
#define LINKEDLISTITERATORBASE_H

#include "Node.h"

class LinkedListIteratorBase {
	friend bool operator==(const LinkedListIteratorBase &, const LinkedListIteratorBase &);
protected:
	LinkedListIteratorBase() = default;
	LinkedListIteratorBase(NodeBase *);

	NodeBase *curr = nullptr;
};

LinkedListIteratorBase::LinkedListIteratorBase(NodeBase *p) : curr(p) {}

bool operator==(const LinkedListIteratorBase &lhs, const LinkedListIteratorBase &rhs) {
	return lhs.curr == rhs.curr;
}

bool operator!=(const LinkedListIteratorBase &lhs, const LinkedListIteratorBase &rhs) {
	return !(lhs == rhs);
}

#endif
