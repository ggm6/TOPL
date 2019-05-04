#ifndef CURRY_HPP_INCLUDED
#define CURRY_HPP_INCLUDED

#include <vector>
#include <typeinfo>
#include <string>
#include "lang.hpp"

using namespace std;

template<typename T>
string curry_unary(T e, string Node) {
	return curry(e.expr);
}

template<typename T>
string curry_binary(T e, string Node) {
	return curry(e.lhs) + curry(e.rhs);
}

template<typename T>
string curry_app(T e) {
	if (typeid(e.rhs).name() == "PlaceholderExpr")
		return typeid(e.lhs).name();
	return curry_binary(e,"AppExpr");
}

template<typename T>
vector<string> find_placeholders(vector<string> args) {
	vector<string> phs;
	for (int i=0; i<args.size(); ++i) {
		if (typeid(args[i]).name() == "PlaceholderExpr")
			phs.push_back(args[i]);
	}

	return phs;
}

template<typename T>
T curry_call(T e) {
	std::vector<string> phs = find_placeholders(e.args);
	if(phs.empty()) {
		CallExpr expr;
		expr.__init__(e.fn.vars,e.args);
		return expr;
	}

	vector<string> vars;
	for (int i=0;i<phs.size();++i) {
		VarDecl expr;
		expr.__init__("_"+e.fn.vars[i].id);
		vars.push_back(expr.__str__());
	}

	int n = 0;
	for (int i=0;i<phs.size();++i) {
		IdExpr expr;
		expr.__init__(vars[n]);
		e.args[i] = expr;
		n += 1;
	}

	return (LambdaExpr) {vars,e};
}

template<typename T>
string curry(T e) {
	if (typeid(e).name() == "BoolExpr")
		return "BoolExpr";

	if (typeid(e).name() == "AndExpr")
		return curry_binary(e,"AndExpr");

	if (typeid(e).name() == "OrExpr")
		return curry_binary(e,"OrExpr");

	if (typeid(e).name() == "NotExpr")
		return curry_unary(e,"NotExpr");

	if (typeid(e).name() == "IfExpr")
		return "IfExpr";

	if (typeid(e).name() == "IdExpr")
		return e.__str__();

	if (typeid(e).name() == "AbsExpr")
		return "AbsExpr";

	if (typeid(e).name() == "AppExpr")
		return curry_app(e);

	if (typeid(e).name() == "LambdaExpr")
		return "LambdaExpr";

	if (typeid(e).name() == "CallExpr")
		return curry_call(e);
}

#endif