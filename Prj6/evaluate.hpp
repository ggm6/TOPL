#ifndef EVALUATE_HPP_INCLUDED
#define EVALUATE_HPP_INCLUDED

#include <assert.h>
#include <typeinfo>
#include <iterator>
#include "lang.hpp"
#include "decorate.hpp"

using namespace std;

template <typename T1,typename T2>
class Closure {
	T1 abs = new T1;
	T2 env = new T2;

	void __init__(T1 abs,T2 env) {
		this->abs = abs;
		this->env = env;
	}
};

template <typename T>
class Location {
	T index;

	void __init__(T ix) {
		this->index = ix;
	}
};

template<typename T,typename T1,typename T2,typename T3,typename T4>
T eval_binary(T1 e,vector<T2> stack,vector<T3> heap,T4 object) {
	T v1 = evaluate(e.lhs,stack,heap);
	T v2 = evaluate(e.rhs,stack,heap);
	return v1;
}

template<typename T,typename T1,typename T2,typename T3,typename T4>
T eval_unary(T1 e,vector<T2> stack,vector<T3> heap,T4 object) {
	T v1 = evaluate(e.rhs,stack,heap);
	return v1;
}

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

template <typename T,typename T2>
T eval_lambda(T e, vector<T2> stack) {
	Closure<T,T2> expr;
	expr.__init__(e,stack);
	return expr;
}

template <typename T>
T eval_call(T e, vector<T> stack, vector<T> heap) {
	T c = evaluate(e.fn,stack,heap);
	if (typeid(c).name() != "Closure")
		throw "cannot apply a non-closure to an argument";

	vector<T> args;
	for (auto a = e.args.begin(); a!=e.args.end(); ++a)
		args.push_back(evaluate(*a,stack,heap));

	T env = new T;
	env = c.env;
	for (int i=0; i<args.size(); ++i)
		env[c.abs.vars[i]] = args[i];

	return evaluate(c.abs.expr,env);
}

template <typename T>
T eval_new(T e, vector<T> stack, vector<T> heap) {
	T v1 = evaluate(e.expr,stack,heap);
	T l1 = Location<T>(heap.size());
	heap.push_back(v1);
	return l1;
}

template <typename T>
T eval_deref(T e, vector<T> stack, vector<T> heap) {
	T l1 = evaluate(e.expr,stack,heap);
	if (typeid(l1).name() != "Location")
		throw "invalid reference";
	return heap[l1.index];
}

template <typename T>
T eval_assign(T e, vector<T> stack, vector<T> heap) {
	T v2 = evaluate(e.rhs,stack,heap);
	T l1 = evaluate(e.lhs,stack,heap);
	if (typeid(l1).name() != "Location")
		throw "invalid reference";
	heap[l1.index] = v2;
}


template<typename T>
T evaluate(T e, vector<T> stack, vector<T> heap) {
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

	if (typeid(e).name() == "LambdaExpr")
		return eval_lambda(e);

	if (typeid(e).name() == "CallExpr")
		return eval_call(e);

	if (typeid(e).name() == "NewExpr")
		return eval_new(e);

	if (typeid(e).name() == "DerefExpr")
		return eval_deref(e);

	if (typeid(e).name() == "AssignExpr")
		return eval_assign(e);
}

#endif