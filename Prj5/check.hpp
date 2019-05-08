#ifndef CHECK_HPP_INCLUDED
#define CHECK_HPP_INCLUDED

#include <typeinfo>
#include <assert.h>
#include <string>
#include <iterator>
#include "lang.hpp"

using namespace std;

template<typename T>
bool is_type(T t) {
	return typeid(t).name() == "TupleType";
}

template<typename T>
bool is_record(T t) {
	return typeid(t).name() == "RecordType";
}

template<typename T>
bool is_variant(T t) {
	return typeid(t).name() == "VariantType";
}

template<typename T1,typename T2>
bool is_same_type(T1 t1, T2 t2) {
	if (typeid(t1).name() != typeid(t2).name())
		return false;

	if (typeid(t1).name() == "BoolType")
		return true;

	if (typeid(t1).name() == "IntType")
		return true;

	if (typeid(t1).name() == "FnType") {
		if (!is_same_type(t1,t2))
			return false;
		return is_same_type(t1.ref,t2.ref);
	}

	if (typeid(t1).name() == "RefType")
		return is_same_type(t1.ref,t2.ref);
}

template<typename T>
bool is_bool(T x) {
	if (isinstance(x,"Type"))
		return x == boolType;
}

template<typename T>
bool is_int(T x) {
	if (isinstance(x,"Type"))
		return x == intType;
}

template<typename T>
bool is_reference_to(T t,T u) {
	return is_reference<T>(t) && is_same_type(t.ref,u);
}

template<typename T1,typename T2>
bool has_same_type(T1 e1, T2 e2) {
	return is_same_type(check(e1),check(e2));
}

template<typename T>
bool has_bool(T e) {
	return is_same_type(check(e),boolType);
}

template<typename T>
bool has_int(T e) {
	return is_same_type(check(e),intType);
}

template<typename T>
BoolType check_bool(T e) {
	return boolType;
}

template<typename T>
IntType check_int(T e) {
	return intType;
}

template<typename T1,typename T2>
BoolType check_logical_unary(T1 e,T2 op) {
	if (is_bool(e.expr))
		return boolType;
	throw "invalid operands to " + to_string(op);
}

template<typename T1,typename T2>
BoolType check_logical_binary(T1 e,T2 op) {
	if (is_bool(e.lhs) && is_bool(e.rhs))
		return boolType;
	throw "invalid operands to " + to_string(op);
}

template<typename T>
BoolType check_and(T e) {
	return check_logical_binary(e,"and");
}

template<typename T>
BoolType check_or(T e) {
	return check_logical_binary(e,"or");
}

template<typename T1,typename T2>
IntType check_arithmetic_binary(T1 e,T2 op) {
	if (is_int(e.lhs) && is_int(e.rhs))
		return intType;
	throw "invalid operands to " + to_string(op);
}

template<typename T>
IntType check_add(T e) {
	return check_arithmetic_binary(e,"+");
}

template<typename T>
IntType check_sub(T e) {
	return check_arithmetic_binary(e,"-");
}

template<typename T>
IntType check_mul(T e) {
	return check_arithmetic_binary(e,"*");
}

template<typename T>
IntType check_div(T e) {
	return check_arithmetic_binary(e,"/");
}

template<typename T>
IntType check_rem(T e) {
	return check_arithmetic_binary(e,"%");
}

template<typename T,typename T2>
BoolType check_relational(T e,T2 op) {
	if (has_same_type(e.lhs,e.rhs))
		return boolType;
	throw "invalid operands to " + to_string(op);
}

template<typename T>
BoolType check_eq(T e) {
	return check_relational(e,"==");
}

template<typename T>
BoolType check_ne(T e) {
	return check_relational(e,"!=");
}

template<typename T>
BoolType check_lt(T e) {
	return check_relational(e,"<");
}

template<typename T>
BoolType check_gt(T e) {
	return check_relational(e,">");
}

template<typename T>
BoolType check_le(T e) {
	return check_relational(e,"<=");
}

template<typename T>
BoolType check_ge(T e) {
	return check_relational(e,">=");
}

template<typename T>
T check_id(T e) {
	return e.ref.type;
}

template<typename T>
T check_lambda(T e) {
	vector<T> parms;
	for (auto p=e.vars.begin(); p!=e.vars.end(); ++p)
		parms.push_back(check(*p));
	T ret = check(e.expr);
	return FnType<T,T>(parms,ret);
}

template<typename T>
T check_call(T e) {
	T t = check(e.fn);
	if (!is_function<T>(t))
		throw "invalid function call";

	if (e.args.size() < t.parms.size())
		throw "too few arguments";

	if (e.args.size() > t.parms.size())
		throw "too many arguments";

	for (int i=0; i<e.args.size(); ++i) {
		T arg = check(e.args[i]);
		T parm = t.parms[i];
		if (!is_same_type(arg,parm))
			throw "parameter/argument mismatch";
	}
	return t.ret;
}

template<typename T>
T check_new(T e) {
	T t = check(e.expr);
	return RefType<T>(t);
}

template<typename T>
T check_deref(T e) {
	T t = check(e.expr);
	if (!is_reference<T>(t))
		throw "cannot dereference a non-reference";
	return t.ref;
}

template <typename T,typename T2>
void check_assign(T e) {
	T t1 = check(e.lhs);
	if (!is_reference<T>(t1))
		throw "operand is not a reference";

	T2 t2 = check(e.rhs);
	if (!is_reference_to(t1,t2))
		throw "type mismatch in assignment";
}

template <typename T1,typename T2>
T2 do_check(T1 e) {
	assert(isinstance(e,"Expr"));
	if (typeid(e).name() == "BoolExpr")
		return check_bool(e);

	if (typeid(e).name() == "AndExpr")
		return check_logical_binary(e,"and");

	if (typeid(e).name() == "OrExpr")
		return check_logical_binary(e,"or");

	if (typeid(e).name() == "NotExpr")
		return check_logical_unary(e,"not");

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

	if (typeid(e).name() == "IdExpr")
		return check_id(e);

	if (typeid(e).name() == "LambdaExpr")
		return check_lambda(e);

	if (typeid(e).name() == "CallExpr")
		return check_call(e);

	if (typeid(e).name() == "NewExpr")
		return check_new(e);

	if (typeid(e).name() == "DerefExpr")
		return check_deref(e);

	if (typeid(e).name() == "AssignExpr")
		return check_assign(e);
}

template<typename T>
string check(T e) {
	if (!e.type)
		return to_string(do_check(e));

	return e.type;
}

#endif