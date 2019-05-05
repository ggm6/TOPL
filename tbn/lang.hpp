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

class Expr {
  string type;

  void __init__() {
    this->type = "None";
  }
};
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

class IdExpr : public Expr {
  string value;

  void __init__(string val) {
    this->value = val;
  }

  string __str__() {return value;}
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

template<typename T>
class IntExpr : public Expr {
  T value;

  void __init__(T val) {
    this->value = val;
  }

  string __str__() {return to_string(this->value);}
};

template<typename T>
class AddExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} + {" + to_string(this->rhs) + "})";}
};

template<typename T>
class SubExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} - {" + to_string(this->rhs) + "})";}
};

template<typename T>
class MulExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} * {" + to_string(this->rhs) + "})";}
};

template<typename T>
class DivExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} / {" + to_string(this->rhs) + "})";}
};

template<typename T>
class RemExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} % {" + to_string(this->rhs) + "})";}
};

template<typename T>
class NegExpr : public Expr {
  T expr;

  void __init__(T e1) {
    this->expr = e1;
  }

  string __str__() {return "(-{" + this->expr + "})";}
};

template<typename T>
class EqExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} == {" + to_string(this->rhs) + "})";}
};

template<typename T>
class NeExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} != {" + to_string(this->rhs) + "})";}
};

template<typename T>
class LtExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} < {" + to_string(this->rhs) + "})";}
};

template<typename T>
class GtExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} > {" + to_string(this->rhs) + "})";}
};

template<typename T>
class LeExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} <= {" + to_string(this->rhs) + "})";}
};

template<typename T>
class GeExpr : public Expr {
  T lhs;
  T rhs;

  void __init__(T lhs,T rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
  }

  string __str__() {return "({" + to_string(this->lhs) + "} >= {" + to_string(this->rhs) + "})";}
};

template <typename T1,typename T2>
T2 expr(T1 x) {
  if(typeid(x).name() == "bool") {
    BoolExpr expr;
    expr.__init__(x);
    return expr;
  }

  if(typeid(x).name() == "int") {
    IntExpr<T1> expr;
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

#endif