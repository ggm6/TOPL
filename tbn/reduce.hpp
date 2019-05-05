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
  return typeid(e).name() == "BoolExpr" || typeid(e).name() == "IntExpr";
}

template<typename T>
void is_reducible(T e) {return !is_value(e);}

template<typename T1,typename T2>
T2 step_unary(T1 e,string op) {
  if (is_reducible(e.expr))
    return step(e.expr);

  return e.expr.value;
}

template<typename T1,typename T2>
T2 step_binary(T1 e,string op) {
  if (is_reducible(e.lhs))
    step(e.lhs);

  if (is_reducible(e.rhs))
    step(e.rhs);

  return e.expr.value;
}

template<typename T>
T step_not(T e) {
  return step_unary(e,"NotExpr");
}

template <typename T>
T step_and(T e) {
  return step_binary(e,"AndExpr");
}

template <typename T>
T step_or(T e) {
  return step_binary(e,"OrExpr");
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

template<typename T1,typename T2>
T2 step_add(T1 e) {
  return step_binary(e,"AddExpr");
}

template<typename T1,typename T2>
T2 step_sub(T1 e) {
  return step_binary(e,"SubExpr");
}

template<typename T1,typename T2>
T2 step_mul(T1 e) {
  return step_binary(e,"MulExpr");
}

template<typename T1,typename T2>
T2 step_div(T1 e) {
  return step_binary(e,"DivExpr");
}

template<typename T1,typename T2>
T2 step_rem(T1 e) {
  return step_binary(e,"RemExpr");
}

template<typename T1,typename T2>
T2 step_eq(T1 e) {
  return step_binary(e,"EqExpr");
}

template<typename T1,typename T2>
T2 step_ne(T1 e) {
  return step_binary(e,"NeExpr");
}

template<typename T1,typename T2>
T2 step_lt(T1 e) {
  return step_binary(e,"LtExpr");
}

template<typename T1,typename T2>
T2 step_gt(T1 e) {
  return step_binary(e,"GtExpr");
}

template<typename T1,typename T2>
T2 step_le(T1 e) {
  return step_binary(e,"LeExpr");
}

template<typename T1,typename T2>
T2 step_ge(T1 e) {
  return step_binary(e,"GeExpr");
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

  if (typeid(e).name() == "SubExpr")
    return step_sub(e);

  if (typeid(e).name() == "MulExpr")
    return step_mul(e);

  if (typeid(e).name() == "DivExpr")
    return step_div(e);

  if (typeid(e).name() == "RemExpr")
    return step_rem(e);

  if (typeid(e).name() == "NegExpr")
    return step_neg(e);

  if (typeid(e).name() == "EqExpr")
    return step_eq(e);

  if (typeid(e).name() == "NeExpr")
    return step_ne(e);

  if (typeid(e).name() == "LtExpr")
    return step_lt(e);

  if (typeid(e).name() == "GtExpr")
    return step_gt(e);

  if (typeid(e).name() == "LeExpr")
    return step_le(e);

  if (typeid(e).name() == "GeExpr")
    return step_ge(e);
}

template <typename T>
T reduce(T e) {
  while (is_reducible(e))
    e = step(e);
  return e;
}



#endif