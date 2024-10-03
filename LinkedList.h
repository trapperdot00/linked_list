#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>
#include <utility>
#include <iostream>
#include <stdexcept>

template <typename Derived, typename T> class LinkedListIteratorBase;
template <typename D, typename T>
bool operator==(const LinkedListIteratorBase<D, T> &, const LinkedListIteratorBase<D, T> &);
template <typename D, typename T>
bool operator!=(const LinkedListIteratorBase<D, T> &, const LinkedListIteratorBase<D, T> &);
template <typename T> class LinkedListIterator;
template <typename T> class LinkedListConstIterator;
template <typename T> class LinkedList;
template <typename T> class Node;

// Base class for the Linked List iterators,
// contains common code, to avoid duplication
template <typename Derived, typename T>
class LinkedListIteratorBase {
	friend bool operator==<Derived, T>
		(const LinkedListIteratorBase<Derived, T> &, const LinkedListIteratorBase<Derived, T> &);
public:
	// Default constructor, node pointer is null: uses in-class initializer
	LinkedListIteratorBase() = default;
	// Constructor, makes node denote the given node
	LinkedListIteratorBase(Node<T> *n) : node(n) {}

	// Prefix increment, make node point to the next element,
	// return this object
	Derived &operator++() {
		if (!node)
			throw std::runtime_error("increment past end of list");
		node = node->next;
		return static_cast<Derived &>(*this);
	}
	// Postfix increment, make node point to the next element,
	// return the old value
	// Delegates work to the prefix increment operator, preventing code duplication
	Derived operator++(int) {
		Derived old = static_cast<Derived &>(*this);
		++*this;
		return old;
	}
	// Explicit bool conversion: will be used implicitly in conditions,
	// can be casted explicitly, returns false if node is null, else true
	explicit operator bool() const {
		return node;
	}
protected:
	Node<T> *node = nullptr;
};

// Equality operator, checks if two iterators point to the same Node
template <typename D, typename T>
bool operator==(const LinkedListIteratorBase<D, T> &lhs, const LinkedListIteratorBase<D, T> &rhs) {
	return lhs.node == rhs.node;
}
// Inequality operator, checks whether two iterators point to two different Nodes
// Delegates work to the Equality operator, thus preventing duplicated code
template <typename D, typename T>
bool operator!=(const LinkedListIteratorBase<D, T> &lhs, const LinkedListIteratorBase<D, T> &rhs) {
	return !(lhs == rhs);
}

// Linked List iterator that the non-const versions of begin and end return
// Dereferencing with a * operator returns a non-const reference to the
// value that is held by the denoted Node pointer
template <typename T>
class LinkedListIterator : public LinkedListIteratorBase<LinkedListIterator<T>, T> {
public:
	using LinkedListIteratorBase<LinkedListIterator<T>, T>::LinkedListIteratorBase;

	// Return a plain reference to the denoted element
	T &operator*() const {
		if (!this->node)
			throw std::runtime_error("dereference past end");
		return this->node->val;
	}
	T *operator->() const {
		return &(this->operator*());
	}
};

// Linked List iterator that the const versions of begin, end and the cbegin, cend
// member functions return
// Dereferencing with a * operator returns a const reference to the value that is held
// by the denoted Node pointer
template <typename T>
class LinkedListConstIterator : public LinkedListIteratorBase<LinkedListConstIterator<T>, T> {
public:
	using LinkedListIteratorBase<LinkedListConstIterator<T>, T>::LinkedListIteratorBase;
	
	// Return a reference to const to the denoted element
	const T &operator*() const {
		if (!this->node)
			throw std::runtime_error("dereference past end");
		return this->node->val;
	}
	const T *operator->() const {
		return &(this->operator*());
	}
};

// Class that the user should interact with, acts as an interface
// to the whole class family
// Can get iterators via (c)begin/(c)end members
// Nodes managed by the implementation of the class
template <typename T>
class LinkedList {
public:
	typedef LinkedListIterator<T> iterator;
	typedef LinkedListConstIterator<T> const_iterator;

