#ifndef VARIABLE_CLUSTER_H_
#define VARIABLE_CLUSTER_H_

// Engine headers
#include <token.hpp>

namespace zhetapi {

	struct variable_cluster : public Token {

		::std::string __cluster;

		variable_cluster(const ::std::string & = "");

		type caller() const override;
		Token *copy() const override;
		::std::string str() const override;

		virtual bool operator==(Token *) const override;
	};

}

#endif
