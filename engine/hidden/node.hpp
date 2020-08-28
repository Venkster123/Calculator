#ifndef NODE_H_
#define NODE_H_

/* C++ Standard Libraries */
#include <cassert>
#include <cmath>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

/* Engine Headers */
// #include "config.hpp"
#include <barn.hpp>
#include <class.hpp>
#include <complex.hpp>
#include <config.hpp>
#include <label.hpp>
#include <operation_holder.hpp>
#include <rational.hpp>
#include <stree.hpp>
#include <variable.hpp>
#include <vtable.hpp>
#include <types.hpp>

template <class T>
class vtable;

template <class T, class U>
class Function;

/**
 * @brief Singular node class,
 * used in expression trees.
 */
template <class T, class U>
class node {
public:
	// Friends
	friend class Function <T, int>;
	
	// Aliases
	__TYPEDEFS__

	using params = std::vector <Variable <T>>;
	
	using opn = operation;
	using opd = operand <T>;
	using var = Variable <T>;
	// using ftr = Function <T, int>;
	// using cfg = config <T>;
private:
	// Members
	token *tok;			// Main unit being stored

	nd_label type;			// Auxiliary labeling
					// for compression and differentiation

	nd_class cls = c_none;		// Auxiliary labeling
					// for function specifics

	params pars;			// List of parameters that should be
					// considered in every process

	// shared_ptr <cfg> cfg_ptr;	// Ptr to the operation and numeric
					// configurations

	std::vector <node *> leaves;	// Leaves of the current tree node

	Barn <T, U> *brn;
public:
	// Constructors
	node(std::string, vtable <T> = vtable <T> (), params = params());
			// shared_ptr <cfg> = shared_ptr <cfg> (new cfg()));
	node(stree, vtable <T> = vtable <T> (), params = params()/*, shared_ptr
			<cfg> = shared_ptr <cfg> (new cfg()) */);
	node(token *t, nd_label l, std::vector <node *> lv = {}/* shared_ptr <cfg> */);
	node(token *t, std::vector <node *> lv/* shared_ptr <cfg> */);
	node(const node &);
	node(node *);

	// on
	node();

	// Deconstructor
	~node();
	
	// Assignment Operator
	const node &operator=(const node &);
	
	// Setters
	void set(node *);
	void set(params);
	void set(token *, std::vector <node *>/* shared_ptr <cfg> */);
	void set(token *, nd_label, std::vector <node *> /*, shared_ptr <cfg> */);

	void retokenize(token *);
	void reparametrize(params);

	// Getters
	token *get_token() const;
	nd_class get_class() const;
	nd_label get_label() const;
	params get_params() const;
	// shared_ptr <cfg> get_config() const;
	std::vector <node *> get_leaves() const;

	node *child_at(size_t) const;
	node *operator[](size_t) const;

	// Functional
	bool valid() const;

	bool matches(node *) const;
	bool matches(const node &) const;

	token *value() const;

	void simplify();
	void compress();

	void differentiate(const std::string &);
	void stable_differentiate(const std::string &);

	void integrate(const std::string &);
	void stable_integrate(const std::string &);
	
	void classify();
	void label_all();
	void label(const std::vector <std::string> &);
	
	// Helper
	void print(int = 1, int = 0) const;
	void address_print(int = 1, int = 0) const;

	void traverse(const std::function <void (node)> &);
	
	std::string display() const;

	// Arithmetic 
	template <class A, class B>
	friend const node <A, B> &operator+(const node <A, B> &, const node <A, B> &);

	template <class A, class B>
	friend const node <A, B> &operator-(const node <A, B> &, const node <A, B> &);
	
	template <class A, class B>
	friend const node <A, B> &operator*(const node <A, B> &, const node <A, B> &);
	
	template <class A, class B>
	friend const node <A, B> &operator/(const node <A, B> &, const node <A, B> &);
private:
	// Token Factories
	opn *get(opcode) const;
	opn *get(const std::string &) const;

	// Memory Functions
	node *copy() const;
	
	void clear();

	// Conversion
	void initialize(stree);

	void initialize_as_operation(stree);
	void initialize_as_variable_cluster(stree);

	static token *convert_to_z(const std::string &);
	static token *convert_to_q(const std::string &);
	static token *convert_to_r(const std::string &);

	// Stree Convertion
	node convert(stree, vtable <T> = vtable <T> ());
	node convert_operation(stree, vtable <T> = vtable <T> ());
	node convert_summation(stree, vtable <T> = vtable <T> ());
	node convert_Variable_cluster(stree, vtable <T> = vtable <T> ());

	// Special Nodes
	bool special() const;

	// Labeling Methods
	void label_as_operand();
	void label_as_operation();
	void label_as_special(const std::vector <std::string> &vars);

	// Node Matching
	bool matches_term(const node &) const;

	// Compression Methods
	void compress_as_dot();
	
	// off
	void compress_as_separable();
	void compress_as_multiplied();
	void compress_as_divided();
	void compress_as_power();
	void compress_as_exponential();

	// Differentiation Methods
	void differentiate_as_dot(const std::string &);
	
	// off
	void differentiate_as_multiplied(const std::string &);
	void differentiate_as_divided(const std::string &);
	void differentiate_as_power(const std::string &);
	void differentiate_as_exponential(const std::string &);
	void differentiate_as_trigonometric(const std::string &);
	void differentiate_as_logarithmic(const std::string &);
	void differentiate_as_constant_logarithmic(const std::string &);
	void differentiate_as_function(const std::string &);

	// Integration Methods

	// Display Methods
	std::string display_as_operand(nd_label) const;
	std::string display_as_trigonometric() const;
	std::string display_as_function() const;
public:
	// Exceptions
	class incomputable_tree {};

	// Base class for informative exceptions
	class node_error {
		std::string str;
	public:
		node_error(std::string s)
			: str(s) {}

		const std::string &what() const {
			return str;
		}
	};
	
	// Exception for syntax error
	class syntax_error : public node_error {
	public:
		syntax_error(std::string s)
			: node_error(s) {}
	};
	
	// Exception for unknown symbols
	class undefined_symbol : public node_error {
	public:
		undefined_symbol(std::string s)
			: node_error(s) {}
	};
};

//////////////////////////////////////////
// Constructors
//////////////////////////////////////////

