#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <cstddef>
#include <type_traits>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <initializer_list>

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
	LinkedListIteratorBase(Node<T> *n, bool l = false) : node(n), before_begin(l) {}

	// Prefix increment, make node point to the next element,
	// return this object
	Derived &operator++() {
		if (!node)
			throw std::runtime_error("increment past end of list");
		node = node->next;
		if (before_begin)
			before_begin = false;
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
	bool before_begin = false;
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
		if (this->before_begin)
			throw std::runtime_error("dereferencing illegal iterator");
		return this->node->val;
	}
	// Returns the address of the denoted element
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
		if (this->before_begin)
			throw std::runtime_error("dereferencing illegal iterator");
		return this->node->val;
	}
	// Returns the address of the denoted element
	const T *operator->() const {
		return &(this->operator*());
	}
};

// SFINAE
// Default type trait defaults to std::false_type
template <typename T, typename = void>
struct is_input_iterator : std::false_type {};

// If T supports postfix incremention, dereferencing, and 
// equality comparison, then matches this type trait:
// becomes std::true_type
template <typename T>
struct is_input_iterator<T, std::void_t<
		decltype(std::declval<T&>()++),
		decltype(*std::declval<T&>()),
		decltype(std::declval<T&>() == std::declval<T&>())>
> : std::true_type {};

// Class that the user should interact with, acts as an interface
// to the whole class family
// Can get iterators via (c)begin/(c)end members
// Nodes managed by the implementation of the class
template <typename T>
class LinkedList {
public:
	typedef T value_type;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef LinkedListIterator<T> iterator;
	typedef LinkedListConstIterator<T> const_iterator;

	// Default constructor to leave head as a nullptr
	LinkedList() = default;
	// Constructor that allocates default initialized elements of size n
	explicit LinkedList(size_type n) {
		init(n);
	}
	// Constructor that allocates n elements with a given e value
	LinkedList(size_type n, const T &e) {
		init(n, e);
	}
	// Constructor that takes an initializer list,
	// and constructs each element from the values in the initializer list
	template <typename T2>
	LinkedList(std::initializer_list<T2> il) {
		head = copy_from_iterrange(il.begin(), il.end());
		dummy->next = head;
	}
	// Constructor that takes an iterator range and constructs each element from those in the range
	// Element type in the given range can differ from the type in the list, if a conversion exists
	// 'It' must allow dereferencing, postfix incrementing and have an overloaded equality operator
	template <typename It, typename = std::enable_if_t<is_input_iterator<It>::value>>
	LinkedList(It b, It e) {
		head = copy_from_iterrange(b, e);
		dummy->next = head;
	}
	// Copy constructor, valuelike: copies all elements one-by-one
	LinkedList(const LinkedList &li) {
		head = copy_list(li);
		dummy->next = head;
	}
	// Move constructor, takes ownership of the given list's pointer and
	// nullifies it in the given list, to make its free member not delete our new
	// moved values
	LinkedList(LinkedList &&li) noexcept : head(li.head), dummy(li.dummy) {
		li.head = nullptr;
		li.dummy = nullptr;
	}
	// Copy assignment operator, makes a copy of the elements from the given list,
	// then frees the previous elements, and assigns the head to point to the 
	// first element from the copied Nodes
	// Self assignment ready, as it copies before freeing
	LinkedList &operator=(const LinkedList &rhs) {
		Node<T> *data = copy_list(rhs);
		free();
		head = data;
		dummy->next = head;
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
			dummy = rhs.dummy;
			rhs.head = nullptr;
			rhs.dummy = nullptr;
		}
		return *this;
	}
	// Assignment operator that takes an initializer list, frees elements,
	// then copies all elements from il, and head will either point to the
	// first copied element, or be a nullptr if the initializer list was empty
	template <typename T2>
	LinkedList &operator=(std::initializer_list<T2> il) {
		free();
		head = copy_from_iterrange(il.begin(), il.end());
		dummy->next = head;
		return *this;
	}

	// Destructor calls free, deletes each element from first to last
	~LinkedList() {
		free();
		delete dummy;
	}
	// Returns a reference to the first element in the list, or throws a runtime_error if the list is empty
	T &front() {
		if (!head)
			throw std::runtime_error("front on empty LinkedList");
		return head->val;
	}
	// Returns a const reference to the first element in the list, or throws a runtime_error if the list is empty
	// Overloaded on const: this will only be calles on const objects
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
		dummy->next = head;
	}
	// Insertion to the front of the list: moves its argument
	void push_front(T &&e) {
		Node<T> *data = new Node<T>(std::move(e));
		data->next = head;
		head = data;
		dummy->next = head;
	}
	
	// Constructs a new element to the front of the list, forwarding the given arguments to the object's constructor
	template <typename... Args>
	void emplace_front(Args&&... args) {
		Node<T> *data = new Node<T>(std::forward<Args>(args)...);
		data->next = head;
		head = data;
		dummy->next = head;
	}

	// Erases first element of the list, or does nothing if the list is empty
	void pop_front() {
		if (!head) return;
		Node<T> *curr = head->next;
		delete head;
		head = curr;
		dummy->next = head;
	}

	// Delete all elements
	void clear() {
		free();
		head = nullptr;
		dummy->next = nullptr;
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

	// Returns a modifiable off-the-beginning iterator, denoting the dummy Node,
	// the iterator is not dereferencable until incremented
	iterator before_begin() {
		return iterator(dummy, true);
	}
	// Returns a non-modifiable off-the-beginning iterator denoting the dummy Node
	// implicitly if the list is const,
	// the iterator is not dereferencable until incremented
	const_iterator before_begin() const {
		return cbefore_begin();
	}
	// Returns a non-modifiable off-the-beginning iterator denoting the dummy Node
	// explicitly, the iterator is not dereferencable until incremented
	const_iterator cbefore_begin() const {
		return const_iterator(dummy, true);
	}
private:
	// Does the work of the constructor with the same parameter
	void init(size_type n) {
		// If size is 0, don't allocate Nodes and keep the head a nullptr
		if (!n) return;
		head = new Node<T>(T());
		dummy->next = head;
		Node<T> *curr = head;
		for (size_type i = 1; i < n; ++i) {
			curr->next = new Node<T>(T());
			curr = curr->next;
		}
	}
	// Does the work of the constructor with the same parameters
	void init(size_type n, const T &e) {
		if (!n) return;
		head = new Node<T>(e);
		dummy->next = head;
		Node<T> *curr = head;
		for (size_type i = 1; i < n; ++i) {
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
	// Copies all element from given iterator range, returns a pointer to the first element, or a nullptr if the range was empty
	template <typename It>
	Node<T> *copy_from_iterrange(It b, It e) {
		if (b == e)
			return nullptr;
		Node<T> *data = new Node<T>(*b++), *curr = data;
		while (b != e) {
			curr->next = new Node<T>(*b++);
			curr = curr->next;
		}
	return data;
	}
	
	// Deletes each element from front to back
	void free() {
		Node<T> *curr = head;
		while (curr) {
			Node<T> *next = curr->next;

			// DEBUG DELETION MESSAGE
			// ERROR ON TYPES WITH NO OUTPUT OPERATOR OVERLOAD
			std::cout << curr->val << " deleted" << std::endl;
			
			delete curr;
			curr = next;
		}
	}

	// Holds the address of the first element in the list
	Node<T> *head = nullptr;

	// Dummy Node whose next pointer should point to head
	Node<T> *dummy = new Node<T>(T());
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
