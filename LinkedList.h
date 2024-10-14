#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <iostream>

#include "LinkedListIterator.h"
#include "LinkedListConstIterator.h"
#include "Node.h"
#include "LinkedListExcept.h"

template <typename T>
class LinkedList {
public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef LinkedListIterator<T> iterator;
	typedef LinkedListConstIterator<T> const_iterator;

	LinkedList() = default;
	explicit LinkedList(size_type);
	LinkedList(size_type, const_reference);
	LinkedList(std::initializer_list<value_type>);
	//template <typename It>
	//LinkedList(It, It);
	LinkedList(const LinkedList &);
	LinkedList(LinkedList &&) noexcept;

	LinkedList &operator=(const LinkedList &);
	LinkedList &operator=(LinkedList &&) noexcept;
	LinkedList &operator=(std::initializer_list<value_type>);

	~LinkedList();

	void assign(size_type, const_reference);
	//template <typename It>
	//void assign(It, It);
	void assign(std::initializer_list<value_type>);

	reference front();
	const_reference front() const;

	iterator before_begin();
	const_iterator before_begin() const;
	const_iterator cbefore_begin() const;
	
	iterator begin();
	const_iterator begin() const;
	const_iterator cbegin() const;

	iterator end();
	const_iterator end() const;
	const_iterator cend() const;

	bool empty() const;

	void clear() noexcept;

	iterator insert_after(const_iterator, const_reference);
	iterator insert_after(const_iterator, value_type &&);
	iterator insert_after(const_iterator, size_type, const_reference);
	template <typename It>
	iterator insert_after(const_iterator, It, It);
	iterator insert_after(const_iterator, std::initializer_list<value_type>);

	template <typename... Args>
	iterator emplace_after(const_iterator, Args&&...);

	iterator erase_after(const_iterator);
	iterator erase_after(const_iterator, const_iterator);

	void push_front(const_reference);
	void push_front(value_type &&);
	template <typename... Args>
	void emplace_front(Args&&...);

	void pop_front();

	void resize(size_type);
	void resize(size_type, const_reference);

	void swap(LinkedList &) noexcept;
	
private:
	NodeBase *copy_list(const LinkedList &);
	template <typename It>
	NodeBase *copy_from_iterrange(It, It);
	void free() noexcept;

	NodeBase *head = nullptr;
};

template <typename T>
LinkedList<T>::LinkedList(size_type sz) : LinkedList(sz, value_type()) {}

template <typename T>
LinkedList<T>::LinkedList(size_type sz, const_reference val) {
	if (!sz)
		throw LList::invalid_constructor("explicit constructor with size of 0");
	head = new NodeBase;
	head->setNext(new Node<T>(val));
	NodeBase *curr = head->getNext();
	std::cout << 0 << ": " << val << std::endl;
	for (size_type i = 1; i < sz; ++i) {
		std::cout << i << ": " << val << std::endl;
		curr->setNext(new Node<T>(val));
		curr = curr->getNext();
	}
}

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> il)
	: head(copy_from_iterrange(il.begin(), il.end())) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList &li) : head(copy_list(li)) {}

template <typename T>
LinkedList<T>::LinkedList(LinkedList &&li) noexcept : head(li.head) {
	li.head = nullptr;
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList &rhs) {
	NodeBase *data = copy_list(rhs);
	free();
	head = data;
	return *this;
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList &&rhs) noexcept {
	if (this != &rhs) {
		free();
		head = rhs.head;
		rhs.head = nullptr;
	}
	return *this;
}

template <typename T>
LinkedList<T> &LinkedList<T>::operator=(std::initializer_list<T> il) {
	free();
	head = copy_from_iterrange(il.begin(), il.end());
	return *this;
}

template <typename T>
LinkedList<T>::~LinkedList() { free(); }

template <typename T>
void LinkedList<T>::assign(size_type sz, const_reference val) {
	free();
	head = new NodeBase();
	NodeBase *curr = head;
	for (size_type i = 0; i != sz; ++i) {
		curr->setNext(new Node<T>(val));
		curr = curr->getNext();
	}
}

template <typename T>
void LinkedList<T>::assign(std::initializer_list<value_type> il) {
	free();
	head = copy_from_iterrange(il.begin(), il.end());
}

