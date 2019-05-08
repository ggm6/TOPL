#ifndef INSTANTIATE_HPP_INCLUDED
#define INSTANTIATE_HPP_INCLUDED

#include <vector>
#include "lang.hpp"
#include "decorate.hpp"

using namespace std;

template <typename T>
T inst_unary_expr(T e,string s,T object) {
	T e1 = instantiate(e.expr);
	return e1;
}

template <typename T>
T inst_binary_expr(T e,string s,T object) {
	T e1 = instantiate(e.lhs);
	T e2 = instantiate(e.rhs);
	BoolExpr expr;
	expr.__init__(e1,e2);
	return expr;
}

template <typename T>
T instantiate(T e,string s) {
	if (typeid(e).name()=="BoolExpr")
		return e;

	if (typeid(e).name()=="AndExpr")
		return inst_binary_expr(e,s,(AndExpr) {});

	if (typeid(e).name()=="OrExpr")
		return inst_binary_expr(e,s,(OrExpr) {});

	if (typeid(e).name()=="NotExpr")
		return inst_unary_expr(e,s,(NotExpr) {});

	if (typeid(e).name()=="IfExpr") {
		T e1 = instantiate(e.cond,s);
		T e2 = instantiate(e.True,s);
		T e3 = instantiate(e.False,s);
		IfExpr<T,T,T> expr;
		expr.__init__(e1,e2,e3);
		return expr;
	}

	if (typeid(e).name()=="IntExpr")
		return e;

	if (typeid(e).name()=="AddExpr")
		return inst_binary_expr(e,s,(AddExpr<T>) {});

	if (typeid(e).name()=="SubExpr")
		return inst_binary_expr(e,s,(SubExpr<T>) {});

	if (typeid(e).name()=="MulExpr")
		return inst_binary_expr(e,s,(MulExpr<T>) {});

	if (typeid(e).name()=="DivExpr")
		return inst_binary_expr(e,s,(DivExpr<T>) {});

	if (typeid(e).name()=="RemExpr")
		return inst_binary_expr(e,s,(RemExpr<T>) {});

	if (typeid(e).name()=="NegExpr")
		return inst_unary_expr(e,s,(NegExpr<T>) {});

	if (typeid(e).name()=="EqExpr")
		return inst_binary_expr(e,s,(EqExpr<T>) {});

	if (typeid(e).name()=="NeExpr")
		return inst_binary_expr(e,s,(NeExpr<T>) {});

	if (typeid(e).name()=="LtExpr")
		return inst_binary_expr(e,s,(LtExpr<T>) {});

	if (typeid(e).name()=="GtExpr")
		return inst_binary_expr(e,s,(GtExpr<T>) {});

	if (typeid(e).name()=="LeExpr")
		return inst_binary_expr(e,s,(LeExpr<T>) {});

	if (typeid(e).name()=="GeExpr")
		return inst_binary_expr(e,s,(GeExpr<T>) {});

	if (typeid(e).name()=="IdExpr") {
		IdExpr expr;
		expr.__init__(e.id);
		return expr;
	}

	if (typeid(e).name()=="LambdaExpr") {
		vector<T> ps;
		ps.push_back(e.vars);
		T e1 = instantiate(e.expr,s);
		LambdaExpr expr;
		expr.__init__(e.vars,e1);
		return expr;
	}

	if (typeid(e).name()=="CallExpr") {
		T e = instantiate(e.fn,s);
		vector<T> es;
		es.push_back(e.args);
		
		CallExpr expr;
		expr.__init__(e,es);
		return expr;
	}

	if (typeid(e).name()=="TupleExpr") {
		TupleExpr<T> expr;
		expr.__init__(e.elems,s);
		return expr;
	}

	if (typeid(e).name()=="ProjExpr") {
		ProjExpr<T> expr;
		expr.__init__(e.fields,s);
		return expr;
	}

	if (typeid(e).name()=="RecordExpr") {
		vector<T> fs;
		for (T f : e.fields) {
			fs.push_back(f.id,e);
		}

		RecordExpr<T> expr;
		expr.__init__(fs);
		return expr;
	}

	if (typeid(e).name()=="MemberExpr") {
		MemberExpr<T> expr;
		expr.__init__(e.obj,e.id);
		return expr;
	}

	if (typeid(e).name()=="VariantExpr") {
		return;
	}

	if (typeid(e).name()=="CaseExpr") {
		return;
	}

	if (typeid(e).name()=="GenericExpr") {
		return e;
	}

	throw "unknown expression";
}

#endif