template <class T, class U>
node <T, U> ::node(std::string str, vtable <T> tbl, params __params)
	: tok(nullptr), brn(nullptr), pars(__params), type(l_none), cls(c_none)
{
	// Use the member constructor above
	brn = new Barn <T, U> ();

	// Implicit conversion from string
	// to stree
	initialize(str);

	reparametrize(pars);

	simplify();
	
	cout << "This:" << endl;
	print();
}

template <class T, class U>
node <T, U> ::node(stree raw, vtable <T> tbl, params __params)
	: tok(nullptr), brn(nullptr), pars(__params), type(l_none), cls(c_none)
{
	// Use the member constructor above
	brn = new Barn <T, U> ();

	// Implicit conversion from string
	// to stree
	initialize(raw);

	reparametrize(pars);

	simplify();
}

template <class T, class U>
node <T, U> ::node(token *t, nd_label l, std::vector <node <T, U> *> lv)
	: tok(t), type(l), leaves(lv) {}

template <class T, class U>
node <T, U> ::node(token *t, std::vector <node <T, U> *> lv)
{
	tok = t;
	leaves = lv;
	type = l_none;
}

template <class T, class U>
node <T, U> ::node(const node &other)
{
	*this = other;

	simplify();
}

template <class T, class U>
node <T, U> ::node(node *other)
{
	*this = *other;

	simplify();
}

template <class T, class U>
node <T, U> ::node() : tok(nullptr), type(l_none), cls(c_none),
	pars({}), leaves({}) {}

//////////////////////////////////////////
// Deconstructors
//////////////////////////////////////////

template <class T, class U>
node <T, U> ::~node()
{
	delete tok;
	delete brn;

	for (size_t i = 0; i < leaves.size(); i++)
		delete leaves[i];
}

//////////////////////////////////////////
// Assignment Operator
//////////////////////////////////////////

template <class T, class U>
const node <T, U> &node <T, U> ::operator=(const node <T, U> &other)
{
	if (this != &other) {
		if (tok != nullptr)
			delete tok;

		tok = other.tok->copy();
		type = other.type;
		cls = other.cls;

		pars = other.pars;

		clear();
		for (size_t i = 0; i < other.leaves.size(); i++)
			leaves.push_back(other.leaves[i]->copy());
	}

	return *this;
}

//////////////////////////////////////////
// Setters
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::set(node *nd)
{
	tok = nd->tok;
	type = nd->type;
	leaves = nd->leaves;
}

template <class T, class U>
void node <T, U> ::set(params p)
{
	pars = p;
}

template <class T, class U>
void node <T, U> ::set(token *t, std::vector <node <T, U> *> lv)
{
	tok = t;
	leaves = lv;
	type = l_none;
}

template <class T, class U>
void node <T, U> ::set(token *t, nd_label l, std::vector <node <T, U> *> lv)
{
	tok = t;
	type = l;
	leaves = lv;
}

template <class T, class U>
void node <T, U> ::retokenize(token *t)
{
	delete tok;
	tok = t;
}

template <class T, class U>
void node <T, U> ::reparametrize(params pr)
{
	pars = pr;

	for (node *nd : leaves)
		nd->reparametrize(pr);
}

//////////////////////////////////////////
// Getters
//////////////////////////////////////////

template <class T, class U>
token *node <T, U> ::get_token() const
{
	return tok;
}

template <class T, class U>
nd_class node <T, U> ::get_class() const
{
	return cls;
}

template <class T, class U>
nd_label node <T, U> ::get_label() const
{
	return type;
}

template <class T, class U>
typename node <T, U> ::params node <T, U> ::get_params() const
{
	return pars;
}

/* template <class T, class U>
shared_ptr <typename node <T, U> ::cfg> node <T, U> ::get_config() const
{
	return cfg_ptr;
} */

template <class T, class U>
std::vector <node <T, U> *> node <T, U> ::get_leaves() const
{
	return leaves;
}

template <class T, class U>
node <T, U> *node <T, U> ::child_at(size_t i) const
{
	return leaves[i];
}

template <class T, class U>
node <T, U> *node <T, U> ::operator[](size_t i) const
{
	return leaves[i];
}

//////////////////////////////////////////
// Functional
//////////////////////////////////////////

template <class T, class U>
bool node <T, U> ::valid() const
{
	size_t operands = 0;
	
	switch (tok->caller()) {
	case token::OPERAND:
		return true;
	case token::OPERATION:
		for (auto itr : leaves) {
			switch(itr->tok->caller()) {
			case token::OPERAND:
				operands++;
				break;
			case token::OPERATION:
				if (!itr->valid())
					return false;
				operands++;
				break;
			default:
				return false;
			}
		}

		if (operands != (dynamic_cast <opn *> (tok))->inputs())
			return false;
	}

	return true;
}

template <class T, class U>
bool node <T, U> ::matches(const node <T, U> &other) const
{
	std::vector <node> first;
	std::vector <node> second;

	queue <node> que;

	node current;

	if (type == l_separable)
		que.push(*this);
	else
		first.push_back(*this);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_separable:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			first.push_back(current);
			break;
		}
	}
	
	if (other.type == l_separable)
		que.push(other);
	else
		second.push_back(other);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_separable:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			second.push_back(current);
			break;
		}
	}

	std::vector <node> complete;

	for (auto nd : first) {
		for (auto itr = second.begin(); itr != second.end(); itr++) {
			if (nd.matches_term(*itr)) {
				complete.push_back(nd);
				second.erase(itr);
				break;
			}
		}
	}

	return ((first.size() == complete.size()) && second.empty());
}

