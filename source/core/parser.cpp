#include <core/parser.hpp>

namespace zhetapi {

parser::parser() : parser::base_type(__start)
{
	__esc.add("\\a", '\a')("\\b", '\b')("\\f", '\f')("\\n", '\n')
		("\\r", '\r')("\\t", '\t')("\\v", '\v')("\\\\", '\\')
		("\\\'", '\'')("\\\"", '\"');

	/*
	 * Parser for an identifier. Used to construct variable
	 * clusters.
	 */
	__ident = +qi::char_("a-zA-Z$_");

	__str = +(__esc | (qi::char_ - '\"'));

	// Operation parsers

	__add_operation_symbol(__plus, +);
	__add_operation_symbol(__minus, -);
	__add_operation_symbol(__times, *);
	__add_operation_symbol(__divide, /);
	__add_operation_symbol(__power, ^);
	__add_operation_symbol(__dot, .);
	__add_operation_symbol(__mod, %);
	
	// Binary comparison
	__add_operation_symbol(__eq, ==);
	__add_operation_symbol(__neq, !=);
	__add_operation_symbol(__ge, >);
	__add_operation_symbol(__le, <);
	__add_operation_symbol(__geq, >=);
	__add_operation_symbol(__leq, <=);

	// Unary increment/decrement
	__add_operation_heter_symbol(__post_incr, ++, p++);
	__add_operation_heter_symbol(__post_decr, --, p--);
	
	__add_operation_heter_symbol(__pre_incr, ++, r++);
	__add_operation_heter_symbol(__pre_decr, --, r--);

	/*
	 * Represents a binary operation of lowest priotrity.
	 * These are used to combine terms into expressions.
	 * Exmaples of such operations are addition,
	 * subtraction and the dot product.
	 */
	__t0_bin = __plus | __minus | __dot | __mod
			| __eq | __neq | __geq
			| __leq | __ge | __le;

	/*
	 * Represents a binary operation of second to lowest
	 * priority. Connects factors into term. Examples are
	 * multiplication and division.
	 */
	__t1_bin = __times | __divide;
	
	/*
	 * Represents other binary operations with precedence
	 * higher than that of __t1_bin. Examples are the
	 * exponentiation operation.
	 */
	__t2_bin = __power;

	__t_post = __post_incr | __post_decr;
	__t_pre = __pre_incr | __pre_decr;

	// Type parsers (change dependence on int_ and double_ parsers
	// specifically)

	// Reals
	__z = int_;

	__q = (int_ >> '/' >> int_) [
		_val = phoenix::construct <Q> (_1, _2)
	];

	// __r = double_;
	__r = qi::real_parser <R, qi::strict_real_policies <R>> ();

	// Generalized
	__gq = __q | __z;
	__gr = __r | __gq;
	
	// Complex
	__cz = (__z >> 'i') [
		_val = phoenix::construct <CZ> (0, _1)
	];
	
	__cq = (__q >> 'i') [
		_val = phoenix::construct <CQ> (0, _1)
	];
	
	__cr = (__r >> 'i') [
		_val = phoenix::construct <CR> (0, _1)
	];
	
	__cgq = (__gq >> 'i') [
		_val = phoenix::construct <CQ> (0, _1)
	];
	
	__cgr = (__gr >> 'i') [
		_val = phoenix::construct <CR> (0, _1)
	];

	// Vector

	__vz_inter = __z % ',';
	__vq_inter = __q % ',';
	__vr_inter = __r % ',';
	__vgq_inter = __gq % ',';
	__vgr_inter = __gr % ',';
	__vcz_inter = __cz % ',';
	__vcq_inter = __cq % ',';
	__vcr_inter = __cr % ',';
	__vcgq_inter = __cgq % ',';
	__vcgr_inter = __cgr % ',';
	
	__vz = ('[' >> __vz_inter >> ']') [
		_val = _1
	];
	
	__vq = ('[' >> __vq_inter >> ']') [
		_val = _1
	];
	
	__vr = ('[' >> __vr_inter >> ']') [
		_val = _1
	];
	
	__vgq = ('[' >> __vgq_inter >> ']') [
		_val = _1
	];
	
	__vgr = ('[' >> __vgr_inter >> ']') [
		_val = _1
	];
	
	__vcz = ('[' >> __vcz_inter >> ']') [
		_val = _1
	];
	
	__vcq = ('[' >> __vcq_inter >> ']') [
		_val = _1
	];
	
	__vcr = ('[' >> __vcr_inter >> ']') [
		_val = _1
	];
	
	__vcgq = ('[' >> __vcgq_inter >> ']') [
		_val = _1
	];
	
	__vcgr = ('[' >> __vcgr_inter >> ']') [
		_val = _1
	];

	// Matrix
	__mz_inter = __vz % ',';
	__mq_inter = __vq % ',';
	__mr_inter = __vr % ',';
	__mgq_inter = __vgq % ',';
	__mgr_inter = __vgr % ',';
	__mcz_inter = __vcz % ',';
	__mcq_inter = __vcq % ',';
	__mcr_inter = __vcr % ',';
	__mcgq_inter = __vcgq % ',';
	__mcgr_inter = __vcgr % ',';
	
	__mz = ('[' >> __mz_inter >> ']') [
		_val = _1
	];
	
	__mq = ('[' >> __mq_inter >> ']') [
		_val = _1
	];
	
	__mr = ('[' >> __mr_inter >> ']') [
		_val = _1
	];
	
	__mgq = ('[' >> __mgq_inter >> ']') [
		_val = _1
	];
	
	__mgr = ('[' >> __mgr_inter >> ']') [
		_val = _1
	];
	
	__mcz = ('[' >> __mcz_inter >> ']') [
		_val = _1
	];
	
	__mcq = ('[' >> __mcq_inter >> ']') [
		_val = _1
	];
	
	__mcr = ('[' >> __mcr_inter >> ']') [
		_val = _1
	];
	
	__mcgq = ('[' >> __mcgq_inter >> ']') [
		_val = _1
	];
	
	__mcgr = ('[' >> __mcgr_inter >> ']') [
		_val = _1
	];
	
	// Token parsers

	// Reals
	__o_str = qi::lit('\"') >> __str [
		_val = phoenix::new_ <opd_s> (_1)
	] >> qi::lit('\"');

	__o_z = __z [
		_val = phoenix::new_ <opd_z> (_1)
	];
	
	__o_q = __q [
		_val = phoenix::new_ <opd_q> (_1)
	];

	__o_r = __r [
		_val = phoenix::new_ <opd_r> (_1)
	];
	
	// Complex
	__o_cz = __cz [
		_val = phoenix::new_ <opd_cz> (_1)
	];
	
	__o_cq = __cq [
		_val = phoenix::new_ <opd_cq> (_1)
	];

	__o_cr = __cr [
		_val = phoenix::new_ <opd_cr> (_1)
	];

	// Vector
	__o_vz = __vz [
		_val = phoenix::new_ <opd_v_z> (_1)
	];
	
	__o_vq = __vq [
		_val = phoenix::new_ <opd_v_q> (_1)
	];
	
	__o_vr = __vr [
		_val = phoenix::new_ <opd_v_r> (_1)
	];
	
	__o_vgq = __vgq [
		_val = phoenix::new_ <opd_v_q> (_1)
	];
	
	__o_vgr = __vgr [
		_val = phoenix::new_ <opd_v_r> (_1)
	];
	
	__o_vcz = __vcz [
		_val = phoenix::new_ <opd_v_cz> (_1)
	];
	
	__o_vcq = __vcq [
		_val = phoenix::new_ <opd_v_cq> (_1)
	];
	
	__o_vcr = __vcr [
		_val = phoenix::new_ <opd_v_cr> (_1)
	];
	
	__o_vcgq = __vcgq [
		_val = phoenix::new_ <opd_v_cq> (_1)
	];
	
	__o_vcgr = __vcgr [
		_val = phoenix::new_ <opd_v_cr> (_1)
	];

	// Matrix
	__o_mz = __mz [
		_val = phoenix::new_ <opd_m_z> (_1)
	];
	
	__o_mq = __mq [
		_val = phoenix::new_ <opd_m_q> (_1)
	];
	
	__o_mr = __mr [
		_val = phoenix::new_ <opd_m_r> (_1)
	];
	
	__o_mgq = __mgq [
		_val = phoenix::new_ <opd_m_q> (_1)
	];
	
	__o_mgr = __mgr [
		_val = phoenix::new_ <opd_m_r> (_1)
	];
	
	__o_mcz = __mcz [
		_val = phoenix::new_ <opd_m_cz> (_1)
	];
	
	__o_mcq = __mcq [
		_val = phoenix::new_ <opd_m_cq> (_1)
	];
	
	__o_mcr = __mcr [
		_val = phoenix::new_ <opd_m_cr> (_1)
	];
	
	__o_mcgq = __mcgq [
		_val = phoenix::new_ <opd_m_cq> (_1)
	];
	
	__o_mcgr = __mcgr [
		_val = phoenix::new_ <opd_m_cr> (_1)
	];

	// Nodes

	__node_pack = __start % ',';

	__collection = ('{' >> __node_pack >> '}') [
		_val = phoenix::new_ <node_list> (_1)
	];

	/*
	 * Pure numerical Operands, representing the 18
	 * primitive types of computation. The exceptions are
	 * rational numbers, which are excluded so that the
	 * order of operations can be applied. This exclusion
	 * should not cause any trouble, as integer division
	 * will yield a rational result.
	 */
	__node_opd = (
			__o_str
			| __o_cr | __o_cz
			| __o_r | __o_z
			| __o_vcr | __o_vcz
			| __o_vr | __o_vz
			| __o_vcgr
			| __o_vgr
			| __o_mcr | __o_mcz
			| __o_mr | __o_mz
			| __o_mcgr
			| __o_mgr
		) [
		_val = phoenix::construct <zhetapi::node> (_1,
				::std::vector <zhetapi::node> {})
	];

	/*
	 * A variable cluster, which is just a string of
	 * characters. The expansion/unpakcing of this variable
	 * cluster is done in the higher node_manager class,
	 * where access to the engine object is present.
	 */
	__node_var = (
			(__ident >> '(' >> __node_pack >> ')') [_val = phoenix::construct <zhetapi::node> (phoenix::new_ <variable_cluster> (_1), _2)]
			| __ident [_val = phoenix::construct
				<zhetapi::node> (phoenix::new_
					<variable_cluster> (_1),
					::std::vector <zhetapi::node> {})]
		);

	/*
	 * Represents a parenthesized expression.
	 */
	__node_prth = "(" >> __start [_val = _1] >> ")";

	/*
	 * Represents a repeatable factor. Examples are
	 * variables and parenthesized expressions. The reason
	 * is because terms like x3(5 + 3) are much more awkward
	 * compared to 3x(5 + 3)
	 */
	__node_rept = __node_var | __node_prth;
	
	/*
	 * Represents a series of parenthesized expression,
	 * which are mutlilpied through the use of
	 * juxtaposition.
	 */
	__node_prep = __node_rept [_val = _1] >> *(
			(__node_rept) [_val = phoenix::construct
			<zhetapi::node> (phoenix::new_
				<operation_holder>
				(::std::string("*")), _val, _1)]
		);

	/*
	 * Represents a part of a term. For example, in the term
	 * 3x, 3 and x are both collectibles.
	 */
	__node_factor = (
			__node_prep [_val = _1]

			| __node_opd [_val = _1] >> *(
				(__node_rept) [_val =
				phoenix::construct
				<zhetapi::node> (phoenix::new_
					<operation_holder>
					(::std::string("*")),
					_val, _1)]
			)

			| __collection [_val = _1]
		);

	/*
	 * Represents a term as in any mathematical expression.
	 * Should be written without addition or subtraction
	 * unless in parenthesis.
	 */
	__node_term = (
			(__node_factor >> __power >> __node_term) [
				_val = phoenix::construct <zhetapi::node> (_2, _1, _3)
			]
			
			| (__t_pre >> __node_var) [
				_val = phoenix::construct <zhetapi::node> (_1, _2, true)
			]

			| (__node_var >> __t_post) [
				_val = phoenix::construct <zhetapi::node> (_2, _1, true)
			]

			| __node_factor [_val = _1] >> *(
				(__t1_bin >> __node_factor)
				[_val = phoenix::construct
				<zhetapi::node> (_1, _val, _2)]
			)

		);

	/*
	 * A full expression or function definition.
	 */
	__node_expr = __node_term [_val = _1] >> *(
			(__t0_bin >> __node_term) [_val =
			phoenix::construct <zhetapi::node> (_1,
				_val, _2)]
		);
	
	// Entry point
	__start = __node_expr;

	// Naming rules
	__start.name("start");

	__node_expr.name("node expression");
	__node_term.name("node term");
	__node_opd.name("node Operand");

	__plus.name("addition");
	__minus.name("substraction");
	__times.name("multiplication");
	__divide.name("division");
	__power.name("exponentiation");
	
	__o_str.name("literal operand");

	__o_z.name("integer operand");
	__o_q.name("rational operand");
	__o_r.name("real operand");
	__o_cz.name("complex integer operand");
	__o_cq.name("complex rational operand");
	__o_cr.name("complex real operand");
	
	__o_vz.name("vector integer operand");
	__o_vq.name("vector rational operand");
	__o_vr.name("vector real operand");
	__o_vgq.name("vector general rational operand");
	__o_vgr.name("vector general real operand");
	__o_vcz.name("vector complex integer operand");
	__o_vcq.name("vector complex rational operand");
	__o_vcr.name("vector complex real operand");
	__o_vcgq.name("vector complex general rational operand");
	__o_vcgr.name("vector complex general real operand");
	
	__o_mz.name("matrix integer Operand");
	__o_mq.name("matrix rational Operand");
	__o_mr.name("matrix real Operand");
	__o_mgq.name("matrix general rational Operand");
	__o_mgr.name("matrix general real Operand");
	__o_mcz.name("matrix complex integer Operand");
	__o_mcq.name("matrix complex rational Operand");
	__o_mcr.name("matrix complex real Operand");
	__o_mcgq.name("matrix complex general rational Operand");
	__o_mcgr.name("matrix complex general real Operand");

	__str.name("literal");

	__z.name("integer");
	__q.name("rational");
	__r.name("real");
	__gq.name("general rational");
	__gr.name("general real");
	__cz.name("complex integer");
	__cq.name("complex rational");
	__cr.name("complex real");
	__cgq.name("complex general rational");
	__cgr.name("complex general real");
	
	__vz.name("vector integer");
	__vq.name("vector rational");
	__vr.name("vector real");
	__vgq.name("vector general rational");
	__vgr.name("vector general real");
	__vcz.name("vector complex integer");
	__vcq.name("vector complex rational");
	__vcr.name("vector complex real");
	__vcgq.name("vector complex general rational");
	__vcgr.name("vector complex general real");
	
	__mz.name("matrix integer");
	__mq.name("matrix rational");
	__mr.name("matrix real");
	__mgq.name("matrix general rational");
	__mgr.name("matrix general real");
	__mcz.name("matrix complex integer");
	__mcq.name("matrix complex rational");
	__mcr.name("matrix complex real");
	__mcgq.name("matrix complex general rational");
	__mcgr.name("matrix complex general real");
	
	__vz_inter.name("intermediate vector integer");
	__vq_inter.name("intermediate vector rational");
	__vr_inter.name("intermediate vector real");
	__vgq_inter.name("intermediate vector general rational");
	__vgr_inter.name("intermediate vector general real");
	__vcz_inter.name("intermediate vector complex integer");
	__vcq_inter.name("intermediate vector complex rational");
	__vcr_inter.name("intermediate vector complex real");
	__vcgq_inter.name("intermediate vector complex general rational");
	__vcgr_inter.name("intermediate vector complex general real");
	
	__mz_inter.name("intermediate matrix integer");
	__mq_inter.name("intermediate matrix rational");
	__mr_inter.name("intermediate matrix real");
	__mgq_inter.name("intermediate matrix general rational");
	__mgr_inter.name("intermediate matrix general real");
	__mcz_inter.name("intermediate matrix complex integer");
	__mcq_inter.name("intermediate matrix complex rational");
	__mcr_inter.name("intermediate matrix complex real");
	__mcgq_inter.name("intermediate matrix complex general rational");
	__mcgr_inter.name("intermediate matrix complex general real");

#ifdef	ZHP_DEBUG

	debug(__start);

	debug(__node_expr);
	debug(__node_term);
	debug(__node_opd);

	debug(__plus);
	debug(__minus);
	debug(__times);
	debug(__divide);
	debug(__power);

	debug(__o_str);

	debug(__o_z);
	debug(__o_q);
	debug(__o_r);
	debug(__o_cz);
	debug(__o_cq);
	debug(__o_cr);

	debug(__o_vz);
	debug(__o_vq);
	debug(__o_vr);
	debug(__o_vgq);
	debug(__o_vgr);
	debug(__o_vcz);
	debug(__o_vcq);
	debug(__o_vcr);
	debug(__o_vcgq);
	debug(__o_vcgr);

	debug(__o_mz);
	debug(__o_mq);
	debug(__o_mr);
	debug(__o_mgq);
	debug(__o_mgr);
	debug(__o_mz);
	debug(__o_mcq);
	debug(__o_mcr);
	debug(__o_mcgq);
	debug(__o_mcgr);

	debug(__str);
	
	debug(__z);
	debug(__q);
	debug(__r);
	debug(__gq);
	debug(__gr);
	debug(__cz);
	debug(__cq);
	debug(__cr);
	debug(__cgq);
	debug(__cgr);

	debug(__vz);
	debug(__vq);
	debug(__vr);
	debug(__vgq);
	debug(__vgr);
	debug(__vcz);
	debug(__vcq);
	debug(__vcr);
	debug(__vcgq);
	debug(__vcgr);

	debug(__mz);
	debug(__mq);
	debug(__mr);
	debug(__mgq);
	debug(__mgr);
	debug(__mz);
	debug(__mcq);
	debug(__mcr);
	debug(__mcgq);
	debug(__mcgr);

#endif

}

}
