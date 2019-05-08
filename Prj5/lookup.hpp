#ifndef LOOKUP_HPP_INCLUDED
#define LOOKUP_HPP_INCLUDED

#include <vector>
#include <typeinfo>
#include <assert.h>
#include <vector>
#include "lang.hpp"

using namespace std;

template<typename T>
T resolve_unary(T e,vector<T> stk) {
  resolve(e.expr,stk);
  return e;
}

template<typename T>
T resolve_binary(T e,vector<T> stk) {
  resolve(e.lhs,stk);
  resolve(e.rhs,stk);
  return e;
}

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

  if (typeid(e).name() == "IntExpr")
    return e;

  if (typeid(e).name() == "AddExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "SubExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "MulExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "DivExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "RemExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "NegExpr")
    return resolve_unary(e,stk);

  if (typeid(e).name() == "EqExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "NeExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "LtExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "GtExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "LeExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "GeExpr")
    return resolve_binary(e,stk);

  if (typeid(e).name() == "IdExpr") {
    resolve(e,stk);
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

  if (typeid(e).name() == "NewExpr")
    return resolve_unary(e,stk);

  if (typeid(e).name() == "DerefExpr")
    return resolve_unary(e,stk);

  if (typeid(e).name() == "AssignExpr")
    return resolve_binary(e,stk);
}

#endif