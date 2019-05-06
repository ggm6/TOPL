#ifndef EVALUATE_HPP_INCLUDED
#define EVALUATE_HPP_INCLUDED

#include <assert.h>
#include <typeinfo>
#include "lang.hpp"

using namespace std;

template <typename T>
T eval_bool(T e) {
	return e.value;
}

template <typename T>
bool eval_and(T e) {
	return evaluate(e.lhs) && evaluate(e.rhs);
}

template <typename T>
bool eval_or(T e) {
	return evaluate(e.lhs) || evaluate(e.rhs);
}

template<typename T>
bool eval_not(T e) {
	return !evaluate(e.expr);
}

template<typename T>
bool eval_if(T e) {
	if (evaluate(e.cond))
		return evaluate(e.True);
	else
		return evaluate(e.False);
}

template<typename T>
T eval_int(T e) {
	return e.value;
}

template<typename T>
T eval_add(T e) {
	return evaluate(e.lhs) + evaluate(e.rhs);
}

template<typename T>
T eval_sub(T e) {
	return evaluate(e.lhs) - evaluate(e.rhs);
}

template<typename T>
T eval_mul(T e) {
	return evaluate(e.lhs) * evaluate(e.rhs);
}

template<typename T>
T eval_div(T e) {
	return evaluate(e.lhs) / evaluate(e.rhs);
}

template<typename T>
T eval_rem(T e) {
	return evaluate(e.lhs) % evaluate(e.rhs);
}

template<typename T>
T eval_neg(T e) {
	return -evaluate(e.expr);
}

template<typename T>
bool eval_eq(T e) {
	return evaluate(e.lhs) == evaluate(e.rhs);
}

template<typename T>
T eval_ne(T e) {
	return evaluate(e.lhs) != evaluate(e.rhs);
}

template<typename T>
T eval_lt(T e) {
	return evaluate(e.lhs) < evaluate(e.rhs);
}

template<typename T>
T eval_gt(T e) {
	return evaluate(e.lhs) > evaluate(e.rhs);
}

template<typename T>
T eval_le(T e) {
	return evaluate(e.lhs) <= evaluate(e.rhs);
}

template<typename T>
T eval_ge(T e) {
	return evaluate(e.lhs) >= evaluate(e.rhs);
}

template<typename T>
T evaluate(T e) {
	assert(isinstance(e,"Expr"));
	if (typeid(e).name() == "BoolExpr")
		return eval_bool(e);

	if (typeid(e).name() == "AndExpr")
		return eval_and(e);

	if (typeid(e).name() == "OrExpr")
		return eval_or(e);

	if (typeid(e).name() == "NotExpr")
		return eval_not(e);

	if (typeid(e).name() == "IfExpr")
		return eval_if(e);

	if (typeid(e).name() == "IntExpr")
		return eval_int(e);

	if (typeid(e).name() == "AddExpr")
		return eval_add(e);

	if (typeid(e).name() == "SubExpr")
		return eval_sub(e);

	if (typeid(e).name() == "MulExpr")
		return eval_mul(e);

	if (typeid(e).name() == "DivExpr")
		return eval_div(e);

	if (typeid(e).name() == "RemExpr")
		return eval_rem(e);

	if (typeid(e).name() == "NegExpr")
		return eval_neg(e);

	if (typeid(e).name() == "EqExpr")
		return eval_eq(e);

	if (typeid(e).name() == "NeExpr")
		return eval_ne(e);

	if (typeid(e).name() == "LtExpr")
		return eval_lt(e);

	if (typeid(e).name() == "GtExpr")
		return eval_gt(e);

	if (typeid(e).name() == "LeExpr")
		return eval_le(e);

	if (typeid(e).name() == "GeExpr")
		return eval_ge(e);
}

#endif