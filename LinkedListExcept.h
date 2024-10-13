#ifndef LINKEDLISTEXCEPT_H
#define LINKEDLISTEXCEPT_H

#include <stdexcept>
#include <string>

namespace LList {

class invalid_constructor : public std::logic_error {
public:
	invalid_constructor(const std::string &s)
		: std::logic_error(s) {}
};

class invalid_iterator : public std::logic_error {
public:
	invalid_iterator(const std::string &s)
		: std::logic_error(s) {}
};

class empty_list : public std::logic_error {
public:
	empty_list (const std::string &s)
		: std::logic_error(s) {}
};

}

#endif
