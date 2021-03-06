#ifndef NODE_H_
#define NODE_H_

// C/C++ headers
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>

// Engine headers
#include <token.hpp>

#include <core/label.hpp>
#include <core/class.hpp>

namespace zhetapi {

class node {
public:
	// Members
	Token *			__tptr		= nullptr;
	lbl			__label		= l_none;
	cls			__class		= c_none;
	size_t			__nodes		= 0;
	std::vector <node>	__leaves	= {};

	// Constructors
	node();
	node(const node &);
	
	node(Token *);
	node(Token *, lbl);
	node(Token *, const node &, bool);
	node(Token *, const node &, const node &);
	node(Token *, const std::vector <node> &);
	node(Token *, lbl, const std::vector <node> &);

	node &operator=(const node &);

	~node();

	// Operations for cleaner code
	template <class T>
	T *cast() const;

	Token *copy_token() const;

	node &operator[](size_t);
	const node &operator[](size_t) const;

	// Properties
	lbl label() const;
	bool empty() const;
	bool null() const;
	size_t child_count() const;
	Token::type caller() const;
	
	// Member functions
	void transfer(const node &);

	void append(const node &);
	void append_front(const node &);

	// Printing
	void print(int = 1, int = 0) const;
	void print_no_address(int = 1, int = 0) const;

	std::string display(int = 1, int = 0) const;

	// Static methods
	static bool loose_match(const node &, const node &);

	friend std::ostream &operator<<(std::ostream &, const node &);
};

template <class T>
T *node::cast() const
{
	return dynamic_cast <T *> (__tptr);
}

}

#endif
