#ifndef NODE_LIST_H_
#define NODE_LIST_H_

// C/C++ headers
#include <sstream>

// Engine headers
#include <token.hpp>

#include <core/node.hpp>

namespace zhetapi {

class Engine;

class node_list : public Token {
protected:
	std::vector <node>	__nodes;
public:
	node_list(const std::vector <node> &);

	Token *evaluate(Engine *engine) const;

	type caller() const override;
	Token *copy() const override;
	std::string str() const override;
	virtual bool operator==(Token *) const override;
};

}

#endif
