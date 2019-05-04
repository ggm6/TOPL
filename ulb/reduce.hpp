#ifndef REDUCE_HPP_INCLUDED
#define REDUCE_HPP_INCLUDED

#include <string>
#include <vector>
#include <typeinfo>
#include <assert.h>
#include "lang.hpp"
#include "subst.hpp"

using namespace std;

template<typename T>
bool is_value(T e) {
  /*Returns true if e is a value (i.e.,
     irreducible).*/
  return typeid(e).name() == "BoolExpr";
}

template<typename T>
void is_reducible(T e) {return !is_value(e);}

template<typename T>
T step_not(T e) {
  // ----------------- Not-T
  // not true -> false
  //
  // ----------------- Not-F
  // not false -> true
  //
  // Alternative for above:
  //
  // -----------------
  // not v1 -> `not [v1]`
  //
  //     e1 -> e1'
  // ----------------- Not-E
  // not e1 -> not e1'
  
  if (is_value(e))
    return BoolExpr(!e.value);
    // if e.expr.value == True: // not true
    //   return BoolExpr(False)
    // else:
    //   return BoolExpr(True) // not false

  return NotExpr(step(e));
}

template <typename T>
T step_and(T e) {
  // ---------------------------- And-V
  // v1 and v2 -> `[v1] and [v2]`
  //
  //          e1 -> e1'
  // --------------------------- And-L
  //   e1 and e2 -> e1' and e2

  //          e2 -> e2'
  // --------------------------- And-R
  //   v1 and e2 -> v1 and e2'
  if (is_value(e.lhs) && is_value(e.rhs))
    return BoolExpr(e.lhs.value && e.rhs.value);

  if (is_reducible(e.lhs)) // Applies And-L
    return AndExpr(step(e.lhs), e.rhs);

  if (is_reducible(e.rhs)) // Applies And-R
    return AndExpr(e.lhs, step(e.rhs));
}

template <typename T>
T step_or(T e) {
  // ---------------------------- Or-V
  // v1 or v2 -> `[v1] or [v2]`
  //
  //          e1 -> e1'
  // --------------------------- Or-L
  //   e1 or e2 -> e1' or e2

  //          e2 -> e2'
  // --------------------------- Or-R
  //   v1 or e2 -> v1 or e2'
  if (is_value(e.lhs) && is_value(e.rhs))
    return BoolExpr(e.lhs.value || e.rhs.value);

  if (is_reducible(e.lhs)) // Applies Or-L
    return OrExpr(step(e.lhs), e.rhs);

  if (is_reducible(e.rhs)) // Applies Or-R
    return OrExpr(e.lhs, step(e.rhs));
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

template <typename T>
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