template <class T, class U>
token *node <T, U> ::value() const
{
	std::vector <token *> vals;
	std::vector <T> hard;

	std::vector <std::type_index> types;
	
	operation_holder *ophptr;
	
	opn *optr;

	token *tmp;

	opd_r *o_r;
	opd_z *o_z;
	opd_q *o_q;
	opd_cr *o_cr;
	opd_cq *o_cq;
	
	switch (tok->caller()) {
	case token::OPERAND:
		return tok;
		/* o_r = dynamic_cast <opd_r *> (tok);
		if (o_r != nullptr)
			return o_r;

		o_z = dynamic_cast <opd_z *> (tok);
		if (o_z != nullptr)
			return o_z;
		
		o_q = dynamic_cast <opd_q *> (tok);
		if (o_q != nullptr)
			return o_q;

		o_z = dynamic_cast <opd_z *> (tok);
		if (o_z != nullptr)
			return o_z;
		
		o_q = dynamic_cast <opd_q *> (tok);
		if (o_q != nullptr)
			return o_q; */

		throw incomputable_tree();
	case token::OPERATION_HOLDER:
		for (auto nd : leaves) {
			types.push_back(typeid(*(nd->value())));
			vals.push_back(nd->value());
		}
			// cout << "\tTYPE:" << typeid(*(nd->tok)).name() << endl;
		
		ophptr = dynamic_cast <operation_holder *> (tok);
		/* if (brn.mkop(ophptr->rep, types) != nullptr)
			cout << "\tINVALID OPERATION" << endl;
		else
			cout << "\tVALID OPERATION" << endl; */

		tmp = brn->value(ophptr->rep, types, vals);

		if (tmp == nullptr) {
			cout << "err @ node:" << endl;
			print();

			cout << "vals:" << endl;
			for (auto tok : vals)
				cout << "\tt: " << tok->str() << endl;

			throw incomputable_tree();
		}

		return tmp;

		// break;
	/* case token::OPERATION:
		optr = dynamic_cast <opn *> (tok);

		switch (cfg_ptr->code(optr->fmt())) {
		case op_sum:
			for (auto itr : leaves)
				vals.push_back(itr->tok);
			return (*optr)(vals);
		}

		for (auto itr : leaves)
			vals.push_back(itr->value());

		tmp = (*optr)(vals);

		for (size_t i = 0; i < vals.size(); i++)
			delete vals[i];
		
		return tmp; */
	case token::VARIABLE:
		if (!(dynamic_cast <var *> (tok))->is_param()) 
			return dynamic_cast <var *> (tok);
		throw incomputable_tree();
	/* case token::FUNCTOR:
		for (auto itr : leaves)
			hard.push_back(itr->value());
		
		// change later to allow functions to accept
		// different types of tokens for computation
		// return (dynamic_cast <ftr *> (tok))->compute(hard);
		break; */
	}

	throw incomputable_tree();
}

template <class T, class U>
void node <T, U> ::simplify()
{
	label_all();
	compress();
}

template <class T, class U>
void node <T, U> ::compress()
{
	if (type == l_operation_constant
			|| type == l_function_constant) {
		token *tmp = value();

		delete tok;
		clear();

		tok = tmp;

		type = constant_label <T, U> (tok);

		return;
	}
	
	switch (type) {
	case l_dot:
		compress_as_dot();
		break;
	// off
	case l_power:
		compress_as_power();
		break;
	case l_separable:
		compress_as_separable();
		break;
	case l_multiplied:
		compress_as_multiplied();
		break;
	case l_divided:
		compress_as_divided();
		break;
	default:
		for (node *nd : leaves)
			nd->compress();
		break;
	}
}

template <class T, class U>
void node <T, U> ::differentiate(const std::string &var)
{
	switch (type) {
	case l_dot:
		differentiate_as_dot(var);
		break;
	// off
	case l_constant_logarithmic:
		differentiate_as_constant_logarithmic(var);
		break;
	case l_logarithmic:
		differentiate_as_logarithmic(var);
		break;
	case l_trigonometric:
		differentiate_as_trigonometric(var);
		break;
	case l_power:
		differentiate_as_power(var);
		break;
	case l_exponential:
		differentiate_as_exponential(var);
		break;
	case l_multiplied:
		differentiate_as_multiplied(var);
		break;
	case l_divided:
		differentiate_as_divided(var);
		break;
	case l_function:
		differentiate_as_function(var);
		break;
	case l_separable:
		leaves[0]->differentiate(var);
		leaves[1]->differentiate(var);
		break;
	case l_variable:
		delete tok;

		tok = new opd(1);
		break;
	case l_operation_constant:
	case l_constant:
		delete tok;
		clear();

		tok = new opd(0);
		type = l_constant;
		break;
	default:
		break;
	}
}

template <class T, class U>
void node <T, U> ::stable_differentiate(const std::string &var)
{
	label(var);
	differentiate(var);
}

template <class T, class U>
void node <T, U> ::integrate(const std::string &var)
{
}

template <class T, class U>
void node <T, U> ::stable_integrate(const std::string &var)
{
	label(var);
	integrate(var);
}

template <class T, class U>
void node <T, U> ::classify()
{
	for (auto nd : leaves)
		nd->classify();

	switch (type) {
	case l_constant:
	case l_operation_constant:
	case l_variable:
	case l_power:
		cls = c_polynomial;
		break;
	case l_separable:
	case l_multiplied:
		if (leaves[0]->cls == leaves[1]->cls == c_polynomial)
			cls = c_polynomial;
		break;
	case l_divided:
		if (leaves[0]->cls == leaves[1]->cls == c_polynomial)
			cls = c_rational;
		break;
	default:
		break;
	}
}

template <class T, class U>
void node <T, U> ::label_all()
{
	std::vector <std::string> names;
	
	for (auto var : pars)
		names.push_back(var.symbol());

	label(names);
}

template <class T, class U>
void node <T, U> ::label(const std::vector <std::string> &vars)
{
	std::string sym;
	
	bool constant = true;

	if (special())
		return label_as_special(vars);

	for (auto nd : leaves)
		nd->label(vars);

	switch (tok->caller()) {
	case token::OPERATION_HOLDER:
		label_as_operation();
		break;
	case token::FUNCTOR:
		type = l_function;
		
		for (node *nd : leaves) {
			if (nd->type != l_constant &&
					nd->type != l_operation_constant) {
				constant = false;
				break;
			}
		}
		
		if (constant)
			type = l_function_constant;

		break;
	case token::VARIABLE:
		sym = (dynamic_cast <var *> (tok))->symbol();
		
		if (find(vars.begin(), vars.end(), sym) != vars.end())
			type = l_variable;
		else
			type = l_constant;

		break;
	case token::OPERAND:
		label_as_operand();
		break;
	}
}

//////////////////////////////////////////
// Helper
//////////////////////////////////////////
template <class T, class U>
void node <T, U> ::print(int num, int lev) const
{
	int counter = lev;
	while (counter > 0) {
		std::cout << "\t";
		counter--;
	}

	std::string pr = "[";

	for (int i = 0; i < pars.size(); i++) {
		pr += pars[i].symbol();

		if (i < pars.size() - 1)
			pr += ", ";
	}

	pr += "]";

	std::cout << "#" << num << " - [" << strclass[cls] << ", " << strlabs[type] << "] "
		<< tok->str() << " (" << tok << ") @ " << this << ", " << pr << endl;

	counter = 0;
	for (node *itr : leaves) {
		if (itr == nullptr)
			continue;
		itr->print(++counter, lev + 1);
	}
}

