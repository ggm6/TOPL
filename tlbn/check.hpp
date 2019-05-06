#ifndef CHECK_HPP_INCLUDED
#define CHECK_HPP_INCLUDED

#include <typeinfo>
#include <assert.h>
#include <string>
#include "lang.hpp"

using namespace std;

BoolType boolType;

IntType intType;

template<typename T>
bool is_bool(T x) {
	if (isinstance(x,"Type"))
		return x == boolType;
	if (isinstance(x,"Expr"))
		return is_bool(check(x));
}

template<typename T>
bool is_int(T x) {
	if (isinstance(x,"Type"))
		return x == intType;
	if (isinstance(x,"Expr"))
		return is_int(check(x));
}

template<typename T1,typename T2>
bool is_same_type(T1 t1, T2 t2) {
	if (typeid(t1).name() != typeid(t2).name())
		return false;

	if (typeid(t1).name() == "BoolType")
		return true;

	if (typeid(t1).name() == "IntType")
		return true;
}

template<typename T1,typename T2>
bool has_same_type(T1 e1, T2 e2) {
	return is_same_type(check(e1),check(e2));
}

template<typename T>
BoolType check_bool(T e) {
	return boolType;
}

template<typename T>
IntType check_int(T e) {
	return intType;
}

template<typename T>
BoolType check_and(T e) {
	if (is_bool(e.lhs) && is_bool(e.rhs))
		return boolType;

	throw "invalid operands to 'and'";
}

template<typename T>
BoolType check_add(T e) {
	if (is_int(e.lhs) && is_int(e.rhs))
		return intType;

	throw "invalid operands to '+'";
}

template<typename T>
BoolType check_sub(T e) {
	if (is_int(e.lhs) && is_int(e.rhs))
		return intType;

	throw "invalid operands to '-'";
}

template<typename T>
BoolType check_eq(T e) {
	if (has_same_type(e.lhs,e.rhs))
		return boolType;

	throw "invalid operands to '=='";
}

template <typename T1,typename T2>
T2 do_check(T1 e) {
	assert(isinstance(e,"Expr"));
	if (typeid(e).name() == "BoolExpr")
		return check_bool(e);

	if (typeid(e).name() == "AndExpr")
		return check_and(e);

	if (typeid(e).name() == "OrExpr")
		return check_or(e);

	if (typeid(e).name() == "NotExpr")
		return check_not(e);

	if (typeid(e).name() == "IfExpr")
		return check_if(e);

	if (typeid(e).name() == "IntExpr")
		return check_int(e);

	if (typeid(e).name() == "AddExpr")
		return check_add(e);

	if (typeid(e).name() == "SubExpr")
		return check_sub(e);

	if (typeid(e).name() == "MulExpr")
		return check_mul(e);

	if (typeid(e).name() == "DivExpr")
		return check_div(e);

	if (typeid(e).name() == "RemExpr")
		return check_rem(e);

	if (typeid(e).name() == "NegExpr")
		return check_neg(e);

	if (typeid(e).name() == "EqExpr")
		return check_eq(e);

	if (typeid(e).name() == "NeExpr")
		return check_ne(e);

	if (typeid(e).name() == "LtExpr")
		return check_lt(e);

	if (typeid(e).name() == "GtExpr")
		return check_gt(e);

	if (typeid(e).name() == "LeExpr")
		return check_le(e);

	if (typeid(e).name() == "GeExpr")
		return check_ge(e);
}

template<typename T>
string check(T e) {
	if (!e.type)
		return to_string(do_check(e));

	return e.type;
}

#endif