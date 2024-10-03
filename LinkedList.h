#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>
#include <utility>
#include <iostream>
#include <stdexcept>

template <typename T> class LinkedList;
template <typename T> class LinkedListIterator;
template <typename T> bool operator==(const LinkedListIterator<T> &, const LinkedListIterator<T> &);
template <typename T> bool operator!=(const LinkedListIterator<T> &, const LinkedListIterator<T> &);
template <typename T> class LinkedListConstIterator;
template <typename T> bool operator==(const LinkedListConstIterator<T> &, const LinkedListConstIterator<T> &);
template <typename T> bool operator!=(const LinkedListConstIterator<T> &, const LinkedListConstIterator<T> &);
template <typename T> class Node;

template <typename T>
class LinkedListIterator {
	friend bool operator==<T>(const LinkedListIterator<T> &, const LinkedListIterator<T> &);
public:
	LinkedListIterator() : node(nullptr) {}
	LinkedListIterator(Node<T> *n) : node(n) {}

	T &operator*() const {
		if (!node)
			throw std::runtime_error("dereference past end");
		return node->val;
	}
	LinkedListIterator &operator++() {
		node = node->next;
		return *this;
	}
	LinkedListIterator operator++(int) {
		LinkedListIterator old = *this;
		++*this;
		return old;
	}

	explicit operator bool() const {
		return node;
	}
private:
	Node<T> *node;
};

template <typename T>
bool operator==(const LinkedListIterator<T> &lhs, const LinkedListIterator<T> &rhs) {
	return lhs.node == rhs.node;
}

template <typename T>
bool operator!=(const LinkedListIterator<T> &lhs, const LinkedListIterator<T> &rhs) {
	return !(lhs == rhs);
}

template <typename T>
class LinkedListConstIterator {
	friend bool operator==<T>(const LinkedListConstIterator<T> &, const LinkedListConstIterator<T> &);
public:
	LinkedListConstIterator() : node(nullptr)  {}
	LinkedListConstIterator(const Node<T> *n) : node(n) {}

	const T &operator*() const {
		if (!node)
			throw std::runtime_error("dereference past end");
		return node->val;
	}
	LinkedListConstIterator &operator++() {
		node = node->next;
		return *this;
	}
	LinkedListConstIterator operator++(int) {
		LinkedListConstIterator old = *this;
		++*this;
		return old;
	}

	explicit operator bool() const {
		return node;
	}
private:
	const Node<T> *node;
};

template <typename T>
bool operator==(const LinkedListConstIterator<T> &lhs, const LinkedListConstIterator<T> &rhs) {
	return lhs.node == rhs.node;
}

template <typename T>
bool operator!=(const LinkedListConstIterator<T> &lhs, const LinkedListConstIterator<T> &rhs) {
	return !(lhs == rhs);
}

template <typename T>
class LinkedList {
public:
	typedef LinkedListIterator<T> iterator;
	typedef LinkedListConstIterator<T> const_iterator;

	LinkedList() = default;
	explicit LinkedList(std::size_t n) {
		init(n);
	}
	LinkedList(std::size_t n, const T &e) {
		init(n, e);
	}
	LinkedList(const LinkedList &li) {
		copy_list(li);
	}
	LinkedList(LinkedList &&li) noexcept : head(li.head) {
		li.head = nullptr;
	}
	LinkedList &operator=(const LinkedList &rhs) {
		
		return *this;
	}
	LinkedList &operator=(LinkedList &&rhs) noexcept {
		if (this != &rhs) {
			free();
			head = rhs.head;
			rhs.head = nullptr;
		}
		return *this;
	}

	~LinkedList() {
		free();
	}

	void push_front(const T &e) {
		if (!head) {
			head = new Node<T>(e);
		} else {
			Node<T> *curr = head;
			head = new Node<T>(e);
			head->next = curr;
		}
	}
	void push_front(T &&e) {
		if (!head) {
			head = new Node<T>(std::move(e));
		} else {
			Node<T> *curr = head;
			head = new Node<T>(std::move(e));
			head->next = curr;
		}
	}

	void pop_front() {
		if (!head) return;
		Node<T> *curr = head->next;
		delete head;
		head = curr;
	}
	
	iterator begin() {
		return iterator(head);
	}
	const_iterator begin() const {
		return cbegin();
	}
	const_iterator cbegin() const {
		return const_iterator(head);
	}
	iterator end() {
		return iterator(nullptr);
	}
	const_iterator end() const {
		return cend();
	}
	const_iterator cend() const {
		return const_iterator(nullptr);
	}
private:
	void init(std::size_t n) {
		// If size is 0, don't allocate Nodes and keep the head a nullptr
		if (!n) return;
		head = new Node<T>(T());	
		Node<T> *curr = head;
		for (std::size_t i = 1; i < n; ++i) {
			curr->next = new Node<T>(T());
			curr = curr->next;
		}
	}
	void init(std::size_t n, const T &e) {
		if (!n) return;
		head = new Node<T>(e);
		Node<T> *curr = head;
		for (std::size_t i = 1; i < n; ++i) {
			curr->next = new Node<T>(e);
			curr = curr->next;
		}
	}
	void copy_list(const LinkedList &li) {
		IF (!li.cbegin()) return;
		const_iterator it = li.cbegin();
		head = new Node<T>(*it++);
		Node<T> *curr = head;
		while (it != li.cend()) {
			curr->next = new Node<T>(*it++);
			curr = curr->next;
		}
	}

	void free() {
		Node<T> *curr = head;
		while (curr) {
			Node<T> *next = curr->next;
			delete curr;
			curr = next;
		}
		head = nullptr;
	}

	Node<T> *head = nullptr;
};

template <typename T>
class Node {
	friend class LinkedList<T>;
	friend class LinkedListIterator<T>;
	friend class LinkedListConstIterator<T>;
public:
	Node(const T &e) : val(e), next(nullptr) {}
	Node(T &&e) noexcept : val(std::move(e)), next(nullptr) {}

private:
	T val;
	Node *next;
};

#endif