template <class T, class U>
void node <T, U> ::address_print(int num, int lev) const
{
	int counter = lev;
	while (counter > 0) {
		std::cout << "\t";
		counter--;
	}

	std::cout << "#" << num << " - [" << strlabs[type] << "] ("
		<< tok << ") @ " << this << std::endl;

	counter = 0;
	for (node *itr : leaves) {
		if (itr == nullptr)
			continue;
		itr->address_print(++counter, lev + 1);
	}
}

template <class T, class U>
void node <T, U> ::traverse(const std::function <void (node)> &fobj)
{
	fobj(*this);

	for (node *nd : leaves)
		nd->traverse(fobj);
}

template <class T, class U>
std::string node <T, U> ::display() const
{
	string str;
	size_t i;

	switch (type) {
	case l_separable:
		//if (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt()) == op_add)
		//	return leaves[0]->display() + " + " + leaves[1]->display();

		return leaves[0]->display() + " +/- " + leaves[1]->display();
	case l_multiplied: 
		return leaves[0]->display_as_operand(type) + " * " + leaves[1]->display_as_operand(type);
	case l_divided:
		return leaves[0]->display_as_operand(type) + " / " + leaves[1]->display_as_operand(type);
	case l_constant ... l_constant_matrix_complex_real:
		return tok->str();
	case l_variable:
		return (dynamic_cast <var *> (tok))->symbol();
	case l_summation_Variable:
		return (dynamic_cast <var *> (tok))->symbol();
	//case l_function:
	//	return display_as_function();
	case l_summation_function:
		// str = (dynamic_cast <ftr *> (tok))->display();

		/* i = str.find(")");
		if (i != string::npos)
			return str.substr(i + 4, str.length()); */
		
		return "?";
	// case l_polynomial: Unnecessary label?
	case l_exp:
	case l_power:
	case l_exponential:
		return leaves[0]->display_as_operand(type) + "^" + leaves[1]->display_as_operand(type);
	// remove operation constant later
	case l_operation_constant:
		return value()->str();
	case l_constant_logarithmic:
	case l_logarithmic:
		return "log_{" + leaves[0]->display() + "} (" + leaves[1]->display() + ")";
	case l_trigonometric:
		return display_as_trigonometric();
	case l_summation:
		return "sum^{" + leaves[2]->display() + "}_{" + leaves[0]->display() + " = " + leaves[1]->display()
			+ "} " + leaves[3]->display();
	case l_factorial:
		return leaves[0]->display() + "!";
	default:
		return "?";
	}
}

//////////////////////////////////////////
// Arithmetic
//////////////////////////////////////////

template <class T, class U>
const node <T, U> &operator+(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_add), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

template <class T, class U>
const node <T, U> &operator-(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_sub), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

template <class T, class U>
const node <T, U> &operator*(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_mul), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

template <class T, class U>
const node <T, U> &operator/(const node <T, U> &a, const node <T, U> &b)
{
	node <T, U> *out = new node <T, U> (a.get(op_div), {
			new node <T, U> (a),
			new node <T, U> (b)
	}, a.cfg_ptr);

	return *out;
}

//////////////////////////////////////////
// Token Factories
//////////////////////////////////////////

template <class T, class U>
typename node <T, U> ::opn *node <T, U> ::get(opcode ocode) const
{
	// return cfg_ptr->alloc_opn(ocode);
	return nullptr;
}

template <class T, class U>
typename node <T, U> ::opn *node <T, U> ::get(const std::string &str) const
{
	// return cfg_ptr->alloc_opn(str);
	return nullptr;
}

//////////////////////////////////////////
// Memory Functions
//////////////////////////////////////////

template <class T, class U>
node <T, U> *node <T, U> ::copy() const
{
	node *cpy;

	cpy = new node(tok->copy(), type, {});
	
	cpy->pars = pars;

	node *tmp;
	for (size_t i = 0; i < leaves.size(); i++) {
		tmp = leaves[i]->copy();

		cpy->leaves.push_back(tmp);
	}
	
	return cpy;
}

template <class T, class U>
void node <T, U> ::clear()
{
	for (size_t i = 0; i < leaves.size(); i++)
		delete leaves[i];

	leaves.clear();
}

//////////////////////////////////////////
// Stree Convertion
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::initialize(stree s)
{
	cout << "initializing..." << endl;

	std::string str = s.str();

	switch (s.kind()) {
	case l_number_real:
		tok = convert_to_r(str);
		type = l_constant_real;
		break;
	case l_number_rational:
		tok = convert_to_q(str);
		type = l_constant_rational;
		break;
	case l_number_integer:
		tok = convert_to_z(str);
		type = l_constant_integer;
		break;
	case l_variable_cluster:
		initialize_as_variable_cluster(s);
		break;
	case l_operation:
		initialize_as_operation(s);
		break;
	}
}

template <class T, class U>
void node <T, U> ::initialize_as_operation(stree s)
{
	tok = new operation_holder(s.str());

	node *cv;
	for (stree *t : s.children()) {
		cv = new node(*t);

		if (cv)
			leaves.push_back(cv);
	}
}

template <class T, class U>
void node <T, U> ::initialize_as_variable_cluster(stree s)
{
	std::string str = s.str();

	// Set up the unit (also make macros for
	// the symbols of operations, until users are allowed
	// to make their own)
	tok = new operation_holder("*");

	leaves = {
		new node {types <T, U> ::one(), l_constant_integer, {}},
		new node {types <T, U> ::one(), l_constant_integer, {}}
	};

	std::string acc;
}

template <class T, class U>
token *node <T, U> ::convert_to_z(const std::string &str)
{
	std::istringstream iss = std::istringstream(str);

	U val;

	iss >> val;

	return new opd_z(val);
}

template <class T, class U>
token *node <T, U> ::convert_to_q(const std::string &str)
{
	std::istringstream iss = std::istringstream(str);

	U num;
	U den;

	char c;

	iss >> num >> c >> den;

	if (c != '/') {
		// throw something instead
		den = 1;
	}

	return new opd_q({num, den});
}

template <class T, class U>
token *node <T, U> ::convert_to_r(const std::string &str)
{
	std::istringstream iss = std::istringstream(str);

	T val;

	iss >> val;

	return new opd_r(val);
}

