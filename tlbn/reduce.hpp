#ifndef REDUCE_HPP_INCLUDED
#define REDUCE_HPP_INCLUDED

#include <string>
#include <vector>
#include <typeinfo>
#include <assert.h>
#include "lang.hpp"

using namespace std;

template<typename T>
bool is_value(T e) {
  /*Returns true if e is a value (i.e.,
     irreducible).*/
  return typeid(e).name() == "BoolExpr" || typeid(e).name() == "AbsExpr" || typeid(e).name() == "LambdaExpr";
}

template<typename T>
void is_reducible(T e) {return !is_value(e);}

template <typename T>
T step_and(T e) {
  if (is_reducible(e.lhs)) {
    AndExpr expr;
    expr.__init__(step(e.lhs),e.rhs);
    return expr;
  }

  if (is_reducible(e.rhs)) {
    AndExpr expr;
    expr.__init__(e.lhs,step(e.rhs));
    return expr;
  }

  BoolExpr expr;
  expr.__init__(e.lhs.val + " && " + e.rhs.val);
  return expr;
}

template <typename T>
T step_or(T e) {
  if (is_reducible(e.lhs)) {
    AndExpr expr;
    expr.__init__(step(e.lhs),e.rhs);
    return expr;
  }

  if (is_reducible(e.rhs)) {
    AndExpr expr;
    expr.__init__(e.lhs,step(e.rhs));
    return expr;
  }

  BoolExpr expr;
  expr.__init__(e.lhs.val + " || " + e.rhs.val);
  return expr;
}

template<typename T>
T step_not(T e) {
  if (is_reducible(e.expr)) {
    NotExpr expr;
    expr.__init__(step(e.expr));
    return expr;
  }
  BoolExpr expr;
  expr.__init__("!" + e.expr.val);
  return expr;
}

template <typename T>
T step_if(T e) {
  if (is_reducible(e.cond))
    return NotExpr(step(e.cond), e.True, e.False);

  if (e.cond.val)
    return e.True;
  else
    return e.False;
}

template<typename T>
T step_app(T e) { 
  if (is_reducible(e.lhs))
    return AppExpr(step(e.lhs), e.rhs);

  if (typeid(e.lhs).name() != "AbsExpr")
    throw "application of non-lambda";

  if (is_reducible(e.rhs))
    return AppExpr(e.lhs, step(e.rhs));

  std::vector<T> s;
  s.push_back(e.lhs,e.rhs);
  return subst(e.lhs.expr, s);
}

template <typename T>
T step_call(T e) {

  if (is_reducible(e.fn)) {
    CallExpr expr;
    expr.__init__(step(e.fn),e.args);
    return expr;
  }

  if (sizeof(e.args)/sizeof(T) < sizeof(e.fn.vars)/sizeof(T))
    throw "too few arguments";

  if (sizeof(e.args)/sizeof(T) > sizeof(e.fn.vars)/sizeof(T))
    throw "too many arguments";

  for (int i=0; i < sizeof (e.args)/sizeof(T); ++i)
    if (is_reducible(e.args[i])) {
      CallExpr expr;
    expr.__init__(step(e.fn),e.args);
    return expr;
    }

  vector<T> s;
  for (int i=0; i < sizeof(e.args)/sizeof(T); ++i)
    s[e.fn.vars[i]] = e.args[i];

  return subst(e.fn.expr, s);
}

template <typename T>
T step(T e) {
  assert(isinstance(e, "Expr"));
  assert(is_reducible(e));


  if (typeid(e).name() == "NotExpr")
    return step_not(e);

  if (typeid(e).name() == "AndExpr")
    return step_and(e);

  if (typeid(e).name() == "OrExpr")
    return step_or(e);

  if (typeid(e).name() == "IfExpr")
    return step_if(e);

  if (typeid(e).name() == "AddExpr")
    return step_add(e);

  if (typeid(e).name() == "AppExpr")
    return step_app(e);

  if (typeid(e).name() == "CallExpr")
    return step_call(e);
}

template <typename T>
T reduce(T e) {
  while (is_reducible(e))
    e = step(e);
  return e;
}

#endif