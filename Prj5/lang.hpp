#ifndef LANG_HPP_INCLUDED
#define LANG_HPP_INCLUDED

#include <string>
#include <vector>
#include <typeinfo>
#include <iterator>
#include <assert.h>

using namespace std;

template <typename T>
T exp(T x);

template<typename T>
class VarDecl
{
  string id;
  T type;

  void __init__(string id,T t) {
    this->id = id;
    this->type = typify(t);
  }

  string __str__() {return this->id + to_string(this->type);}
};

template<typename T>
class FieldDecl {
  string id;
  T type;

  void __init__(string id,T t) {
    this->id = id;
    this->type = typify(t);
  }
  string __str__() {return this->id + to_string(this->type);}

};

template<typename T>
class FieldInit {
  string id;
  T value;

  void __init__(string id,T e) {
    this->id = id;
    this->type = exp(e);
  }
  string __str__() {return this->id + to_string(this->e);}

};

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

template <typename T>
class TupleType : public Type {
  vector<T> elems;

  void __init__(T ts) {
    elems = typify(ts);
  }

  string __str__() {
    string bigStr="";
    for (auto it=elems.begin(); it!=elems.end(); ++it)
      bigStr += to_string(*it) + ",";
    return bigStr;
  }
};

template <typename T>
class RecordType : public Type {
  vector<T> fields;

  void __init__(T fs) {
    fields = field(fs);
  }

  string __str__() {
    string bigStr="";
    for (auto it=fields.begin(); it!=fields.end(); ++it)
      bigStr += to_string(*it) + ",";
    return bigStr;
  }
};

template <typename T>
class VariantType : public Type {
  vector<T> fields;

  void __init__(T fs) {
    fields = field(fs);
  }

  string __str__() {
    string bigStr="<";
    for (auto it=fields.begin(); it!=fields.end(); ++it)
      bigStr += to_string(*it) + ",";
    return bigStr + ">";
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
    this->cond = exp(e1);
    this->True = exp(e2);
    this->False = exp(e3);
  }
};

class IdExpr : public Expr {
  string value;

  void __init__(string val) {
    this->value = val;
  }

  string __str__() {return value;}
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

class NewExpr : public Expr {
  string expr;

  void __init__(string e) {
    this->expr = exp(e);
  }

  string __str__() {return "new " + this->expr;}
};

class DerefExpr : public Expr {
  string expr;

  void __init__(string e) {
    this->expr = exp(e);
  }

  string __str__() {return "*" + this->expr;}
};

class AssignExpr : public Expr {
  string lhs;
  string rhs;

  void __init__(string e1,string e2) {
    this->lhs = exp(e1);
    this->rhs = exp(e2);
  }

  string __str__() {return this->lhs + "=" + this->rhs;}
};

template <typename T>
class TupleExpr : public Expr {
  vector<T> elems;

  void __init__(vector<T> es) {
    this->elems = es;
  }

  string __str__() {
    string bigStr="";
    for (auto it=elems.begin(); it!=elems.end(); ++it)
      bigStr += to_string(*it) + ",";
    return bigStr;
  }
};

template <typename T>
class ProjExpr : public Expr {
  T obj;
  int index;

  void __init__(T e1,int n) {
    this->obj = e1;
    this->index = n;
  }

  string __str__() {
    return to_string(this->obj) + "." + this->index;
  }
};

template <typename T>
class RecordExpr : public Expr {
  vector<T> fields;

  void __init__(vector<T> fs) {
    this->fields = fs;
  }

  string __str__() {
    string bigStr="";
    for (auto it=fields.begin(); it!=fields.end(); ++it)
      bigStr += to_string(*it) + ",";
    return bigStr;
  }
};

template <typename T>
class MemberExpr : public Expr {
  T obj;
  int id;
  string Ref;

  void __init__(T e1,int id) {
    this->obj = e1;
    this->id = id;
    this->Ref = "None";
  }

  string __str__() {
    return to_string(this->obj) + "." + to_string(id);
  }
};

template <typename T1, typename T2>
class VariantExpr : public Expr {
  T1 field;
  T2 variant;

  void __init__(T1 f,T2 t) {
    this->field = init(f);
    this->t = typify(t);
  }

  string __str__() {
    return "<" + this->field + "> as " + this->variant;
  }
};

template <typename T>
class Case {
  int id;
  T var;
  string expr;

  void __init__(int id, T n, string e) {
    this->id = id;
    this->var = VarDecl<T>("None",n);
    this->expr = exp(e);
  }

  string __str__() {
    return "<" + to_string(this->id) + "=" + to_string(this->var) + "> => " + this->expr;
  }
};

template <typename T>
class CaseExpr : public Expr {
  string expr;
  vector<T> Cases;

  void __init__(string e,T cs) {
    this->expr = exp(e);
    Cases.push_back(cs);
  }
};

template <typename T>
T typify(T x) {
  if (typeid(x).name() == "bool")
    return "BoolType";
  if (typeid(x).name() == "int")
    return "IntType";
  return x;
}

template <typename T>
T exp(T x) {
  if(typeid(x).name() == "bool") {
    BoolExpr expr;
    expr.__init__(x);
    return expr;
  }

  if(typeid(x).name() == "int") {
    IntExpr<T> expr;
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
    return VarDecl<T>(x);
  return x;
}

template <typename T>
T field(T x) {
  if (typeid(x).name() == "tuple")
    return FieldDecl<T>(x[0],x[1]);
  return x;
}

template <typename T>
T init(T x) {
  if (typeid(x).name() == "tuple")
    return FieldInit<T>(x[0],x[1]);
  return x;
}

template <typename T>
T cCase(T x) {
  if (typeid(x).name() == "tuple")
    return Case<T>(x[0],x[1],x[2]);
  return x;
}

#endif