	// Default constructor to leave head as a nullptr
	LinkedList() = default;
	// Constructor that allocates default initialized elements of size n
	explicit LinkedList(std::size_t n) {
		init(n);
	}
	// Constructor that allocates n elements with a given e value
	LinkedList(std::size_t n, const T &e) {
		init(n, e);
	}
	// Copy constructor, valuelike: copies all elements one-by-one
	LinkedList(const LinkedList &li) {
		head = copy_list(li);
	}
	// Move constructor, takes ownership of the given list's pointer and
	// nullifies it in the given list, to make its free member not delete our new
	// moved values
	LinkedList(LinkedList &&li) noexcept : head(li.head) {
		li.head = nullptr;
	}
	// Copy assignment operator, makes a copy of the elements from the given list,
	// then frees the previous elements, and assigns the head to point to the 
	// first element from the copied Nodes
	// Self assignment ready, as it copies before freeing
	LinkedList &operator=(const LinkedList &rhs) {
		Node<T> *data = copy_list(rhs);
		free();
		head = data;
		return *this;
	}
	// Move assignment operator, frees current elements, then takes ownership
	// of the given list's pointer to the first element of the list,
	// then assigns a nullptr to the given list's pointer to prevent freeing
	// our moved elements
	// Self assignment ready: explicit address checking
	LinkedList &operator=(LinkedList &&rhs) noexcept {
		if (this != &rhs) {
			free();
			head = rhs.head;
			rhs.head = nullptr;
		}
		return *this;
	}
	// Destructor calls free, deletes each element from first to last
	~LinkedList() {
		free();
	}
	
	T &front() {
		if (!head)
			throw std::runtime_error("front on empty LinkedList");
		return head->val;
	}
	const T &front() const {
		if (!head)
			throw std::runtime_error("front on empty LinkedList");
		return head->val;
	}
	// Insertion to the front of the list: copies its argument
	void push_front(const T &e) {
		Node<T> *data = new Node<T>(e);
		data->next = head;
		head = data;
	}
	// Insertion to the front of the list: moves its argument
	void push_front(T &&e) {
		Node<T> *data = new Node<T>(std::move(e));
		data->next = head;
		head = data;
	}

	template <typename... Args>
	void emplace_front(Args&&... args) {
		Node<T> *data = new Node<T>(std::forward<Args>(args)...);
		data->next = head;
		head = data;
	}

	// Erases first element of the list, or does nothing if the list is empty
	void pop_front() {
		if (!head) return;
		Node<T> *curr = head->next;
		delete head;
		head = curr;
	}

	// Delete all elements
	void clear() {
		free();
		head = nullptr;
	}
	
	// Returns a modifiable iterator to the first element
	iterator begin() {
		return iterator(head);
	}
	// Returns a non-modifiable iterator to the first element implicitly,
	// if the list is const
	const_iterator begin() const {
		return cbegin();
	}
	// Returns a non-modifiable iterator to the first element explicitly
	const_iterator cbegin() const {
		return const_iterator(head);
	}
	// Returns a modifiable off-the-end iterator
	iterator end() {
		return iterator(nullptr);
	}
	// Returns a non-modifiable off-the-end iterator implicitly if the list is const
	const_iterator end() const {
		return cend();
	}
	// Returns a non-modifiable off-the-end iterator explicitly
	const_iterator cend() const {
		return const_iterator(nullptr);
	}
private:
	// Does the work of the constructor with the same parameter
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
	// Does the work of the constructor with the same parameters
	void init(std::size_t n, const T &e) {
		if (!n) return;
		head = new Node<T>(e);
		Node<T> *curr = head;
		for (std::size_t i = 1; i < n; ++i) {
			curr->next = new Node<T>(e);
			curr = curr->next;
		}
	}
	
	// Makes a copy of the given list, and returns a pointer to the first element,
	// or a nullptr if the given list was empty
	Node<T> *copy_list(const LinkedList &li) {
		if (li.cbegin() == li.cend())
			return nullptr;
		const_iterator it = li.cbegin();
		Node<T> *data = new Node<T>(*it++), *curr = data;
		while (it != li.cend()) {
			curr->next = new Node<T>(*it++);
			curr = curr->next;
		}
		return data;
	}
	
	// Deletes each element from front to back
	void free() {
		Node<T> *curr = head;
		while (curr) {
			Node<T> *next = curr->next;
			delete curr;
			curr = next;
		}
	}

	// Holds the address of the first element in the list
	Node<T> *head = nullptr;
};

// Node class, data members managed by friend classes
// Users can only access the val member using the iterators
template <typename T>
class Node {
	friend class LinkedList<T>;
	template <typename D, typename T2> friend class LinkedListIteratorBase;
	friend class LinkedListIterator<T>;
	friend class LinkedListConstIterator<T>;
public:
	// Constructs a Node, with the copy of the given value, and a null next pointer
	Node(const T &e) : val(e), next(nullptr) {}
	// Constructs a Node, moving the given value, and making the next pointer null
	Node(T &&e) noexcept : val(std::move(e)), next(nullptr) {}
	// Variadic template constructor, constructs val with the given arguments
	template <typename... Args>
	Node(Args&&... args) : val(args...), next(nullptr) {}

private:
	// Held value
	T val;
	// Pointer to the next element
	Node *next;
};

#endif
