#ifndef LOOKUP_HPP_INCLUDED
#define LOOKUP_HPP_INCLUDED

#include <vector>
#include <typeinfo>
#include <assert.h>
#include "lang.hpp"

using namespace std;

template <typename T>
T resolve(T e, vector<T> stk) {

  if (typeid(e).name() == "BoolExpr")
    return e;

  if (typeid(e).name() == "AndExpr") {
    resolve(e.lhs, stk);
    resolve(e.rhs, stk);
    return e;
  }

  if (typeid(e).name() == "OrExpr") {
    resolve(e.lhs, stk);
    resolve(e.rhs, stk);
    return e;
  }

  if (typeid(e).name() == "NotExpr") {
    resolve(e.expr, stk);
    return e;
  }

  if (typeid(e).name() == "IfExpr") {
    resolve(e.cond, stk);
    resolve(e.True, stk);
    resolve(e.False, stk);
    return e;
  }

  if (typeid(e).name() == "IdExpr") {
    resolve(e,stk);
    return e;
  }

  if (typeid(e).name() == "AbsExpr") {
  	stk.push_back(e.vars);
    resolve(e.expr, stk);
    return e;
  }

  if (typeid(e).name() == "AppExpr") {
    resolve(e.lhs, stk);
    resolve(e.rhs, stk);
    return e;
  }

  if (typeid(e).name() == "LambdaExpr") {
  	stk.push_back(e.vars);
  	resolve(e.expr,stk);
  	return e;
  }

  if (typeid(e).name() == "CallExpr") {
    resolve(e.fn, stk);
    for (T a : e.args)
      resolve(e.fn, stk);
    return e;
  }
}

#endif