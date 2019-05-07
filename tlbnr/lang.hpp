#ifndef LANG_HPP_INCLUDED
#define LANG_HPP_INCLUDED

#include <string>
#include <vector>
#include <typeinfo>
#include <assert.h>

using namespace std;

class Type {};

class BoolType : public Type {
  string __str__() {return "Bool";}
};

class IntType : public Type {
  string __str__() {return "Int";}
};

template<typename T1,typename T2>
class FnType : public Type {
  vector<T1> parms;
  T2 ret;

  void __init__(vector<T1> parms,T2 ret) {
    this->parms = parms;
    this->ret = ret;
  }
};

template<typename T>
class RefType : public Type {
  T ref;

  void __init__(T t) {
    this->ref = t;
  }
};

BoolType boolType;
IntType intType;

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

class NotExpr : public Expr
{

  string expr;


  void __init__(string e) {this->expr = e;}

  string __str__() {return "(not {" + this->expr + "})";}
};

template <typename T1,typename T2,typename T3>
class IfExpr : public Expr
{
  T1 cond;
  T2 True;
  T3 False;

  void __init__(T1 e1, T2 e2, T3 e3) {
    this->cond = expr(e1);
    this->True = expr(e2);
    this->False = expr(e3);
  }
};

class IdExpr : public Expr {
  string value;

  void __init__(string val) {
    this->value = val;
  }

  string __str__() {return value;}
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
  vector<string> args;

  void __init__(string fn, vector<string> args) {
    this->fn = fn;
    this->args = args;
  }

  string __str__() {
    return "{self.fn} ({args})";
  }
};

class PlaceholderExpr : public Expr
{
  string __str__() {return "_";}
};

template <typename T1,typename T2>
T2 expr(T1 x) {
  if(typeid(x).name() == "bool") {
    BoolExpr expr;
    expr.__init__(x);
    return expr;
  }

  if(typeid(x).name() == "string") {
    IdExpr expr;
    expr.__init__(x);
    return expr;
  }
  return x;
}

template <typename T>
T decl(T x) {
  if (typeid(x).name() == "string")
    return VarDecl(x);
  return x;
}

#endif