template <class T, class U>
node <T, U> node <T, U> ::convert(stree st, vtable <T> tbl)
{
	node out;

	T val;

	U num;
	U den;

	nd_label mn;

	char c;
	char i;

	istringstream iss;

	vector <node *> vec;

	// create aliases
	vector <Rational <U>> rats;
	vector <T> reals;
	vector <Complex <Rational <U>>> crats;
	vector <Complex <T>> creals;
	
	vector <vector <Rational <U>>> m_rats;
	vector <vector <T>> m_reals;
	vector <vector <Complex <Rational <U>>>> m_crats;
	vector <vector <Complex <T>>> m_creals;

	// matrices
	
	vector <vector <node *>> mat;

	// flags
	bool fl_real = false;
	
	/* Vector <Rational <U>> rats;
	Vector <T> reals;
	Vector <Complex <Rational <U>>> crats;
	Vector <Complex <T>> creals; */

	switch (st.kind()) {
	case l_operation:
		out = convert_operation(st, tbl);
		break;
	case l_variable_cluster:
		out = convert_Variable_cluster(st, tbl);
		break;
	case l_matrix:
		for (stree *s : st.children()) {
			vec.clear();

			for (stree *t : s->children())
				vec.push_back(convert(t));

			mat.push_back(vec);
		}

		mn = l_none;
		for (auto row : mat) {
			for (node *nd : row) {
				if (nd->type == l_constant_real)
					fl_real = true;

				mn = max(mn, nd->type);
			}
		}

		if ((mn == l_constant_complex_rational) && fl_real)
			mn = l_constant_complex_real;
		
		switch (mn) {
		case l_constant_integer:
		case l_constant_rational:
			for (auto row : mat) {
				rats.clear();

				for (node *nd : row) {
					Rational <U> tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = t1->get();
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = t2->get();

					rats.push_back(tmp);
				}

				m_rats.push_back(rats);
			}
			
			out = node {new opd_m_q(m_rats), l_constant_matrix_rational, {}};
			break;
		case l_constant_real:
			for (auto row : mat) {
				reals.clear();

				for (node *nd : row) {
					T tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = t1->get();
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = t2->get();
					
					auto t3 = dynamic_cast <operand <T> *> (nd->tok);
					if (t3 != nullptr)
						tmp = t3->get();

					reals.push_back(tmp);
				}

				m_reals.push_back(reals);
			}
			
			out = node {new opd_m_r(m_reals), l_constant_matrix_real, {}};
			break;
		case l_constant_complex_rational:
			for (auto row : mat) {
				crats.clear();

				for (node *nd : row) {
					Complex <Rational <U>> tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = Rational <U> (t1->get());
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = t2->get();
					
					auto t3 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
					if (t3 != nullptr)
						tmp = t3->get();

					crats.push_back(tmp);
				}

				m_crats.push_back(crats);
			}
			
			out = node {new opd_m_cq(m_crats), l_constant_matrix_complex_rational, {}};
			break;
		case l_constant_complex_real:
			for (auto row : mat) {
				creals.clear();

				for (node *nd : row) {
					Complex <T> tmp;
					
					auto t1 = dynamic_cast <operand <U> *> (nd->tok);
					if (t1 != nullptr)
						tmp = (T) t1->get();
					
					auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
					if (t2 != nullptr)
						tmp = (T) t2->get();
					
					auto t3 = dynamic_cast <operand <T> *> (nd->tok);
					if (t3 != nullptr)
						tmp = t3->get();
					
					auto t4 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
					if (t4 != nullptr)
						tmp = {(T) t4->get().real(), (T) t4->get().imag()};
					
					auto t5 = dynamic_cast <operand <Complex <T>> *> (nd->tok);
					if (t5 != nullptr)
						tmp = t5->get();

					creals.push_back(tmp);
				}

				m_creals.push_back(creals);
			}
			
			out = node {new opd_m_cr(m_creals), l_constant_matrix_complex_real, {}};
			break;
		default:
			throw syntax_error("err");
		}

		break;
	case l_vector:
		for (stree *s : st.children())
			vec.push_back(convert(s));

		mn = l_none;
		for (node *nd : vec) {
			if (nd->type == l_constant_real)
				fl_real = true;

			mn = max(mn, nd->type);
		}

		if ((mn == l_constant_complex_rational) && fl_real)
			mn = l_constant_complex_real;

		// real & crat -> creal not crat

		// turn into a function(mn, vec), then return function(mn, vec)
		switch (mn) {
		case l_constant_integer:
		case l_constant_rational:
			for (node *nd : vec) {
				Rational <U> tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = t1->get();
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = t2->get();

				rats.push_back(tmp);
			}
			
			out = node {new opd_v_q(rats), l_constant_vector_rational, {}};
			break;
		case l_constant_real:
			for (node *nd : vec) {
				T tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = t1->get();
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = t2->get();
				
				auto t3 = dynamic_cast <operand <T> *> (nd->tok);
				if (t3 != nullptr)
					tmp = t3->get();

				reals.push_back(tmp);
			}
			
			out = node {new opd_v_r(reals), l_constant_vector_real, {}};
			break;
		case l_constant_complex_rational:
			for (node *nd : vec) {
				Complex <Rational <U>> tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = Rational <U> (t1->get());
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = t2->get();
				
				auto t3 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
				if (t3 != nullptr)
					tmp = t3->get();

				crats.push_back(tmp);
			}
			
			out = node {new opd_v_cq(crats), l_constant_vector_complex_rational, {}};
			break;
		case l_constant_complex_real:
			for (node *nd : vec) {
				Complex <T> tmp;
				
				auto t1 = dynamic_cast <operand <U> *> (nd->tok);
				if (t1 != nullptr)
					tmp = (T) t1->get();
				
				auto t2 = dynamic_cast <operand <Rational <U>> *> (nd->tok);
				if (t2 != nullptr)
					tmp = (T) t2->get();
				
				auto t3 = dynamic_cast <operand <T> *> (nd->tok);
				if (t3 != nullptr)
					tmp = t3->get();
				
				auto t4 = dynamic_cast <operand <Complex <Rational <U>>> *> (nd->tok);
				if (t4 != nullptr)
					tmp = {(T) t4->get().real(), (T) t4->get().imag()};
				
				auto t5 = dynamic_cast <operand <Complex <T>> *> (nd->tok);
				if (t5 != nullptr)
					tmp = t5->get();

				creals.push_back(tmp);
			}
			
			out = node {new opd_v_cr(creals), l_constant_vector_complex_real, {}};
			break;
		default:
			throw syntax_error("err");
		}

		break;
	case l_complex_rational:
		iss = istringstream(st.str());

		iss >> num >> c >> den >> i;

		if (c == 'i')
			den = 1;
		else
			assert((i == 'i') && (c == '/'));

		out = node {new opd_cq({0, {num, den}}),
			l_constant_complex_rational, {}};
		break;
	case l_complex_real:
		iss = istringstream(st.str());

		iss >> val >> i;

		assert(i == 'i');

		out = node {new opd_cr ({0, val}),
			l_constant_complex_real, {}};
		break;
	case l_number_rational:
		iss = istringstream(st.str());

		iss >> num >> c >> den;

		if (c != '/')
			den = 1;

		out = node {new opd_q ({num, den}),
			l_constant_rational, {}};
		break;
	case l_number_real:
		iss = istringstream(st.str());

		iss >> val;

		out = node {new opd_r(val), l_constant_real, {}};
		break;
	case l_number_integer:
		iss = istringstream(st.str());

		iss >> num;

		out = node {new opd_z(num), l_constant_integer, {}};
		break;
	default:
		break;
	}

	if (out)
		out->pars = pars;

	return out;
}

