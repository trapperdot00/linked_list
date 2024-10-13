#ifndef NODE_H
#define NODE_H

#include <utility>
#include <iostream>

class NodeBase {
public:
	NodeBase() = default;
	NodeBase(NodeBase *);

	NodeBase *getNext();
	const NodeBase *getNext() const;
	
	void setNext(NodeBase *);

	virtual ~NodeBase() = default;
protected:
	NodeBase *next = nullptr;
};

NodeBase::NodeBase(NodeBase *n) : next(n) {}

NodeBase *NodeBase::getNext() { return next; }
const NodeBase *NodeBase::getNext() const { return next; }

void NodeBase::setNext(NodeBase *n) { next = n; }

template <typename T>
class Node : public NodeBase {
public:
	Node() = default;
	Node(const T &, NodeBase * = nullptr);
	Node(T &&, NodeBase * = nullptr) noexcept;

	T &getValue();
	const T &getValue() const;

	void setValue(const T &);
	void setValue(T &&) noexcept;
private:
	T value = T();
};

template <typename T>
Node<T>::Node(const T &e, NodeBase *n) : NodeBase(n), value(e) {}

template <typename T>
Node<T>::Node(T &&e, NodeBase *n) noexcept : NodeBase(n), value(std::move(e)) {}

template <typename T> T &Node<T>::getValue() { return value; }
template <typename T> const T &Node<T>::getValue() const { return value; }

template <typename T> void Node<T>::setValue(const T &e) { value = e; }
template <typename T> void Node<T>::setValue(T &&e) noexcept { value = std::move(e); }

template <typename T>
std::ostream &operator<<(std::ostream &os, const Node<T> &n) {
	return os << n.getValue();
}

#endif