template <typename T>
typename LinkedList<T>::reference LinkedList<T>::front() {
	if (!head)
		throw LList::empty_list("front on empty list");
	Node<T> *ret = dynamic_cast<Node<T> *>(head->getNext());
	if (!ret)
		throw LList::empty_list("front on list with only dummy node");
	return ret->getValue();
}

template <typename T>
typename LinkedList<T>::const_reference LinkedList<T>::front() const {
	if (!head)
		throw LList::empty_list("front on empty list");
	Node<T> *ret = dynamic_cast<Node<T> *>(head->getNext());
	if (!ret)
		throw LList::empty_list("front on list with only dummy node");
	return ret->getValue();
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::before_begin() {
	return iterator(head);
}
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::before_begin() const {
	return cbefore_begin();
}
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cbefore_begin() const {
	return const_iterator(head);
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::begin() {
	if (!head)
		return iterator(head);
	return iterator(head->getNext());
}
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::begin() const {
	return cbegin();
}
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cbegin() const {
	if (!head)
		return const_iterator(head);
	return const_iterator(head->getNext());
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::end() {
	NodeBase *p = head;
	while (p) {
		p = p->getNext();
	}
	return iterator(p);
}
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::end() const {
	return cend();
}
template <typename T>
typename LinkedList<T>::const_iterator LinkedList<T>::cend() const {
	NodeBase *p = head;
	while (p) {
		p = p->getNext();
	}
	return const_iterator(p);
}

template <typename T>
bool LinkedList<T>::empty() const { return !head; }

template <typename T>
void LinkedList<T>::clear() noexcept {
	free();
	head = nullptr;
}

template <typename T>
typename LinkedList<T>::iterator
LinkedList<T>::insert_after(const_iterator pos, const_reference val) {
	if (!pos.curr) {
		head = new NodeBase(new Node<T>(val));
		return iterator(head->getNext());
	} else {
		NodeBase *next = pos.curr->getNext();
		pos.curr->setNext(new Node<T>(val, next));
		return iterator(pos.curr->getNext());
	}
}

template <typename T>
typename LinkedList<T>::iterator
LinkedList<T>::insert_after(const_iterator pos, value_type &&val) {
	if (!pos.curr) {
		head = new NodeBase(new Node<T>(std::move(val)));
		return iterator(head->getNext());
	} else {
		NodeBase *next = pos.curr->getNext();
		pos.curr->setNext(new Node<T>(std::move(val), next));
		return iterator(pos.curr->getNext());
	}
}
/*
template <typename T>
typename LinkedList<T>::iterator
LinkedList<T>::insert_after(const_iterator pos, size_type sz, const_reference val)

template <typename T>
typename LinkedList<T>::iterator
LinkedList<T>::insert_after(const_iterator pos, std::initializer_list<value_type> il)
*/

template <typename T>
typename LinkedList<T>::iterator
LinkedList<T>::erase_after(const_iterator pos) {
	if (!pos.curr)
		return iterator(pos.curr);
	NodeBase *next = pos.curr->getNext(),
			 *newnext = next->getNext();
	delete next;
	pos.curr->setNext(newnext);
	return iterator(newnext);
}

template <typename T>
NodeBase *LinkedList<T>::copy_list(const LinkedList &other) {
	if (other.empty())
		return nullptr;
	const_iterator it = other.cbegin();
	NodeBase *data = new NodeBase(new Node<T>(*it++));
	NodeBase *curr = data->getNext();
	while (it != other.cend()) {
		curr->setNext(new Node<T>(*it++));
		curr = curr->getNext();
	}
	return data;
}

template <typename T>
template <typename It>
NodeBase *LinkedList<T>::copy_from_iterrange(It first, It last) {
	if (first == last)
		return nullptr;
	NodeBase *data = new NodeBase(new Node<T>(*first++));
	NodeBase *curr = data->getNext();
	while (first != last) {
		curr->setNext(new Node<T>(*first++));
		curr = curr->getNext();
	}
	return data;
}

template <typename T>
void LinkedList<T>::free() noexcept {
	NodeBase *curr = head;
	while (curr) {
		NodeBase *next = curr->getNext();
		std::cout << "deleted" << std::endl;
		delete curr;
		curr = next;
	}
}

#endif
