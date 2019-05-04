#ifndef EVALUATE_HPP_INCLUDED
#define EVALUATE_HPP_INCLUDED

#include <vector>
#include <assert.h>
#include <typeinfo>
#include "lang.hpp"
#include "lookup.hpp"

using namespace std;

template <typename T,typename T1,typename T2>
class Closure {
	T1 abs = new T1;
	T2 env = new T2;

	void __init__(T1 abs,T2 env) {
		this->abs = abs;
		this->env = env;
	}
};

template<typename T>
T eval_bool(T e, vector<T> store) {
	return e.val;
}

template<typename T>
T eval_and(T e, vector<T> store) {
	return evaluate(e.lhs,store) && evaluate(e.rhs,store);
}

template<typename T>
T eval_or(T e, vector<T> store) {
	return evaluate(e.lhs,store) || evaluate(e.rhs,store);
}

template<typename T>
T eval_not(T e, vector<T> store) {
	return !evaluate(e.expr,store);
}

template<typename T>
T eval_cond(T e, vector<T> store) {
	if (evaluate(e.cond))
		return evaluate(e.True);
	else
		return evaluate(e.False);
}

template<typename T>
T eval_id(T e, vector<T> store) {
	return store[e.ref];
}

template<typename T,typename T1,typename T2>
T eval_abs(T e, vector<T> store) {
	Closure<T,T1,T2> expr;
	expr.__init__(e,store);
	return expr;
}

template<typename T>
T eval_app(T e, vector<T> store) {
	T c = evaluate(e.lhs,store);
	if (typeid(c).name() != "Closure")
		throw "cannot apply a non-closure to an argument";

	T v = evaluate(e.rhs,store);
	return evaluate(c.abs.expr,(vector<T>) {c.env,c.abs.var});
}

template<typename T,typename T1,typename T2>
T eval_lambda(T e, vector<T> store) {
	Closure<T,T1,T2> expr;
	expr.__init__(e,store);
	return expr;
}

template<typename T>
T eval_call(T e, vector<T> store) {
	T c = evaluate(e.fn,store);
	if (typeid(c).name() != "Closure")
		throw "cannot apply a non-closure to an argument";

	vector<T> args;
	for (auto it = args.begin(); it!=args.end(); ++it)
		args.push_back(evaluate(*it,store));

	vector<T> env;
	env = c.env;
	for (int i=0; i<sizeof(args)/sizeof(T); ++i)
		env[c.abs.vars[i]] = args[i];

	return evaluate(c.abs.expr,env);
}

template<typename T>
T evaluate(T e, vector<T> store) {
	if (typeid(e).name() == "BoolExpr")
		return eval_bool(e,store);

	if (typeid(e).name() == "AndExpr")
		return eval_and(e,store);

	if (typeid(e).name() == "OrExpr")
		return eval_or(e,store);

	if (typeid(e).name() == "NotExpr")
		return eval_not(e,store);

	if (typeid(e).name() == "IdExpr")
		return eval_id(e,store);

	if (typeid(e).name() == "AbsExpr")
		return eval_abs(e,store);

	if (typeid(e).name() == "AppExpr")
		return eval_app(e,store);

	if (typeid(e).name() == "LambdaExpr")
		return eval_lambda(e,store);

	if (typeid(e).name() == "CallExpr")
		return eval_call(e,store);
}


#endif