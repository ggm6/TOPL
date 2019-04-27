#ifndef LANG_HPP_INCLUDED
#define LANG_HPP_INCLUDED

#include <string>
#include <typeinfo>
#include <assert.h>

using namespace std;

class Expr {};
  /*
  The language is defined by the following sets
  of expressions.

  e ::= true
        false
        not e1
        e1 and e2
        e1 or e2

  v ::= true
        false
  */


class BoolExpr : public Expr
{
  
    string value;

  
  void __init__(string val) {this->value = val;}

  string __str__()
  {
    if (this->value=="true")
      return "true";
    else 
      return "false";
  }
};

class NotExpr : public Expr
{

  string expr;


  void __init__(string e) {this->expr = e;}

  string __str__() {return "(not {" + this->expr + "})";}
};

class AndExpr : public Expr
{

  string lhs;
  string rhs;


  void __init__(string lhs, string rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() { return "({" + this->lhs + "} and {" + this->rhs + "})";}
};

class OrExpr : public Expr
{

  string lhs;
  string rhs;


  void __init__(string lhs, string rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + this->lhs + "} or {" + this->rhs + "})";}
};

bool same_str(string e1, string e2) {return e1 == e2;}

template <typename T>
bool same(T e1, T e2) {
  /* Returns true when e1 and e2 the same
     string? (or when are the not the same). */

  // Quick reject.
  if (typeid(e1).name() != typeid(e2).name())
    return false;

  // What do I know about e1 and e2? They
  // have the same type.
  
  if (typeid(e1).name() == "BoolExpr")
    return e1.value == e2.value;


  if (typeid(e1).name() == "NotExpr")
    return same(e1.expr, e2.expr);

  if (typeid(e1).name() == "AndExpr")
    return same(e1.lhs, e2.lhs) && same(e1.rhs, e2.rhs);

  if (typeid(e1).name() == "OrExpr")
    return same(e1.lhs, e2.lhs) && same(e1.rhs, e2.rhs);
}

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
T step(T e) {
  //Compute the next state of the program.
  assert(is_reducible(e));

  if (typeid(e).name() == "NotExpr")
    return step_not(e);

  if (typeid(e).name() == "AndExpr")
    return step_and(e);

  if (typeid(e).name() == "OrExpr")
    return step_or(e);
}

template <typename T>
T reduce(T e) {
  while (is_reducible(e))
    e = step(e);
  return e;
}

#endif