template <class T, class U>
node <T, U> node <T, U> ::convert_operation(stree st, vtable <T> tbl)
{
	node *out = new node {new operation_holder(st.str()), {}};

	std::vector <std::type_index> fmt;
	
	node *cv;
	for (stree *s : st.children()) {
		cv = convert(s, tbl);

		if (cv)
			out->leaves.push_back(cv);
	}

	return out;
}

/* template <class T, class U>
node <T, U> *node <T, U> ::convert_summation(stree *st, vtable <T> tbl)
{
	// Requires 4 operands
	// [remove asserts later]
	assert(st->children().size() == 4);

	string str = st->children()[0]->str();
	// string eqn = st->children()[3]->str();

	node *out = new node {get(st->str()), {
		new node {new var {str, true, T()}, {}},
		convert(st->children()[1]),
		convert(st->children()[2]),
		new node {new ftr {
			"f", 
			st->children()[3],
			{var {str, true, T()}},
			tbl},
		{}}
	}};

	return out;
} */

template <class T, class U>
node <T, U> node <T, U> ::convert_Variable_cluster(stree st, vtable <T> vtbl)
{
	// vtbl includes Variables not in the function!!

	node out;

	node *save;
	node *temp;
	node *in;
	
	out = new node {new operation_holder("*"), {
		new node {new operand <U> (1), l_none, {}},
		new node {new operand <U> (1), l_none, {}}
	}};

	temp = out;

	int num = 0;

	std::string str = st.str();
	std::string acc;

	var vr;
	// ftr fr;

	for (int i = 0; i < str.length(); i++) {
		acc += str[i];

		auto vitr = find_if(pars.begin(), pars.end(),
			[&](const var &v) {
				return v.symbol() == acc;
			}
		);

		if (vitr != pars.end()) {
			out = new node {new operation_holder("*"), l_none, {
				out,
				new node {new var {vitr->symbol(), true}, {}}
			}};
			
			for (stree *s : st.children()) {
				out = new node {new operation_holder("*"), {
					out,
					convert(s, vtbl)
				}};
			}

			acc.clear();
			num++;
		}

		try {
			vr = vtbl.find(acc);
			
			out = new node {new operation_holder("*"), {
				out,
				new node {new var(vr), {}}
			}};

			for (stree * s : st.children()) {
				out = new node {new operation_holder("*"), {
					out,
					convert(s, vtbl)
				}};
			}

			acc.clear();
			num++;

			continue;
		} catch(...) {}
		
		/*try {
			fr = tbl.find_ftr(acc);

			out = new node {get(op_mul), {
				out,
				new node {new ftr(fr), {}}
			}};

			for (stree *s : st->children())
				out->leaves[1]->leaves.push_back(convert(s, tbl));

			acc.clear();
			num++;

			continue;
		} catch(...) {} */
	}

	if (!num)
		throw undefined_symbol(acc);

	return out;
}

//////////////////////////////////////////
// Special Nodes
//////////////////////////////////////////

template <class T, class U>
bool node <T, U> ::special() const
{
	/* opn *optr;
	switch (tok->caller()) {
	case token::OPERATION:
		optr = dynamic_cast <opn *> (tok);
		switch (cfg_ptr->code(optr->fmt())) {
		case op_sum:
			return true;
		default:
			break;
		}

		break;
	default:
		break;
	} */

	return false;
}

//////////////////////////////////////////
// Labeling Methods
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::label_as_operand()
{
	if (dynamic_cast <opd_r *> (tok) != nullptr)
		type = l_constant_real;
	else if (dynamic_cast <opd_z *> (tok) != nullptr)
		type = l_constant_integer;
	else if (dynamic_cast <opd_q *> (tok) != nullptr)
		type = l_constant_rational;
	else if (dynamic_cast <opd_cq *> (tok) != nullptr)
		type = l_constant_complex_rational;
	else if (dynamic_cast <opd_cr *> (tok) != nullptr)
		type = l_constant_complex_real;
	else if (dynamic_cast <opd_v_q *> (tok) != nullptr)
		type = l_constant_vector_rational;
	else if (dynamic_cast <opd_v_r *> (tok) != nullptr)
		type = l_constant_vector_real;
	else
		type = l_constant;
}

template <class T, class U>
void node <T, U> ::label_as_operation()
{
	bool constant = true;
	for (node *nd : leaves) {
		if (!is_constant(nd->type)) {
			constant = false;
			break;
		}
	}
	
	if (constant) {
		type = l_operation_constant;
		return;
	}

	codes code = (dynamic_cast <operation_holder *> (tok))->code;

	switch (code) {
	case dot:
		type = l_dot;
		break;
	case mul:
		type = l_multiplied;
		break;
	case pwr:
		type = l_power;
		break;
	}
}

template <class T, class U>
void node <T, U> ::label_as_special(const std::vector <std::string> &vars)
{
	// Early checking
	opn *optr = dynamic_cast <opn *> (tok);

	/* if (optr) {
		switch (cfg_ptr->code(optr->fmt())) {
		case op_sum:
			leaves[0]->type = l_summation_Variable;
			leaves[1]->label(vars);
			leaves[2]->label(vars);
			leaves[3]->type = l_summation_function;
			type = l_summation;
			return;
		default:
			break;
		}
	} */
}

//////////////////////////////////////////
// Node Matching
//////////////////////////////////////////

