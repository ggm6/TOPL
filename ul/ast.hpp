#ifndef AST_HPP_INCLUDED
#define AST_HPP_INCLUDED

#include <string>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include <assert.h>

using namespace std;

class Expr {};
  // Represents the set of expressions in the
  // pure (or untyped) lambda calculus. This is
  // defined as:
  // 
  //   e ::= x                     -- variables
  //         \\x.e1                -- abstractions
  //         e1 e2                 -- application
  //         \(x1, x2, ..., xn).e1 -- lambda expression
  //         e0(e1, e2, ..., en)   -- call expression

class IdExpr : public Expr
{
  string id;
  string ref;

  void __init__(string id) {
    this->id = id;
    this->ref = "";
  }

  string __str__() {return this->id;}

};
 
class VarDecl
{
  string id;

  void __init__(string id) {this->id = id;}

  string __str__() {return this->id;}
};

class AbsExpr : public Expr
{
  string var;
  string expr;

  template <typename T>
  void __init__(T var, string e1) {
    if (typeid(var).name() == "string")
      this->var = VarDecl(var);
    else
      this->var = var;
    this->expr = e1;
  }

  string __str__() {return "{" + this->var + "}.{" + this->expr + "}";}
};

class AppExpr : public Expr
{
  string lhs;
  string rhs;

  void __init__(string lhs,string rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "{" + this->lhs + "} {" + this->rhs + "}";}
};

class LambdaExpr : public Expr
{
  vector<string> vars;
  string expr;

  void __init__(vector<string> vars,string e1) {
    for (int i=0; i<vars.size(); ++i) {
      this->vars.push_back(vars[i]);
    }
    this->expr = e1;
  }

};

class CallExpr
{
  string fn;
  string args;

  void __init__(string fn, string args) {
    this->fn = fn;
    this->args = args;
  }
};

template <typename T>
bool is_value(T e) {
  return (typeid(T).name() == "IdExpr") || (typeid(T).name() == "AbsExpr") || (typeid(T).name() == "LambdaExpr");
}

template <typename T>
bool is_reducible(T e) {
  return !is_value(e);
}

template <typename T>
void resolve(T e, vector<T> scope) {
  if (typeid(e).name() == "AppExpr") {
    resolve(e.lhs, scope);
    resolve(e.rhs, scope);
    return;
  }

  if (typeid(e).name() == "AbsExpr") {
    resolve(e.expr, scope.push_back(e));
    return;
  }

  if (typeid(e).name() == "IdExpr") {
    reverse(scope.begin(),scope.end());
    for (int i=0; i<scope.size(); ++i) {
      if (e.id == scope[i].id) {
        e.ref = scope[i];
        return;
      }
    }
  }

  throw "name lookup error";
}

template <typename T>
T subst(T e,vector<T> s) {
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
}

template <typename T>
T step_app(T e) {
  if (is_reducible(e.lhs))
    return AppExpr(step(e.lhs),e.rhs);
  if (typeid(e.lhs).name() != "AbsExpr")
    throw "application of non-lambda";
  if (is_reducible(e.rhs))
    return AppExpr(e.lhs,step(e.rhs));

  vector<T> s;
  s.push_back(e.lhs);
  s.push_back(e.rhs);
  return subst(e.lhs.expr, s);
}

template <typename T>
T step(T e) {
  assert (typeid(e).name() == "Expr");
  assert (is_reducible(e));

  if (typeid(e).name() == "AppExpr")
    return step_app(e);
}

#endif