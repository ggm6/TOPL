#ifndef SUBST_HPP_INCLUDED
#define SUBST_HPP_INCLUDED

#include <vector>
#include <string>
#include "lang.hpp"

using namespace std;

template <typename T>
T subst(T e,vector<T> s) {
  if (typeid(e).name() == "BoolExpr") {return e;}

  if (typeid(e).name() == "AndExpr") {
    T e1 = subst(e.lhs, s);
    T e2 = subst(e.rhs, s);
    return AndExpr(e1, e2);
  }

  if (typeid(e).name() == "OrExpr") {
    T e1 = subst(e.lhs, s);
    T e2 = subst(e.rhs, s);
    return OrExpr(e1, e2);
  }

  if (typeid(e).name() == "NotExpr") {
    T e1 = subst(e.expr, s);
    return NotExpr(e1);
  }

  if (typeid(e).name() == "IfExpr") {
    T e1 = subst(e.cond, s);
    T e2 = subst(e.True, s);
    T e3 = subst(e.False, s);
    IfExpr<T,T,T> expr;
    expr.__init__(e1,e2,e3);
    return expr;
  }

  if (typid(e).name() == "IdExpr") {
    for (int i=0; i<s.size(); ++i) {
      if (e.ref == s[i])
        return s[e.ref];
    }
    return e;
  }

  if (typeid(e).name() == "AbsExpr")
    return AbsExpr(e.var,subst(e.expr,s));

  if (typeid(e).name() == "AppExpr")
    return AppExpr(subst(e.lhs,s), subst(e.rhs,s));

  if (typeid(e).name() == "LambdaExpr") {
    T e1 = subst(e.expr, s);
    return LambdaExpr(e.vars, e1);
  }

  if (typeid(e).name() == "CallExpr") {
    T e0 = subst(e.fn, s);
    vector<string> args;
    return CallExpr(e0, args);
  }
}

#endif