template <class T, class U>
bool node <T, U> ::matches_term(const node <T, U> &other) const
{
	std::vector <node> first;
	std::vector <node> second;

	queue <node> que;

	node current;

	if (type == l_multiplied)
		que.push(*this);
	else
		first.push_back(*this);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_multiplied:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			first.push_back(current);
			break;
		}
	}
	
	if (other.type == l_multiplied)
		que.push(other);
	else
		second.push_back(other);

	while (!que.empty()) {
		current = que.front();
		que.pop();

		switch (current.type) {
		case l_multiplied:
			que.push(current[0]);
			que.push(current[1]);
			break;
		default:
			second.push_back(current);
			break;
		}
	}

	std::vector <node> complete;

	for (auto nd : first) {
		for (auto itr = second.begin(); itr != second.end(); itr++) {
			if ((*nd.tok) == itr->tok) {
				complete.push_back(nd);
				second.erase(itr);
				break;
			}
		}
	}

	return ((first.size() == complete.size()) && second.empty());
}

//////////////////////////////////////////
// Compression Methods
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::compress_as_separable()
{
	T val;
	T sign;

	/* if (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt()) == op_add)
		sign = cfg_ptr->one;
	else
		sign = cfg_ptr->negative;

	for (node *nd : leaves)
		nd->compress();

	if (leaves[0]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[0]->tok))->get();
		if (val == T(0)) {
			tok = get(op_mul);

			delete leaves[0]->tok;
			leaves[0]->tok = new opd(sign);
			
			label_all();
			compress();
		}
	} else if (leaves[1]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[1]->tok))->get();
		if (val == T(0)) {
			tok = get(op_mul);
			
			delete leaves[1]->tok;
			leaves[1]->tok = new opd(cfg_ptr->one);
			
			label_all();
			compress();
		}
	} */
}

template <class T, class U>
void node <T, U> ::compress_as_dot()
{
	for (node *nd : leaves) 
		nd->compress();
}

template <class T, class U>
void node <T, U> ::compress_as_multiplied()
{
	token *tptr;

	for (node *nd : leaves)
		nd->compress();
	
	if (is_constant(leaves[0]->type)) {
		tptr = leaves[0]->tok;

		if (types <T, U> ::is_one(tptr))
			set(leaves[1]);
		else if (types <T, U> ::is_zero(tptr))
			set(new opd_z(0), {});
	} else if (is_constant(leaves[0]->type)) {
		tptr = leaves[1]->tok;

		if (types <T, U> ::is_one(tptr))
			set(leaves[0]);
		else if (types <T, U> ::is_zero(tptr))
			set(new opd_z(0), {});
	}
}

template <class T, class U>
void node <T, U> ::compress_as_divided()
{
	T val;
	
	for (node *nd : leaves)
		nd->compress();

	if (leaves[0]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[0]->tok))->get();

		if (val == T(1)) {
			delete tok;
			tok = get(op_exp);

			leaves[0] = leaves[1]->copy();

			delete leaves[1];
			leaves[1] = new node(new opd(-1), {});
		} else if (val == T(0)) {
			set(new opd(0), {});
		}
	} else if (leaves[1]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[1]->tok))->get();

		/* if (val == cfg_ptr->one) {
			set(leaves[0]);
		} */
		/* else if (val == 0) <- throw zero division exception
			set(new opd(1), {}); */
	}
}

template <class T, class U>
void node <T, U> ::compress_as_power()
{
	/* cout << "PRE POWER:" << endl;
	print(); */

	T val;

	/* node *lr;
	node *cpy;
	if (leaves[0]->type == l_multiplied) {
		cpy = copy();

		lr = leaves[0]->leaves[1];

		cpy->leaves[0] = lr;
		leaves[0]->leaves[1] = cpy;
		leaves[1] = leaves[0]->leaves[1];
		leaves[0] = leaves[0]->leaves[0];
		tok = get(op_mul);

		leaves[0]->compress();
		leaves[1]->compress();
		
		for (node *nd : leaves)
			nd->compress();
		
		return;
	} */

	for (node *nd : leaves)
		nd->compress();
	
	if (leaves[1]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[1]->tok))->get();
		if (val == T(1))
			set(leaves[0]);
		else if (val == T(0))
			set(new opd(1), {});
	}

	/* cout << "POST POWER:" << endl;
	print(); */
}

template <class T, class U>
void node <T, U> ::compress_as_exponential()
{
	T val;

	for (node *nd : leaves)
		nd->compress();

	if (leaves[0]->type == l_constant) {
		val = (dynamic_cast <opd *> (leaves[0]->tok))->get();
		if (val == T(1))
			set(new opd(1), {});
		else if (val == 0)
			set(new opd(0), {});
	}
}

//////////////////////////////////////////
// Differentiation Methods
//////////////////////////////////////////

template <class T, class U>
void node <T, U> ::differentiate_as_dot(const std::string &var)
{
	node *left = leaves[0];
	node *right = leaves[1];

	/* if (left->type == l_variable)
		set(right->tok, {});
	else
		set(left->tok, {}); */
}

// off
template <class T, class U>
void node <T, U> ::differentiate_as_multiplied(const std::string &var)
{
	node *lcpy;
	node *rcpy;

	tok = get(op_add);

	lcpy = leaves[0]->copy();
	lcpy->differentiate(var);

	rcpy = leaves[1]->copy();
	rcpy->differentiate(var);
	
	leaves = {
		new node(get(op_mul), {lcpy, leaves[1]}),
		new node(get(op_mul), {leaves[0], rcpy}),
	};
}

template <class T, class U>
void node <T, U> ::differentiate_as_divided(const std::string &var)
{
	node *denom = leaves[1]->copy();
	node *lcpy = leaves[0]->copy();
	node *rcpy = leaves[1]->copy();

	lcpy->differentiate({var});
	rcpy->differentiate({var});
	
	tok = get(op_div);

	std::vector <node *> lv_cpy = {
		new node(get(op_sub), {
			new node(get(op_mul), {lcpy, leaves[1]}),
			new node(get(op_mul), {leaves[0], rcpy}),
		}),

		new node(get(op_exp), {
				denom,
				new node(new opd(2), {})
		}),
	};

	leaves = lv_cpy;
}

