#ifndef ALGORITHM_H_
#define ALGORITHM_H_

// C++ headers
#include <string>
#include <vector>

// Engine headers
#include <core/node_manager.hpp>

namespace zhetapi {

class Barn;
class node_manager;

// Algorithm class
class algorithm : public Token {
	std::string			__ident;

	std::vector <std::string>	__args;

	// Compile when called on
	// std::vector <std::string>			__statements;

	std::string			__alg;
public:
	algorithm();
	algorithm(std::string, const std::vector <std::string> &,
			const std::string &);
	
	Token *execute(const Barn &, const std::vector <Token *> &);
	Token *execute(Barn *, std::string str);

	std::vector <std::string> split(std::string str);
	
	const std::string &symbol() const;

	// Virtual functions
	type caller() const override;
	Token *copy() const override;
	std::string str() const override;

	virtual bool operator==(Token *) const override;
};

}

#endif
