#ifndef LANG_HPP_INCLUDED
#define LANG_HPP_INCLUDED

#include <string>
#include <vector>
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
  vector<string> args;

  void __init__(string fn, vector<string> args) {
    this->fn = fn;
    this->args = args;
  }
};

class PlaceholderExpr : public Expr
{
  string __str__() {return "_";}
};

template <typename T>
class NewExpr : public Expr {
  string expr;

  void __init__(T e) {
    this->expr = to_string(e);
  }

  string __str__() {
    return "new {" + this->expr + "}";
  }
};

template <typename T>
class DerefExpr : public Expr {
  string expr;

  void __init__(T e) {
    this->expr = to_string(e);
  }

  string __str__() {
    return "*{" + this->expr + "}";
  }
};

template <typename T>
class AssignExpr : public Expr {
  string lhs;
  string rhs;

  void __init__(T e1,T e2) {
    this->lhs = to_string(e1);
    this->rhs = to_string(e2);
  }

  string __str__() {
    return "{" this->lhs + "} = {" + this->rhs + "}";
  }
};

template <typename T>
T expr(T x) {
  if (typeid(x).name() == "bool")
    return BoolExpr(x);
  if (typeid(x).name() == "string")
    return IdExpr(x);
  return x;
}

template <typename T>
T decl(T x) {
  if (typeid(x).name() == "string")
    return VarDecl(x);
  return x;
}


#endif