template <class T, class U>
void node <T, U> ::differentiate_as_power(const std::string &var)
{
	T val = (dynamic_cast <opd *> (leaves[1]->tok))->get();

	node *done = leaves[0]->copy();
	done->differentiate({var});
	
	// delete tok;
	tok = get(op_mul);

	// delete leaves[1];
	leaves[1] = new node(get(op_exp), {
			leaves[0],
			new node(new opd(val - 1), {})
	});
	
	// delete leaves[0];
	leaves[0] = new node(get(op_mul), {
			new node(new opd(val), {}),
			done
	});
}

template <class T, class U>
void node <T, U> ::differentiate_as_exponential(const std::string &var)
{
	node *cpy;

	cpy = leaves[1]->copy();
	cpy->differentiate(var);

	T val = (dynamic_cast <opd *> (leaves[0]->tok))->get();

	set(get(op_mul), {
		cpy,
		new node {get(op_mul), {
			new node {new opd(log(val)), {}},
			new node {get(op_exp), {
				leaves[0],
				leaves[1]
			}}
		}}
	});
}

template <class T, class U>
void node <T, U> ::differentiate_as_trigonometric(const std::string &var)
{
	node *cpy;
	
	cpy = leaves[0]->copy();
	cpy->differentiate(var);

	/* switch (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt())) {
	case op_sin:
		set(get(op_mul), {
			cpy,
			new node {get(op_cos), {
				leaves[0]
			}},
		});

		break;
	case op_cos:
		set(get(op_mul), {
			new node {new opd(-1), {}},
			new node {get(op_mul), {
				cpy,
				new node {get(op_sin), {
					leaves[0]
				}},
			}}
		});
		break;
	case op_tan:
		set(get(op_mul), {
			cpy,
			new node {get(op_exp), {
				new node {get(op_sec), {
					leaves[0],
				}},
				new node {new opd(2), {}}
			}}
		});
		break;
	case op_csc:
		set(get(op_mul), {
			new node {new opd(-1), {}},
			new node {get(op_mul), {
				cpy,
				new node {get(op_mul), {
					new node {get(op_csc), {
						leaves[0]->copy()
					}},
					new node {get(op_cot), {
						leaves[0]
					}}
				}}
			}}
		});
		break;
	case op_sec:
		set(get(op_mul), {
			cpy,
			new node {get(op_mul), {
				new node {get(op_sec), {
					leaves[0]->copy()
				}},
				new node {get(op_tan), {
					leaves[0]
				}}
			}}
		});
		break;
	case op_cot:
		set(get(op_mul), {
			new node {new opd(-1), {}},
			new node {get(op_mul), {
				cpy,
				new node {get(op_exp), {
					new node {get(op_csc), {
						leaves[0],
					}},
					new node {new opd(2), {}}
				}}
			}}
		});
		break;
	default:
		break;
	} */
}

template <class T, class U>
void node <T, U> ::differentiate_as_logarithmic(const std::string &var)
{
}

template <class T, class U>
void node <T, U> ::differentiate_as_constant_logarithmic(const std::string &var)
{
	/* T val = log(leaves[0]->value());

	node *cpy = leaves[1]->copy();
	cpy->differentiate(var);

	set(get(op_div), {
		cpy,
		new node {get(op_mul), {
			new node {new opd(val), {}},
			leaves[1]
		}},
	}); */
}

template <class T, class U>
void node <T, U> ::differentiate_as_function(const std::string &var)
{
	/*
	// Move using declaration to class
	ftr *ft = dynamic_cast <ftr *> (tok);

	// Remove asserts later
	assert(ft->ins() == leaves.size() && leaves.size() > 0);

	std::vector <node *> terms;

	node *out;
	node *cpy;
	node *lcp;
	
	ftr tmp;

	// First Node
	tmp = ft->differentiate((*ft)[0].symbol());
	
	lcp = copy();
	lcp->retokenize(new ftr(tmp));

	cpy = leaves[0]->copy();

	cpy->differentiate(var);
	// cpy->label_all();

	out = new node {get(op_add), {
		new node {get(op_mul), {
			lcp,
			cpy
		}},
		new node {new opd(0), l_none, {}}
	}};

	// Other parameters
	for (int i = 1; i < ft->ins(); i++) {
		tmp = ft->differentiate((*ft)[i].symbol());

		lcp = copy();

		lcp->retokenize(new ftr(tmp));

		cpy = leaves[i]->copy();

		cpy->differentiate(var);
		// cpy->label_all();

		* cout << "\t#" << (i + 1) << endl;
		
		cout << "\t\t(a): " << tmp << endl;
		cout << "\t\t(b): " << cpy->display() << endl; *

		out = new node {get(op_add), {
			new node {get(op_mul), {
				lcp,
				cpy
			}},
			out
		}};
	}

	// cout << endl;

	*this = out->copy(); */
}

//////////////////////////////////////////
// Display Methods
//////////////////////////////////////////

template <class T, class U>
std::string node <T, U> ::display_as_operand(nd_label required) const
{
	std::string out = display();

	switch (required) {
	case l_trigonometric:
		if (type == l_separable
			|| type == l_multiplied)
			out = "(" + out + ")";
		break;
	case l_exp:
	case l_power:
	case l_exponential:
		if (type != l_constant
			&& type != l_variable)
			out = "(" + out + ")";
		break;
	case l_multiplied:
	case l_divided:
		if (type == l_separable)
			out = "(" + out + ")";
		break;
	default:
		break;
	}

	return out;
}

template <class T, class U>
std::string node <T, U> ::display_as_trigonometric() const
{
	std::string stropn;
	std::string space = " ";

	/* stropn = leaves[0]->display_as_operand(l_trigonometric);

	if (stropn[0] == '(')
		space = "";

	switch (cfg_ptr->code((dynamic_cast <opn *> (tok))->fmt())) {
	case op_sin:
		return "sin" + space + stropn;
	case op_cos:
		return "cos" + space + stropn;
	case op_tan:
		return "tan" + space + stropn;
	case op_csc:
		return "csc" + space + stropn;
	case op_sec:
		return "sec" + space + stropn;
	case op_cot:
		return "cot" + space + stropn;
	} */

	throw node_error("Node labeled as trigonometric, \
		but token is of an undetecvtable type");
}

/* template <class T, class U>
std::string node <T, U> ::display_as_function() const
{
	std::string out = (dynamic_cast <ftr *> (tok))->symbol();

	out += "(";

	for (size_t i = 0; i < leaves.size(); i++) {
		out += leaves[i]->display();

		if (i < leaves.size() - 1)
			out += ", ";
	}

	out += ")";

	return out;
} */

#endif